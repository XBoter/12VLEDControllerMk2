
//Config for Functions of the Controller
#define LED_STRIP_COUNT 2   //Defines how many LED Strips are Controlled    (Options: 1 and 2)
#define MOTION_SENSORS 1    //Defines how many Motion Sensors are available (Options: 0, 1 and 2)
#define DHT_SENSOR 0        //Defines how many DHT Sensors are available    (Options: 0 and 1)
#define IR_RECIVER 1        //Defines how many IR Receiver are available    (Options: 0 and 1)


//+++ Include Libarys +++//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <IRremoteESP8266.h>
//#include <IRrecv.h>
//#include <IRutils.h>
//#include <DHT.h>
//#include <DHT_U.h>

//+++ Secret Header +++//
#define Controller_Desk
//#define Controller_Bed
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

//-------------------- Basic Information --------------------//
//for Serial print Startup Info
#define Name        "12V LED Controller Mk2"
#define Programmer  "Nico Weidenfeller"
#define Created     "06.06.2019"
#define LastModifed "20.06.2019"
#define Version     "0.0.3"

/*
  Name          :   12V LED Controller Mk2
  Programmer    :   Nico Weidenfeller
  Created       :   06.06.2019
  Last Modifed  :   20.06.2019
  Version       :   0.0.3
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
                      Fixed Information Print bugs.

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

//*************************************************************************************************//
//---------------------------------------------- API ----------------------------------------------//
//*************************************************************************************************//
WiFiClient  http_Client;

//*************************************************************************************************//
//----------------------------------------------- DHT ---------------------------------------------//
//*************************************************************************************************//


//*************************************************************************************************//
//----------------------------------------------- IR ----------------------------------------------//
//*************************************************************************************************//


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

//---- Normal Light ----//

//---- Party ----//

//---- Weekend ----//

//---- Force ----//

//---- Good Night ----//

//---- Error_NoWiFiConnection ----//
uint8_t BrightnessErrorNoWiFiConnection = 80;
uint8_t SwapFadeErrorNoWiFiConnection = true;

//---- Error_NoMqttConnection ----//
uint8_t BrightnessErrorNoMqttConnection = 80;

//---- Error_GerneralError ----//
uint8_t BrightnessErrorGeneral = 80;



//*************************************************************************************************//
//--------------------------------------------- Network -------------------------------------------//
//*************************************************************************************************//
WiFiClient    wifiMqtt;
PubSubClient  mqtt_Client(wifiMqtt);
boolean StartWifi = true;
uint8_t NoWiFiCounter = 0;

//---- Parameter List ----//

//# Global #//
uint8_t mqtt_Global_Color_Fadespeed      = 20; //Needs Init for Startup with WiFi and MQTT Effect
uint8_t mqtt_Global_Brightness_Fadespeed = 20; //""
uint8_t mqtt_Global_Party                = 0;
uint8_t mqtt_Global_Weekend              = 0;
uint8_t mqtt_Global_Force                = 0;
uint8_t mqtt_Global_GoodNight            = 0;

//# Specific #//
//---- LED ----//
uint8_t mqtt_LED_Active         = 0;
uint8_t mqtt_LED_Red            = 0;
uint8_t mqtt_LED_Green          = 0;
uint8_t mqtt_LED_Blue           = 0;
uint8_t mqtt_LED_Brightness     = 0;
uint8_t mqtt_LED_Active_2       = 0;
uint8_t mqtt_LED_Red_2          = 0;
uint8_t mqtt_LED_Green_2        = 0;
uint8_t mqtt_LED_Blue_2         = 0;
uint8_t mqtt_LED_Brightness_2   = 0;

//---- IR ----//
uint8_t mqtt_IR_Active          = 0;

//---- API ----//
uint8_t mqtt_API_Active         = 0;

//---- Motion ----//
uint8_t mqtt_Motion_Active      = 0;
uint8_t mqtt_Motion_Second      = 0;
uint8_t mqtt_Motion_Timout      = 0;

//*************************************************************************************************//
//------------------------------------------- Information -----------------------------------------//
//*************************************************************************************************//
uint16_t Information_MainState               = 0;
uint16_t Information_LightState              = 0;

uint8_t Information_mqtt_Global_Color_Fadespeed      = 0;
uint8_t Information_mqtt_Global_Brightness_Fadespeed = 0;
uint8_t Information_mqtt_Global_Party                = 0;
uint8_t Information_mqtt_Global_Weekend              = 0;
uint8_t Information_mqtt_Global_Force                = 0;
uint8_t Information_mqtt_Global_GoodNight            = 0;

uint8_t Information_mqtt_LED_Active         = 0;
uint8_t Information_mqtt_LED_Red            = 0;
uint8_t Information_mqtt_LED_Green          = 0;
uint8_t Information_mqtt_LED_Blue           = 0;
uint8_t Information_mqtt_LED_Brightness     = 0;
uint8_t Information_mqtt_LED_Active_2       = 0;
uint8_t Information_mqtt_LED_Red_2          = 0;
uint8_t Information_mqtt_LED_Green_2        = 0;
uint8_t Information_mqtt_LED_Blue_2         = 0;
uint8_t Information_mqtt_LED_Brightness_2   = 0;

uint8_t Information_mqtt_IR_Active          = 0;

uint8_t Information_mqtt_API_Active         = 0;

uint8_t Information_mqtt_Motion_Active      = 0;
uint8_t Information_mqtt_Motion_Second      = 0;
uint8_t Information_mqtt_Motion_Timout      = 0;


//*************************************************************************************************//
//---------------------------------------------- Delay --------------------------------------------//
//*************************************************************************************************//

//Timer / Delay
unsigned long PrevMillis_Example                     = 0;
unsigned long PrevMillis_NoWiFiConnected             = 0;
unsigned long PrevMillis_ErrorNoWiFiConnection       = 0;
unsigned long PrevMillis_ColorFadeSpeed              = 0;
unsigned long PrevMillis_BrightnessFadeSpeedStrip1   = 0;
unsigned long PrevMillis_BrightnessFadeSpeedStrip2   = 0;
unsigned long PrevMillis_ColorFadeSpeedStrip1        = 0;
unsigned long PrevMillis_ColorFadeSpeedStrip2        = 0;

unsigned long TimeOut_Example                  = 1000;   // 1.00 Seconds
unsigned long TimeOut_NoWiFiConnected          = 1000;   // 1.00 Seconds
unsigned long TimeOut_ErrorEffectSpeed         = 10;     // 0.01 Seconds

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
