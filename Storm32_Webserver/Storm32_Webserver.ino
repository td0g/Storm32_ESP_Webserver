/*
  ESP8266-01 Code for Storm32 Webserver
  Written by Tyler Gerritsen
  vtgerritsen@gmail.com
  www.td0g.ca

DESCRIPTION:
  Power ESP8266 with separate 3.3V source, connect TX, RX, and GND pins to Storm32 board
  Serves as a standalone webserver to control motion and parameters

CHANGELOG:
  0.1   2021-10-10
    Initial Commit


LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

NOTES:
  https://os.mbed.com/users/shimniok/code/AVC_2012/file/826c6171fc1b/MAVlink/include/checksum.h/ for crc
  GUI Line 222 for option list
  GUI line 6144 for Status list
  GUI line 10888 for RC Motion Control

ToDo:
  Pan Mode Control (reply[120]) and Pan Mode Setting 3 (reply[124])
  Add controls for all parameters
  Improve motion control page
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#define SERVER_NAME "EDWARD"
#define PAYLOAD_SIZE 391 //263
#define EEPROM_STRING_SIZE 9
#define EEPROM_BLOCK_SIZE (PAYLOAD_SIZE + EEPROM_STRING_SIZE)
#define PRESET_COUNT 4
#define SERIAL_TIMEOUT 400
#define EEPROM_SIZE EEPROM_BLOCK_SIZE * PRESET_COUNT + 64
#define EEPROM_BAUD EEPROM_SIZE - 1
#define REPLY_BUFFER_SIZE 500


const char *ssid = SERVER_NAME;
String serverName = SERVER_NAME;
char reply[REPLY_BUFFER_SIZE];
int replyLength = 0;
String dataList[32] = {"State","Status","Status2","Status3","Performance","Errors","Voltage","Millis","Cycle Time","IMU1Pitch","IMU1Roll","IMU1Yaw","IMU1Xr(pitch)","IMU1Yr(yaw)","IMU1Zr(roll)","PIDoutPitch","PIDoutRoll","PIDoutYaw","RCinputPitch","RCinputRoll","RCinputYaw","IMU2Pitch","IMU2Roll","IMU2Yaw","PIDoutPitchr","PIDoutRollr","PIDoutYawr","?","?","?","IMU1AHRSaccMag","IMU1AHRSaccConf"};
uint16_t crc;       
byte page = 0;
byte messageCode = 0;
byte activeParams[391];
String errorMsg = "";
const byte gpio0_pin = 0;
const byte gpio2_pin = 2;  //LED pin
byte baud = 1;


//Messages (line 7535)
#define STORM32_MSG_STORE_TO_EEPROM "xs"
#define STORM32_MSG_RESTART "xx"

#define X25_INIT_CRC 0xffff
#define X25_VALIDATE_CRC 0xf0b8

#define WEBPAGE_MAIN 0
#define WEBPAGE_PRESETS 1
#define WEBPAGE_PRESET_SAVE 2
#define WEBPAGE_GET_PARAMS 3
#define WEBPAGE_LIVE_DATA 4
#define WEBPAGE_VIEW_PRESETS 5
#define WEBPAGE_NAME_PRESETS 6
#define WEBPAGE_ADVANCED 7
#define WEBPAGE_MOTION 8
#define WEBPAGE_PID 9
#define WEBPAGE_BAUD 10
#define WEBPAGE_CUSTOM_ERROR 128


#define PARAM_GYRO_LPF 12
#define PARAM_PITCH_MOTOR_POLES 13
#define PARAM_PITCH_MOTOR_DIR 14
#define PARAM_PITCH_MOTOR_STARTUP_DIR 15
#define PARAM_PITCH_MOTOR_STARTUP_POS 16
#define PARAM_ROLL_MOTOR_POLES 17
#define PARAM_ROLL_MOTOR_DIR 18
#define PARAM_ROLL_MOTOR_STARTUP_POS 19
#define PARAM_YAW_MOTOR_POLES 20
#define PARAM_YAW_MOTOR_DIR 21
#define PARAM_YAW_MOTOR_STARTUP_POS 22
#define PARAM_UARTX 48
#define PARAM_LIPO_CELLS 49
#define PARAM_LIPO_VOLTAGE 50
#define PARAM_IMU_ORIENTATION 51
#define PARAM_IMU_MAPPING 52
#define PARAM_IMU2_CONFIG 53
#define PARAM_IMU2_ORIENTATION 54
#define PARAM_PITCH_MOTOR_USAGE 55
#define PARAM_ROLL_MOTOR_USAGE 56
#define PARAM_YAW_MOTOR_USAGE 57
#define PARAM_PITCH_OFFSET 58
#define PARAM_ROLL_OFFSET 59
#define PARAM_YAW_OFFSET 60
#define PARAM_IMU_AHRS 61
#define PARAM_ACC_COMP_METHOD 65
#define PARAM_MAVLINK_COM_PORT 66
#define PARAM_UART_BAUD 67
#define PARAM_UART2_BAUD 68
#define PARAM_USB_BAUD 69
#define PARAM_ACC_LPF 70
#define PARAM_IMU2_FEEDFORWARD_LPF 71
#define PARAM_ROLL_YAW_PD_MIXING 72
#define PARAM_LOW_VOLTAGE_LIMIT 73
#define PARAM_VOLTAGE_CORRECTION 75
#define PARAM_ADC_CALIBRATION 76
#define PARAM_VIRTUAL_CHANNEL_CONFIG 77
#define PARAM_PWM_OUT_CONFIG 78
#define PARAM_PAN_MODE_CONTROL 79
#define PARAM_PAN_MODE_DEFAULT_SETTING 80
#define PARAM_PAN_MODE_SETTING_1 81
#define PARAM_PAN_MODE_SETTING_2 82
#define PARAM_PAN_MODE_SETTING_3 83
#define PARAM_PITCH_PAN 84
#define PARAM_PITCH_PAN_DEADBAND 85
#define PARAM_PITCH_PAN_EXPO 86
#define PARAM_ROLL_PAN 87
#define PARAM_ROLL_PAN_DEADBAND 88
#define PARAM_ROLL_PAN_EXPO 89
#define PARAM_YAW_PAN 90
#define PARAM_YAW_PAN_DEADBAND 91
#define PARAM_YAW_PAN_EXPO 92
#define PARAM_YAW_PAN_LPF 93
#define PARAM_HOLD_PAN_TRANSITION_TIME 95
#define PARAM_RC_DEADBAND 96
#define PARAM_RC_HESTERESIS 97
#define PARAM_RC_ADC_LPF 98
#define PARAM_RC_PITCH_OFFSET 99
#define PARAM_RC_ROLL_OFFSET 100
#define PARAM_RC_YAW_OFFSET 101
#define PARAM_RC_PITCH 102
#define PARAM_RC_PITCH_MODE 103
#define PARAM_RC_PITCH_TRIM 104
#define PARAM_RC_PITCH_MIN 105
#define PARAM_RC_PITCH_MAX 106
#define PARAM_RC_PITCH_SPEED_LIMIT 107
#define PARAM_RC_PITCH_ACCEL_LIMIT 108
#define PARAM_RC_ROLL 109
#define PARAM_RC_ROLL_MODE 110
#define PARAM_RC_ROLL_TRIM 111
#define PARAM_RC_ROLL_MIN 112
#define PARAM_RC_ROLL_MAX 113
#define PARAM_RC_ROLL_SPEED_LIMIT 114
#define PARAM_RC_ROLL_ACCEL_LIMIT 115
#define PARAM_RC_YAW 116
#define PARAM_RC_YAW_MODE 117
#define PARAM_RC_YAW_TRIM 118
#define PARAM_RC_YAW_MIN 119
#define PARAM_RC_YAW_MAX 120
#define PARAM_RC_YAW_SPEED_LIMIT 121
#define PARAM_RC_YAW_ACCEL_LIMIT 122
#define PARAM_STANDBY 123
#define PARAM_RE_CENTRE 124
#define PARAM_CAMERA_CONTROL 125
#define PARAM_CAMERA_MODEL 126
#define PARAM_CAMERA_CONTROL_MODE 127
#define PARAM_CAMERA_TIME_INTERVAL 129
#define PARAM_PWM_OUT_CONTROL 130
#define PARAM_PWM_OUT_MID 131
#define PARAM_PWM_OUT_MIN 132
#define PARAM_PWM_OUT_MAX 133
#define PARAM_PWM_OUT_SPEED_LIMIT 134
#define PARAM_RE_CENTRE 135
#define PARAM_STORM32_CONFIG 137
#define PARAM_CAMERA_CONTROL_2 138
#define PARAM_MAVLINK_CAMERA 139
#define PARAM_STARTUP_MODE 140
#define PARAM_STARTUP_DELAY 141
#define PARAM_NT_LOGGING 142
#define PARAM_BEEP 143
#define PARAM_MAVLINK_GIMBAL 144
#define PARAM_MAVLINK_SYSTEM_ID 145
#define PARAM_MAVLINK_GIMBAL_STREAM 146
#define PARAM_ESP_CONFIG 147
#define PARAM_FILTER_1_SOURCE 150
#define PARAM_FILTER_1_FREQ 151
#define PARAM_FILTER_1_BANDWIDTH 152
#define PARAM_FILTER_1_ATTENUATION 153
#define PARAM_FILTER_2_SOURCE 154
#define PARAM_FILTER_2_FREQ 155
#define PARAM_FILTER_2_BANDWIDTH 156
#define PARAM_FILTER_2_ATTENUATION 157
#define PARAM_FILTER_3_SOURCE 158
#define PARAM_FILTER_3_FREQ 159
#define PARAM_FILTER_3_BANDWIDTH 160
#define PARAM_FILTER_3_ATTENUATION 161
#define PARAM_SERVO_PITCH_P 162
#define PARAM_SERVO_PITCH_I 163
#define PARAM_SERVO_PITCH_D 164
#define PARAM_SERVO_PITCH_DLPF 165
#define PARAM_SERVO_ROLL_P 166
#define PARAM_SERVO_ROLL_I 167
#define PARAM_SERVO_ROLL_D 168
#define PARAM_SERVO_ROLL_DLPF 169
#define PARAM_SERVO_YAW_P 170
#define PARAM_SERVO_YAW_I 171
#define PARAM_SERVO_YAW_D 172
#define PARAM_SERVO_YAW_DLPF 173
#define PARAM_CAMERA_COM_PORT 174
#define PARAM_FOC_CONTROLLER 175
#define PARAM_T1_PITCH 176
#define PARAM_T1_ROLL 177
#define PARAM_T1_YAW 178
#define PARAM_T2_ROLL 179
#define PARAM_T2_YAW 180
#define PARAM_T3_YAW 181
#define PARAM_D_SOURCE 182
#define PARAM_CONTROLLER 183



MDNSResponder mdns;
ESP8266WebServer server(80);

String htmlButton = "<input type=\"submit\" style=\"font-size:33pt;height:100px;width:500px;\" ";

String webPageMain = "<h1>" + serverName + " (Storm32)</h1><form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Presets\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Info\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Read_Params\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Live_Data\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Motion\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Advanced\" ?></form><form action=\"/act\">Command:<br><input type=\"text\" name=\"action\" style=\"font-size:100pt;height:200px;width:500px;\" value=\"\"><br><br>" + htmlButton + "value=\"Submit\"></form>";
String webPagePresets0 = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Save_Preset\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Name_Preset\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"View_Presets\" ?><br><br><h1>Load Presets</h1><br>";
String webPagePresets1 = "<br>" + htmlButton + "name=\"Page\" value=\"loadOne\" ?><br><br>";
String webPagePresets2 = "<br>" + htmlButton + "name=\"Page\" value=\"loadTwo\" ?><br><br>";
String webPagePresets3 = "<br>" + htmlButton + "name=\"Page\" value=\"loadThree\" ?><br><br>";
String webPagePresets4 = "<br>" + htmlButton + "name=\"Page\" value=\"loadFour\" ?></form>";
String webPagePresetsSave = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + "<h1>Save Preset</h1><br>" + htmlButton + "name=\"Page\" value=\"One\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Two\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Three\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Four\" ?></form>";
String webPagePresetsName = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + "<h1>Name Preset</h1><br>" + htmlButton + "name=\"Page\" value=\"nameOne\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"nameTwo\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"nameThree\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"nameFour\" ?><br><br>Name:<br><input type=\"text\" name=\"preset_name\" style=\"font-size:100pt;height:200px;width:500px;\" value=\"\"></form>";
String webPageAdvanced = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Save_To_EEPROM\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Reset\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"set_Baud\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"set_PID\" ?><br>";

String webPageMotion0 = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Pitch_UP\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Pitch_MID\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Pitch_DOWN\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Pitch_CENTRE\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Roll_UP\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Roll_MID\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Roll_DOWN\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Roll_CENTRE\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Yaw_RIGHT\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Yaw_MID\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Yaw_LEFT\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Yaw_CENTRE\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"Level\" ?><br><br><h1>Manual Position</h1><br>" + htmlButton + "name=\"Page\" value=\"Refresh_Angle\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"GoTo_Angle\" ?><br><br><label style=\"font-size:120pt;\">Pitch</label><input type=\"text\" name=\"angle_0\" style=\"font-size:100pt;height:200px;width:500px;\" value=\"";
String webPageMotion1 = "\"><br><label style=\"font-size:120pt;\">Roll</label><input type=\"text\" name=\"angle_1\" style=\"font-size:100pt;height:200px;width:500px;\" value=\"";
String webPageMotion2 = "\"><br><label style=\"font-size:120pt;\">Yaw</label><input type=\"text\" name=\"angle_2\" style=\"font-size:100pt;height:200px;width:500px;\" value=\"";
String webPageMotion3 = "\">";

String webPagePID0 = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br><h1>Set PID</h1><br>" + htmlButton + "name=\"Page\" value=\"setPID_Now\" ?><br><br>";
String webPagePID1 = "\"><br><br>";
String webPagePID2 = "<input type=\"text\" name=\"";
String webPagePID3 = "\" style=\"font-size:100pt;height:200px;width:440px;\" value=\"";
String webPagePID4 = "</form>";

String webPageBaud = "<form action=\"/page\">" + htmlButton + "name=\"Page\" value=\"Home\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"setBaud9600\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"setBaud19200\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"setBaud115200\" ?><br><br>" + htmlButton + "name=\"Page\" value=\"setBaud250000\" ?></form>";



void setup(void){
  //Load settings
  EEPROM.begin(EEPROM_SIZE);
  byte _b = EEPROM.read(EEPROM_BAUD);
  if ((_b & 0b00001111) == ((_b >> 4) & 0b00001111)) {
    _b = _b & 0b00001111;
    if (_b < 5) baud = _b;
  }

  //Setup Serial connection with controller
  setupSerial();

  //Setup WiFi
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();

  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);

  //Run MDNS
  setupMDNS();

}
 
void loop(void){
  server.handleClient();
} 
