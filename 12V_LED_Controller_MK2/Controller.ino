
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

  //-- HeartBeat --//
  HeartBeat();

  //-- Printer --//
  printer();


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

  //-------------- Main State Machine --------------//
  switch (MainState) {

    case 0: ;
      break;

    case 10: ;
      break;

    //Run Mode
    case 100:

      switch (LightState) {

        case 0: ;
          break;

        case 10: ;
          break;

      }

      break;

    //No WiFi
    case 777: Error_NoWiFiConnection();
      break;

    //No MQTT Broker
    case 888: Error_NoMqttConnection();
      break;

    //General Error
    case 999: Error_GerneralError();
      break;
  }

}
