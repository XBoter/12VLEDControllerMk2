
//Config for Functions of the Controller (Prio from Top to Bottom)
#define LED_STRIP_COUNT 1   //Defines how many LED Strips are Controlled    (Options: 1 and 2)
#define MOTION_SENSORS 1    //Defines how many Motion Sensors are available (Options: 0, 1 and 2)
#define IR_RECIVER 1        //Defines how many IR Receiver are available    (Options: 0 and 1)
#define DHT_SENSOR 1        //Defines how many DHT Sensors are available    (Options: 0 and 1)

//+++ Include Libarys +++//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//+++ Secret Header +++//
//#define Controller_Desk
#define Controller_Bed
//#define Controller_WardrobeMiddle
//#define Controller_Shelf
//#define Controller_TvStand
//#define Controller_Kitchen
//#define Controller_Bath

//Used for New Network Parameter
#define NicoNetworkSettings

#include <secrets.h>

//+++ DEBUG OPTIONS +++//
#define DEBUG_MAIN_STATE
#define DEBUG_LIGHT_STATE
#define DEBUG_MQTT_PARAMETER
#define DEBUG_API_DATA
#define DEBUG_MOTION
#define DEBUG_IR
#define DEBUG_DHT

//-------------------- Basic Information --------------------//
//for Serial print Startup Info
#define Name        "12V LED Controller Mk2"
#define Programmer  "Nico Weidenfeller"
#define Created     "06.06.2019"
#define LastModifed "23.06.2019"
#define Version     "1.0.1"

/*
  Name          :   12V LED Controller Mk2
  Programmer    :   Nico Weidenfeller
  Created       :   06.06.2019
  Last Modifed  :   23.06.2019
  Version       :   1.0.1
  Description   :

  ToDoList      :   - Check Pin Configuration for IR, Motion, LED, DHT
                    - Add Error Mode for No API Connection if API is used
                    - Add Retain Flag to mqtt.publish !!!! But not by Global Paths !!!!

  Bugs          :   -

  Optimize      :   -

  Patchnotes    :   Version 0.0.1
                      Init Code for Mk2 LED Controller for 12V LED Strips with DHT, API, IR, Motion Detection Support.
                    Version 0.0.2
                      Worked on the Pin Config. Added WiFi, MQTT and Information basis code.
                    Version 0.0.3
                      Fixed Information Print bugs. Added Brightness and Color Fade.
                    Version 0.0.4
                      Added Error Effects and Color Effect Modes.
                    Version 0.0.5
                      Added API support. And Master Present switch. Added Motion Tab
                    Version 0.0.6
                      Added Motion Detection and HeartBeat.
                    Version 0.0.7
                      Added IR and fixed bugs.
                    Version 0.0.8
                      improved Information Tab. Added DHT Sensor Read.
                    Version 1.0.0
                      First Finished Versione of the LED Controller MK2. Fixed Bug with the Motion Detection.
                    Version 1.0.1
                      Fixed DHT Mqtt publish Error.

*/

//Pin Defines for Out / Inputs
//PINs D1, D2, D3
#define PIN_LED_STRIP_1_RED D3
#define PIN_LED_STRIP_1_GREEN D2
#define PIN_LED_STRIP_1_BLUE D1

//PINs D5, D6, D7
#define PIN_LED_STRIP_2_RED D7
#define PIN_LED_STRIP_2_GREEN D6
#define PIN_LED_STRIP_2_BLUE D5
#define PIN_IR D5
#define PIN_DHT D6
#define PIN_MOTION_SENSOR_2 D6
#define PIN_MOTION_SENSOR_1 D7

//*************************************************************************************************//
//--------------------------------------------- SHARED --------------------------------------------//
//*************************************************************************************************//


//*************************************************************************************************//
//------------------------------------------- Controller ------------------------------------------//
//*************************************************************************************************//
uint16_t MainState = 0;
uint16_t LastMainState = 0;

uint16_t LightState = 0;
uint16_t LastLightState = 0;

boolean NoWiFiError = true;
boolean NoMqttError = true;
boolean NoApiError = true;

//*************************************************************************************************//
//---------------------------------------------- API ----------------------------------------------//
//*************************************************************************************************//
WiFiClient  http_Client;
uint8_t api_TimeHour = 0;
uint8_t api_TimeMinute = 0;
uint8_t api_SunDown = 0;

//*************************************************************************************************//
//----------------------------------------------- DHT ---------------------------------------------//
//*************************************************************************************************//
DHT dht(PIN_DHT, DHT22);
int Temperature = 0;
int Humidity = 0;

//*************************************************************************************************//
//----------------------------------------------- IR ----------------------------------------------//
//*************************************************************************************************//
IRrecv IrRecv(PIN_IR);
decode_results IrRecvResult;

//*************************************************************************************************//
//---------------------------------------------- LED ----------------------------------------------//
//*************************************************************************************************//
//---- MAIN LED VAR ----//
uint8_t ActualColorRedStrip_1   = 0;
uint8_t ActualColorGreenStrip_1 = 0;
uint8_t ActualColorBlueStrip_1  = 0;
uint8_t ActualBrightnessStrip_1 = 0;

uint8_t ActualColorRedStrip_2   = 0;
uint8_t ActualColorGreenStrip_2 = 0;
uint8_t ActualColorBlueStrip_2  = 0;
uint8_t ActualBrightnessStrip_2 = 0;

//---- Party ----//
uint8_t StatePartyMode      = 0;
uint8_t PartyColorRedStrip1   = 0;
uint8_t PartyColorGreenStrip1 = 0;
uint8_t PartyColorBlueStrip1  = 0;
uint8_t PartyColorRedStrip2   = 0;
uint8_t PartyColorGreenStrip2 = 0;
uint8_t PartyColorBlueStrip2  = 0;

//---- Weekend ----//
uint8_t StateWeekend = 0;
uint8_t WeekendColorRed = 255;
uint8_t WeekendColorGreen = 0;
uint8_t WeekendColorBlue = 0;

//---- Force ----//
uint8_t StateForce = 0;

//---- Good Night ----//
uint8_t StateGoodNight = 0;

//---- Error_NoWiFiConnection ----//
uint8_t BrightnessErrorNoWiFiConnection = 80;
uint8_t StateErrorNoWiFiConnection = 0;
boolean ErrorNoWiFiSwap = true;

//---- Error_NoMqttConnection ----//
uint8_t BrightnessErrorNoMqttConnection = 80;
uint8_t StateErrorNoMqttConnection = 0;

//---- Error_NoApiConnection ----//
uint8_t BrightnessErrorNoApiConnection = 80;
uint8_t StateErrorNoApiConnection = 0;

//---- Error_GerneralError ----//
uint8_t BrightnessErrorGeneral = 80;
uint8_t StateErrorGeneral = 0;


//*************************************************************************************************//
//-------------------------------------------- Motion ---------------------------------------------//
//*************************************************************************************************//
uint8_t MotionBrightness = 50;
uint8_t MotionColorRed   = 255;
uint8_t MotionColorGreen = 63;
uint8_t MotionColorBlue  = 0;

uint8_t MotionOccured = 0;
uint8_t PirSensor1MotionDetected = 0;
uint8_t PirSensor2MotionDetected = 0;

//*************************************************************************************************//
//--------------------------------------------- Network -------------------------------------------//
//*************************************************************************************************//
WiFiClient    wifiMqtt;
PubSubClient  mqtt_Client(wifiMqtt);
boolean StartWifi = true;
uint8_t NoWiFiCounter = 0;
uint16_t HeartBeatCounter = 0;

//---- Parameter List ----//

//# Global #//
uint8_t mqtt_Global_Color_Fadespeed      = 20; //Needs Init for Startup with WiFi and MQTT Effect
uint8_t mqtt_Global_Brightness_Fadespeed = 20; //""
uint16_t mqtt_Global_Good_Night_Timeout  = 0;
uint8_t mqtt_Global_Party                = 0;
uint8_t mqtt_Global_Weekend              = 0;
uint8_t mqtt_Global_Force                = 0;
uint8_t mqtt_Global_GoodNight            = 0;
uint8_t mqtt_Global_Motion_Active        = 0;
uint8_t mqtt_Global_MasterPresent        = 0;

//# Specific #//
//---- LED ----//
uint8_t mqtt_LED_Active_1       = 0;
uint8_t mqtt_LED_Red_1          = 0;
uint8_t mqtt_LED_Green_1        = 0;
uint8_t mqtt_LED_Blue_1         = 0;
uint8_t mqtt_LED_Brightness_1   = 0;

uint8_t mqtt_LED_Active_2       = 0;
uint8_t mqtt_LED_Red_2          = 0;
uint8_t mqtt_LED_Green_2        = 0;
uint8_t mqtt_LED_Blue_2         = 0;
uint8_t mqtt_LED_Brightness_2   = 0;

uint8_t mqtt_Motion_Timout      = 0;

//*************************************************************************************************//
//------------------------------------------- Information -----------------------------------------//
//*************************************************************************************************//
uint16_t Information_MainState               = 0;
uint16_t Information_LightState              = 0;

uint8_t Information_mqtt_Global_Color_Fadespeed      = 0;
uint8_t Information_mqtt_Global_Brightness_Fadespeed = 0;
uint16_t Information_mqtt_Global_Good_Night_Timeout  = 0;
uint8_t Information_mqtt_Global_Party                = 0;
uint8_t Information_mqtt_Global_Weekend              = 0;
uint8_t Information_mqtt_Global_Force                = 0;
uint8_t Information_mqtt_Global_GoodNight            = 0;
uint8_t Information_mqtt_Global_Motion_Active        = 0;
uint8_t Information_mqtt_Global_MasterPresent        = 0;

uint8_t Information_mqtt_LED_Active_1       = 0;
uint8_t Information_mqtt_LED_Red_1          = 0;
uint8_t Information_mqtt_LED_Green_1        = 0;
uint8_t Information_mqtt_LED_Blue_1         = 0;
uint8_t Information_mqtt_LED_Brightness_1   = 0;
uint8_t Information_mqtt_LED_Active_2       = 0;
uint8_t Information_mqtt_LED_Red_2          = 0;
uint8_t Information_mqtt_LED_Green_2        = 0;
uint8_t Information_mqtt_LED_Blue_2         = 0;
uint8_t Information_mqtt_LED_Brightness_2   = 0;

uint8_t Information_mqtt_Motion_Timout      = 0;

uint8_t Information_api_TimeHour            = 0;
uint8_t Information_api_TimeMinute          = 0;
uint8_t Information_api_SunDown             = 0;

uint8_t Information_PirSensor1MotionDetected = 0;
uint8_t Information_PirSensor2MotionDetected = 0;

uint8_t Information_IR_DataReceived         = 0;

uint8_t Information_DHT_DataRead            = 0;

//*************************************************************************************************//
//---------------------------------------------- Delay --------------------------------------------//
//*************************************************************************************************//

//Timer / Delay
unsigned long PrevMillis_Example                     = 0;
unsigned long PrevMillis_NoWiFiConnected             = 0;
unsigned long PrevMillis_ErrorNoWiFiConnection       = 0;
unsigned long PrevMillis_ErrorNoMqttConnection       = 0;
unsigned long PrevMillis_ErrorNoApiConnection        = 0;
unsigned long PrevMillis_ErrorGeneral                = 0;
unsigned long PrevMillis_ColorFadeSpeed              = 0;
unsigned long PrevMillis_BrightnessFadeSpeedStrip1   = 0;
unsigned long PrevMillis_BrightnessFadeSpeedStrip2   = 0;
unsigned long PrevMillis_ColorFadeSpeedStrip1        = 0;
unsigned long PrevMillis_ColorFadeSpeedStrip2        = 0;
unsigned long PrevMillis_EffectPartyMode             = 0;
unsigned long PrevMillis_EffectWeekendMode           = 0;
unsigned long PrevMillis_EffectForceMode             = 0;
unsigned long PrevMillis_EffectGoodNightMode         = 0;
unsigned long PrevMillis_ApiTimeUpdateRate           = 0;
unsigned long PrevMillis_ApiSunUpdateRate            = 0;
unsigned long PrevMillis_MotionDetected              = 0;
unsigned long PrevMillis_HeartBeat                   = 0;
unsigned long PrevMillis_ReatDHTSensorData           = 0;


unsigned long TimeOut_Example                  = 1000;   // 1.00 Seconds
unsigned long TimeOut_NoWiFiConnected          = 1000;   // 1.00 Seconds
unsigned long TimeOut_ErrorEffectSpeed         = 1000;   // 1.00 Seconds
unsigned long TimeOut_EffectWeekendMode        = 100;    // 0.10 Seconds
unsigned long TimeOut_EffectForceMode          = 60000;  // 1.00 Minute
unsigned long TimeOut_ApiUpdateRate            = 60000;  // 1.00 Minute
unsigned long TimeOut_HeartBeat                = 300000; // 5.00 Minute
unsigned long TimeOut_ReatDHTSensorData        = 60000;  // 1.00 Minute

/*
  unsigned long CurMillis_Example = millis();
  if (CurMillis_Example - PrevMillis_Example >= TimeOut_Example) {
    PrevMillis_Example = CurMillis_Example;
    Serial.println("Without Delay Test Message");
  }
*/

void setup() {
  SetupController();  //Setup gets Called in the Controller Tab. Main Ino is only for Var decleration
}

void loop() {
  LoopController();   //Loop gets Called in the Controller Tab. Main Ino is only for Var decleration
}
