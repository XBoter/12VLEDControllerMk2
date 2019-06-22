
void SetupController() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Name          : ");
  Serial.println(Name);
  Serial.print("Programmer    : ");
  Serial.println(Programmer);
  Serial.print("Created       : ");
  Serial.println(Created);
  Serial.print("Last Modifed  : ");
  Serial.println(LastModifed);
  Serial.print("Version       : ");
  Serial.println(Version);
  Serial.println("");

  Serial.println("Start Setup");

  Serial.println("Set MQTT Parameter");
  mqtt_Client.setServer(mqtt_server, mqtt_port);
  mqtt_Client.setCallback(callback);
  Serial.println("Finished setting MQTT Parameter");

  Serial.println("Start PIN Initialization");
  if (LED_STRIP_COUNT == 2) {
    Serial.println("LED Strip     1: D1,D2,D3");
    pinMode(PIN_LED_STRIP_1_RED, OUTPUT);
    pinMode(PIN_LED_STRIP_1_GREEN, OUTPUT);
    pinMode(PIN_LED_STRIP_1_BLUE, OUTPUT);
    Serial.println("LED Strip     2: D5,D6,D7");
    pinMode(PIN_LED_STRIP_2_RED, OUTPUT);
    pinMode(PIN_LED_STRIP_2_GREEN, OUTPUT);
    pinMode(PIN_LED_STRIP_2_BLUE, OUTPUT);
  } else {
    Serial.println("LED Strip     1: D1,D2,D3");
    pinMode(PIN_LED_STRIP_1_RED, OUTPUT);
    pinMode(PIN_LED_STRIP_1_GREEN, OUTPUT);
    pinMode(PIN_LED_STRIP_1_BLUE, OUTPUT);
    if (MOTION_SENSORS == 2) {
      Serial.println("Motion Sensor 1: D7");
      pinMode(PIN_MOTION_SENSOR_1, INPUT);
      Serial.println("Motion Sensor 2: D6");
      pinMode(PIN_MOTION_SENSOR_2, INPUT);
    } else {
      Serial.println("Motion Sensor 1: D7");
      pinMode(PIN_MOTION_SENSOR_1, INPUT);
    }
  }
  Serial.println("Finished PIN Initialization");

  Serial.println("Finished Setup");
  Serial.println("");
}


void LoopController() {

  //-- WiFi --//
  if (WiFi.status() != WL_CONNECTED) {
    wifi();
  }

  //-- MQTT --//
  if ((WiFi.status() == WL_CONNECTED) and !mqtt_Client.connected()) {
    mqtt();
  }
  mqtt_Client.loop();

  //-- API --//
  if ((WiFi.status() == WL_CONNECTED) and mqtt_Client.connected() and !http_Client.connected()) {
    api();
  }

  //-- HeartBeat --//
  HeartBeat();

  //-- Printer --//
  printer();

  //-- GetApiData --//
  GetApiTimeData();
  GetApiSunData();

  //-- Motion Detection --//
  MotionBrightnessControl();
  MotionDetection();

  //-------------- Error State Controll for Network  -----------------//

  //#### WiFi ####//
  if ((WiFi.status() != WL_CONNECTED) and NoWiFiError) {
    LastMainState = MainState;
    MainState = 777;
    NoWiFiError = false;
  } else {
    if ((WiFi.status() == WL_CONNECTED) and !NoWiFiError) {
      MainState = LastMainState;
      NoWiFiError = true;
    }
  }

  //#### MQTT ####//
  if ((WiFi.status() == WL_CONNECTED) and (!mqtt_Client.connected()) and NoMqttError) {
    LastMainState = MainState;
    MainState = 888;
    NoMqttError = false;
  } else {
    if ((WiFi.status() == WL_CONNECTED) and (mqtt_Client.connected()) and !NoMqttError) {
      MainState = LastMainState;
      NoMqttError = true;
    }
  }

  //#### API ####//
  if ((WiFi.status() == WL_CONNECTED) and (mqtt_Client.connected()) and (!http_Client.connected()) and NoApiError) {
    LastMainState = MainState;
    MainState = 999;
    NoApiError = false;
  } else {
    if ((WiFi.status() == WL_CONNECTED) and (mqtt_Client.connected()) and (http_Client.connected()) and !NoApiError) {
      MainState = LastMainState;
      NoApiError = true;
    }
  }


  //---- Show LED Strip State ----//
  ShowLED();

  //---- Check if Master is Present ----//
  if (mqtt_Global_MasterPresent) {

    //-------------- Main State Machine --------------//
    switch (MainState) {

      //Wait for WiFi and MQTT Connection
      case 0:
        if ((WiFi.status() == WL_CONNECTED) and mqtt_Client.connected()) {
          MainState = 10;
        }
        break;

      //Fade Rest Effects
      case 10:
        MainState = 100;
        break;

      //Run Mode
      case 100:

        //---- Normal Light Mode Strip 1 ----//
        if (!mqtt_Global_Party and !mqtt_Global_Weekend and !mqtt_Global_Force and !mqtt_Global_GoodNight) {

          //---- Strip 1 ----//
          if (mqtt_LED_Active_1) {
            NormalLight(1);
          } else {
            FadeBrightnessTo(1, 0);
          }

          //---- Strip 2 ----//
          if (mqtt_LED_Active_2) {
            NormalLight(2);
          } else {
            FadeBrightnessTo(2, 0);
          }

        }

        //---- Party Light Mode Strip 1 and 2 ----//
        if (mqtt_Global_Party and !mqtt_Global_Weekend and !mqtt_Global_Force and !mqtt_Global_GoodNight) {
          PartyLight();
        }

        //---- Weekend Light Mode Strip 1 and 2 ----//
        if (!mqtt_Global_Party and mqtt_Global_Weekend and !mqtt_Global_Force and !mqtt_Global_GoodNight) {
          WeekendLight();
        }

        //---- Force Light Mode Strip 1 and 2 ----//
        if (!mqtt_Global_Party and !mqtt_Global_Weekend and mqtt_Global_Force and !mqtt_Global_GoodNight) {
          ForceLight();
        }

        //---- Good Night Light Mode Strip 1 and 2 ----//
        if (!mqtt_Global_Party and !mqtt_Global_Weekend and !mqtt_Global_Force and mqtt_Global_GoodNight) {
          GoodNightLight();
        }

        break;

      //No WiFi Connection
      case 777:
        Error_NoWiFiConnection();
        break;

      //No MQTT Broker Connection
      case 888:
        Error_NoMqttConnection();
        break;

      //No API Connection
      case 999:
        Error_NoApiConnection();
        break;

      //General Error
      case 1111:
        Error_GerneralError();
        break;

      //Default Error
      default:
        Error_GerneralError();
        break;

    }

  } else {
    FadeBrightnessTo(1, 0);
    FadeBrightnessTo(2, 0);
  }

}
