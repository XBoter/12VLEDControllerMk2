//Libraries for ESP32
/*
  Error not Checked for ESP32
  #include <WiFi.h>
  #include <PubSubClient.h>
*/

//Libraries for ESP8266
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#ifdef IR_Function
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#ifdef DHT_Function
#include <DHT.h>
#include <DHT_U.h>
#endif

/*
  Name        :   LED Controller 12V MK2
  Programmer  :   Nico Weidenfeller
  Created     :   17.04.2018
  Version     :   2.0
  Description :
*/

/*
  ToDo:
  - Error Warning with blinking LED on the ESP8266
*/

//--------Functions--------//
//#define PIR_Function
//#define DHT_Function
//#define IR_Function
#define LED_Function

//--------DEBUG--------//
#define DEBUG          //Main Loop
//#define DEBUG_DHT      //DHT Sensor
//#define DEBUG_LED      //LED Controller
//#define DEBUG_IR       //IR Input
//#define DEBUG_MOTION   //MOTION Deteced etc
#define DEBUG_NETWORK  //WIFI MQTT etc

//---------------------------------Variable Deklaration----------------------------------//
//MQTT-Server Parameter
WiFiClient    wifiMqtt;
PubSubClient  mqtt_Client(wifiMqtt);
#define mqtt_server       "192.168.99.23"
#define mqtt_username     "pi"
#define mqtt_password     "raspberry"
#define mqtt_port         1883
#define mqtt_client_name  "OfficeShelfController1"

//HTTP-Server Parameter
WiFiClient  http_Client;
#define http_ip   "192.168.99.23"
#define http_port 8123

//WIFI Parameter
const char* wifi_ssid     = "Xenon";
const char* wifi_password = "50213393588677091934";

//PIR Sensore Parameter
#define PIR_SENSOR_ONE_PIN 0  //D5
#define PIR_SENSOR_TWO_PIN 0  //D6

//IR Sensore Paramter
#define IR_DETECTOR_PIN D7

//DH11 Sensor
#define DHT_SENSOR_PIN D7
#define DHT_SENSOR_TYPE DHT22

//LED Controller
#define LED_RED_PIN   D3
#define LED_GREEN_PIN D1
#define LED_BLUE_PIN  D5

//-------------------------------------Mqtt Paths--------------------------------------//
//LED
const char* mqtt_LED_setcolor       = "office/shelf/light/setcolor";
const char* mqtt_LED_getcolor       = "office/shelf/light/getcolor";
const char* mqtt_LED_setbrightness  = "office/shelf/light/setbrightness";
const char* mqtt_LED_getbrightness  = "office/shelf/light/getbrightness";
const char* mqtt_LED_setstatus      = "office/shelf/light/setstatus";
const char* mqtt_LED_getstatus      = "office/shelf/light/getstatus";
const char* mqtt_LED_seteffect      = "office/shelf/light/seteffect";
const char* mqtt_LED_geteffect      = "office/shelf/light/geteffect";
const char* mqtt_LED_FadeSpeed      = "office/shelf/light/fadespeed";

//IR
const char* mqtt_IR_Active          = "kitchen/shelf/ir/active";

//DHT
const char* mqtt_DHT_temperature    = "kitchen/shelf/dht/temp";
const char* mqtt_DHT_humidity       = "kitchen/shelf/dht/humidity";

//MOTION
const char* mqtt_MOTION_Color                     = "kitchen/shelf/motion/color";
const char* mqtt_MOTION_TimeOut                   = "kitchen/shelf/motion/timeout";
const char* mqtt_MOTION_Detection_Active          = "kitchen/shelf/motion/status";
const char* mqtt_MOTION_Brightness_Control_Active = "kitchen/shelf/motion/brightness/control";

//PARTY
const char* mqtt_PARTY_Active       = "protocol/house/party";

//API
const char* mqtt_API_Update_Allowed = "kitchen/shelf/api/update";

//-----------------------------------End Mqtt Paths------------------------------------//

//Time Variables for Delay
unsigned long PrevMillis_ApiUpdate      = 0;
unsigned long PrevMillis_MotionDeteced  = 0;
unsigned long PrevMillis_Fade           = 0;
unsigned long PrevMillis_CheckDHT       = 0;
unsigned long PrevMillis_PartyTime      = 0;
unsigned long PrevMillis_PartyTimeFade  = 0;

//Timeout Varibales for Delay
unsigned long TimeOut_ApiUpdate      = 5 * 60 * 1000;  // 5 Minute TimeOut
unsigned long TimeOut_MotionDeteced  = 1 * 10 * 1000;  // 10 Seconds
unsigned long TimeOut_Fade           = 20;             // 0.02 Seconds
unsigned long TimeOut_CheckDHT       = 5 * 60 * 1000;  // 5 Minute TimeOut
unsigned long TimeOut_PartyTime      = 500;            // 0.5 Seconds
unsigned long TimeOut_PartyTimeFade  = 20;             // 0.02 Seconds

//Color
int LED_Color[3]    = {255, 0, 0};
int MOTION_Color[3] = {255, 127, 0};
int PARTY_Color[3]  = {0, 255, 0};

int NEW_Color[3]    = {0, 0, 0};
int OLD_Color[3]    = {0, 0, 0};

//Brightness
int LED_Brightness    = 0;
int MOTION_Brightness = 50;
int PARTY_Brightness  = 0;

int NEW_Brightness    = 0;
int OLD_Brightness    = 0;

//Fade Speed
int LED_FadeSpeed     = 10;
int MOTION_FadeSpeed  = 10;
int PARTY_FadeSpeed   = 10;

//Functions Activated
boolean API_Active    = false;
boolean MOTION_Active = false;
boolean PARTY_Active  = false;
boolean BRIGHTNESS_CONTROL_Active = false;
boolean LED_Active    = false;
boolean IR_Active     = false;

//Sun API Data
boolean API_SunBelowHorizon = false;

//Time API Data
int API_hour    = 12;
int API_minute  = 0;

//Party Time
int PrevRandomColor = 0;
int NewRandomColor = 0;
boolean SwitchFadeParty = false;
boolean WaitNextColor   = false;

//DHT
#ifdef DHT_Function
DHT_Unified dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
#endif

//IR
#ifdef IR_Function
IRrecv IrDetector(IR_DETECTOR_PIN);
decode_results IrDetectorResults;
int IrMessageValue;
#endif

//Motion Detection
boolean MotionDeteced   = false;
boolean MotionActive    = false;
boolean waitTimeOutTime = false;

//LED Control
double ColorFadeSpeed = 0;

//-------------------------------End Variable Deklaration--------------------------------//

//-----------------------------------------Setup-----------------------------------------//
void setup() {
  //Start Serial Communication
  Serial.begin(115200);

  //Start Setup
  Serial.println("Start Setup");

#ifdef IR_Function
  //Start IR Detector
  Serial.println("Initalize IR Reciver");
  IrDetector.enableIRIn();
  Serial.println("Finished Initalize IR Reciver");
#endif

#ifdef DHT_Function
  //Initialize DHT Sensor
  Serial.println("Initialize DHT Sensor");
  dht.begin();
  Serial.println("Finished Initialize DHT Sensor");
#endif

#ifdef PIR_Function
  //Initialize First PIR
  Serial.println("Initialize First PIR Sensor");
  pinMode(PIR_SENSOR_ONE_PIN, INPUT);
  digitalWrite(PIR_SENSOR_ONE_PIN, LOW);
  Serial.println("Finished Initialize First PIR Sensor");

  //Initialize Second PIR
  Serial.println("Initialize Second PIR Sensor");
  pinMode(PIR_SENSOR_TWO_PIN, INPUT);
  digitalWrite(PIR_SENSOR_TWO_PIN, LOW);
  Serial.println("Finished Initialize Second PIR Sensor");
#endif

#ifdef LED_Function
  //Initialize LED Pins
  Serial.println("Initialize LED Pins");
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  Serial.println("Finished Initialize LED Pins");
#endif

  //Set MQTT Server and Callback
  Serial.println("Set MQTT Parameter");
  mqtt_Client.setServer(mqtt_server, mqtt_port);
  mqtt_Client.setCallback(callback);
  Serial.println("Finished setting MQTT Parameter");

  //Connect Wifi
  Serial.println("Start Wifi Connection");
  reconnect_wifi();
  Serial.println("Finished Wifi Connection");

  //Connect to MQTT Server
  Serial.println("Start MQTT Server Connection");
  reconnect_MqttClient();
  Serial.println("Finished MQTT Server Connection");

  //Connect to HTTP Server
  Serial.println("Start HTTP Server Connection");
  //reconnect_HttpClient();
  Serial.println("Finished HTTP Server Connection");

  //Finished Setup
  Serial.println("Finished Setup");

  Serial.println("");
  Serial.println("----------Mqtt Settings---------");
}
//---------------------------------------End Setup---------------------------------------//

//-----------------------------------------Loop------------------------------------------//
void loop() {
  //-----------------------------Network-----------------------------//
  mqtt_Client.loop();

  //Reconnect MQTT
  if (!mqtt_Client.connected()) {
#ifdef DEBUG
    Serial.println("MQTT Client Disconnected. Attempting reconnection");
#endif
    reconnect_MqttClient();
  }

  //Reconnect HTTP
  /*
    if (!http_Client.connected()) {
    #ifdef DEBUG
    Serial.println("HTTP Client Disconnected. Attempting reconnection");
    #endif
    reconnect_HttpClient();
    }
  */

  //Reconnect WIFI
  if (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUG
    Serial.print("WIFI Disconnected. Attempting reconnection");
#endif
    reconnect_wifi();
  }

  //---------------------------Main Control---------------------------//

  //Update API Data
  if (API_Active) {
    unsigned long CurMillis_ApiUpdate = millis();
    if (CurMillis_ApiUpdate - PrevMillis_ApiUpdate >= TimeOut_ApiUpdate) {
      PrevMillis_ApiUpdate = CurMillis_ApiUpdate;
      UpdateSunData();
      UpdateTimeData();
    }
  }

  //If Party Time Active Ignore All
  if (PARTY_Active == true) {
    PartyTime();
  } else {

    //Motion Control
    if (MOTION_Active == true) {
      //MotionDetection();
    }

    //Brightness Control
    if (BRIGHTNESS_CONTROL_Active == true) {
      //MotionBrightnessTime();
    }

#ifdef IR_Function
    //IR Control
    if (IR_Active == true) {
      checkIrMessage();
      IrMessageUse();
    }
#endif

    //LED Control
    LedController();
  }

#ifdef DHT_Function
  //DHT
  unsigned long CurMillis_CheckDHT = millis();
  if (CurMillis_CheckDHT - PrevMillis_CheckDHT >= TimeOut_CheckDHT) {
    PrevMillis_CheckDHT = CurMillis_CheckDHT;
    checkDHT();
  }
#endif

}
//---------------------------------------End Loop----------------------------------------//

//--------------------------------------Party Time---------------------------------------//
void PartyTime() {
  unsigned long CurMillis_PartyTime = millis();
  if (CurMillis_PartyTime - PrevMillis_PartyTime >= TimeOut_PartyTime) {
    PrevMillis_PartyTime = CurMillis_PartyTime;
    while (true) {
      NewRandomColor = int(random(1, 9));
      if (NewRandomColor != PrevRandomColor) {
        PrevRandomColor = NewRandomColor;
        break;
      }
    }
    switch (PrevRandomColor)
    {
      case 1:
        PARTY_Color[0] = 255;
        PARTY_Color[1] = 0;
        PARTY_Color[2] = 0;
        break;
      case 2:
        PARTY_Color[0] = 255;
        PARTY_Color[1] = 127;
        PARTY_Color[2] = 0;
        break;
      case 3:
        PARTY_Color[0] = 255;
        PARTY_Color[1] = 255;
        PARTY_Color[2] = 0;
        break;
      case 4:
        PARTY_Color[0] = 127;
        PARTY_Color[1] = 255;
        PARTY_Color[2] = 0;
        break;
      case 5:
        PARTY_Color[0] = 0;
        PARTY_Color[1] = 255;
        PARTY_Color[2] = 0;
        break;
      case 6:
        PARTY_Color[0] = 0;
        PARTY_Color[1] = 255;
        PARTY_Color[2] = 127;
        break;
      case 7:
        PARTY_Color[0] = 0;
        PARTY_Color[1] = 255;
        PARTY_Color[2] = 255;
        break;
      case 8:
        PARTY_Color[0] = 0;
        PARTY_Color[1] = 127;
        PARTY_Color[2] = 255;
        break;
      case 9:
        PARTY_Color[0] = 0;
        PARTY_Color[1] = 0;
        PARTY_Color[2] = 255;
        break;
    }
    WaitNextColor = false;
  }

  unsigned long CurMillis_PartyTimeFade = millis();
  if (CurMillis_PartyTimeFade - PrevMillis_PartyTimeFade >= TimeOut_PartyTimeFade && WaitNextColor == false) {
    PrevMillis_PartyTimeFade = CurMillis_PartyTimeFade;
    if (SwitchFadeParty == false) {
      PARTY_Brightness += 10;
      if (PARTY_Brightness == 100) {
        SwitchFadeParty = true;
      }
    } else {
      PARTY_Brightness -= 10;
      if (PARTY_Brightness == 0) {
        SwitchFadeParty = false;
        WaitNextColor = true;
      }
    }
  }

  //RED PartyTime
  analogWrite(LED_RED_PIN, ((PARTY_Color[0] * 4) * PARTY_Brightness) / 100);
  //GREEN PartyTime
  analogWrite(LED_GREEN_PIN, ((PARTY_Color[1] * 4) * PARTY_Brightness) / 100);
  //BLUE PartyTime
  analogWrite(LED_BLUE_PIN, ((PARTY_Color[2] * 4) * PARTY_Brightness) / 100);
}
//------------------------------------End Party Time-------------------------------------//

//-------------------------------------Led Control---------------------------------------//
void LedController() {

  //Check if ON
  if (LED_Active == true) {
    NEW_Brightness = LED_Brightness;

    NEW_Color[0] = LED_Color[0];
    NEW_Color[1] = LED_Color[1];
    NEW_Color[2] = LED_Color[2];
  } else {
    if (MotionActive == true) {
      NEW_Brightness = MOTION_Brightness;

      NEW_Color[0] = MOTION_Color[0];
      NEW_Color[1] = MOTION_Color[1];
      NEW_Color[2] = MOTION_Color[2];

      OLD_Color[0] = MOTION_Color[0];
      OLD_Color[1] = MOTION_Color[1];
      OLD_Color[2] = MOTION_Color[2];

    } else {
      NEW_Brightness = 0;

      if (OLD_Brightness == 0) {
        NEW_Color[0] = LED_Color[0];
        NEW_Color[1] = LED_Color[1];
        NEW_Color[2] = LED_Color[2];

        OLD_Color[0] = LED_Color[0];
        OLD_Color[1] = LED_Color[1];
        OLD_Color[2] = LED_Color[2];
      }
    }
  }

  unsigned long CurMillis_Fade = millis();
  if (CurMillis_Fade - PrevMillis_Fade >= TimeOut_Fade) {
    PrevMillis_Fade = CurMillis_Fade;

    //Fade Brightness
    if (OLD_Brightness < NEW_Brightness) {
      if (OLD_Brightness + LED_FadeSpeed < NEW_Brightness) {
        OLD_Brightness += LED_FadeSpeed;
      } else {
        OLD_Brightness = NEW_Brightness;
      }
    } else if (OLD_Brightness > NEW_Brightness) {
      if (OLD_Brightness - LED_FadeSpeed > NEW_Brightness) {
        OLD_Brightness -= LED_FadeSpeed;
      } else {
        OLD_Brightness = NEW_Brightness;
      }
    } else if (OLD_Brightness == NEW_Brightness) {
      OLD_Brightness = NEW_Brightness;
    }

    ColorFadeSpeed = (1 + ((double)LED_FadeSpeed * ((double)NEW_Brightness / 100.0)));
    int NEW_FadeSpeed = (int)(ColorFadeSpeed);

    //Fade RED
    if (OLD_Color[0] < NEW_Color[0]) {
      if (OLD_Color[0] + NEW_FadeSpeed < NEW_Color[0]) {
        OLD_Color[0] += NEW_FadeSpeed;
      } else {
        OLD_Color[0] = NEW_Color[0];
      }
    } else if (OLD_Color[0] > NEW_Color[0]) {
      if (OLD_Color[0] - NEW_FadeSpeed > NEW_Color[0]) {
        OLD_Color[0] -= NEW_FadeSpeed;
      } else {
        OLD_Color[0] = NEW_Color[0];
      }
    } else if (OLD_Color[0] == NEW_Color[0]) {
      OLD_Color[0] = NEW_Color[0];
    }

    //Fade GREEN
    if (OLD_Color[1] < NEW_Color[1]) {
      if (OLD_Color[1] + NEW_FadeSpeed < NEW_Color[1]) {
        OLD_Color[1] += NEW_FadeSpeed;
      } else {
        OLD_Color[1] = NEW_Color[1];
      }
    } else if (OLD_Color[1] > NEW_Color[1]) {
      if (OLD_Color[1] - NEW_FadeSpeed > NEW_Color[1]) {
        OLD_Color[1] -= NEW_FadeSpeed;
      } else {
        OLD_Color[1] = NEW_Color[1];
      }
    } else if (OLD_Color[1] == NEW_Color[1]) {
      OLD_Color[1] = NEW_Color[1];
    }

    //Fade BLUE
    if (OLD_Color[2]  < NEW_Color[2]) {
      if (OLD_Color[2] + NEW_FadeSpeed < NEW_Color[2]) {
        OLD_Color[2] += NEW_FadeSpeed;
      } else {
        OLD_Color[2] = NEW_Color[2];
      }
    } else if (OLD_Color[2] > NEW_Color[2]) {
      if (OLD_Color[2] - NEW_FadeSpeed > NEW_Color[2]) {
        OLD_Color[2] -= NEW_FadeSpeed;
      } else {
        OLD_Color[2] = NEW_Color[2];
      }
    } else if (OLD_Color[2] == NEW_Color[2]) {
      OLD_Color[2] = NEW_Color[2];
    }

    //Debug
#ifdef DEBUG_LED
    Serial.print(MotionActive);
    Serial.print(" | ");
    Serial.print(LED_Active);
    Serial.print(" | ");
    Serial.print(((OLD_Color[0] * 4)  * OLD_Brightness) / 100);
    Serial.print(" | ");
    Serial.print(((OLD_Color[1] * 4)  * OLD_Brightness) / 100);
    Serial.print(" | ");
    Serial.print(((OLD_Color[2] * 4)  * OLD_Brightness) / 100);
    Serial.print(" | ");
    Serial.print(LED_Brightness);
    Serial.print(" | ");
    Serial.println(OLD_Brightness);
#endif
  }



  //RED
  analogWrite(LED_RED_PIN, ((OLD_Color[0] * 4)  * OLD_Brightness) / 100);
  //GREEN
  analogWrite(LED_GREEN_PIN, ((OLD_Color[1] * 4) * OLD_Brightness) / 100);
  //BLUE
  analogWrite(LED_BLUE_PIN, ((OLD_Color[2] * 4) * OLD_Brightness) / 100);

}
//-----------------------------------End Led Control-------------------------------------//

//-----------------------------------Brightness Time-------------------------------------//
void MotionBrightnessTime() {
  switch (API_hour) {
    case 18: MOTION_Brightness = 60;
      break;
    case 19: MOTION_Brightness = 60;
      break;
    case 20: MOTION_Brightness = 60;
      break;
    case 21: MOTION_Brightness = 60;
      break;
    case 22: MOTION_Brightness = 60;
      break;
    case 23: MOTION_Brightness = 50;
      break;
    case 24: MOTION_Brightness = 40;
      break;
    case  0: MOTION_Brightness = 30;
      break;
    case  1: MOTION_Brightness = 20;
      break;
    case  2: MOTION_Brightness = 20;
      break;
    case  3: MOTION_Brightness = 20;
      break;
    case  4: MOTION_Brightness = 20;
      break;
    case  5: MOTION_Brightness = 20;
      break;
    case  6: MOTION_Brightness = 30;
      break;
    case  7: MOTION_Brightness = 40;
      break;
    case  8: MOTION_Brightness = 50;
      break;
    case  9: MOTION_Brightness = 60;
      break;
    case 10: MOTION_Brightness = 60;
      break;
    case 11: MOTION_Brightness = 60;
      break;
    case 12: MOTION_Brightness = 60;
      break;
    case 13: MOTION_Brightness = 60;
      break;
    case 14: MOTION_Brightness = 60;
      break;
    case 15: MOTION_Brightness = 60;
      break;
    case 16: MOTION_Brightness = 60;
      break;
    case 17: MOTION_Brightness = 60;
      break;
    default: MOTION_Brightness = 60;
      break;
  }
}
//---------------------------------End Brightness Time-----------------------------------//

//------------------------------------Get-Time-Data--------------------------------------//
void UpdateTimeData() {
  String timeData;
  String lastRead;
  String h;
  String m;

  http_Client.println("GET http://192.168.99.23:8123/api/states/sensor.time?api_password=3125612352"); //address define**********************************
  http_Client.println();
  for (int i = 0; i < 3000; i++) {
    if (lastRead == "state") {
      timeData = http_Client.readStringUntil('"');
      h = http_Client.readStringUntil(':');    //Time Hour
      m = http_Client.readStringUntil('"');    //Time Minutes
      API_hour = h.toInt();
      API_minute = m.toInt();
      break;
    } else {
      timeData = http_Client.readStringUntil('"');
    }
    if (timeData == NULL) {
      break;
    }
    lastRead = timeData;
  }
}
//----------------------------------End Get-Time-Data------------------------------------//

//-------------------------------------Get-Sun-Data--------------------------------------//
void UpdateSunData() {
  String sunData;

  http_Client.println("GET http://192.168.99.23:8123/api/states/sun.sun?api_password=3125612352");
  http_Client.println();
  for (int i = 0; i < 3000; i++) {
    sunData = http_Client.readStringUntil('"');
    if (sunData == NULL) {
      break;
    }
    if (sunData == "below_horizon") {
      API_SunBelowHorizon = true;
      break;
    }
    if (sunData == "above_horizon") {
      API_SunBelowHorizon = false;
      break;
    }
  }
}
//-----------------------------------End Get-Sun-Data------------------------------------//

//-----------------------------------Motion Detection------------------------------------//
void MotionDetection() {

  unsigned long CurMillis_MotionDeteced = millis();

  //Read Sensor Data
  if (digitalRead(PIR_SENSOR_ONE_PIN) == HIGH || digitalRead(PIR_SENSOR_TWO_PIN) == HIGH) {
    PrevMillis_MotionDeteced = CurMillis_MotionDeteced;
#ifdef DEBUG_MOTION
    if (MotionDeteced == false) {
      Serial.println("Motion Deteced");
    }
#endif
    MotionDeteced = true;
  } else {
#ifdef DEBUG_MOTION
    if (MotionDeteced == true) {
      Serial.println("No Motion Deteced");
    }
#endif
    MotionDeteced = false;
  }

  //Start Fade
  if (MotionDeteced == true && MotionActive == false && API_SunBelowHorizon == true && LED_Active == false) {
    MotionActive = true;
#ifdef DEBUG_MOTION
    Serial.println("Motion Active!!!");
#endif
  }

  //Wait TimeOut
  if ((CurMillis_MotionDeteced - PrevMillis_MotionDeteced >= TimeOut_MotionDeteced ) && MotionDeteced == false && MotionActive == true && waitTimeOutTime == false) {
    if (PrevMillis_MotionDeteced != 0) {
      waitTimeOutTime = true;
    }
  }

  //End Fade
  if (MotionDeteced == false && MotionActive == true && waitTimeOutTime == true) {
    MotionActive = false;
    waitTimeOutTime = false;
#ifdef DEBUG_MOTION
    Serial.println("Motion Not Active");
#endif
  }

}
//---------------------------------End Motion Detection----------------------------------//
