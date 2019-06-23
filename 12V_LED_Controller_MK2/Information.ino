
//Prints Information about the Programm to the Serial Terminal
void printer() {

#ifdef DEBUG_MAIN_STATE

  if (Information_MainState != MainState) {
    Serial.println("/------- Main State Change --------/");
    Serial.print("   Main State        : ");
    Serial.println(MainState);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_MainState = MainState;
  }

#endif


#ifdef DEBUG_LIGHT_STATE

  if (Information_LightState != LightState) {
    Serial.println("/------ Light State Change --------/");
    Serial.print("   Light State       : ");
    Serial.println(LightState);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_LightState = LightState;
  }

#endif


#ifdef DEBUG_API_DATA

  if ((Information_api_TimeHour != api_TimeHour) or (Information_api_TimeMinute != api_TimeMinute)) {
    Serial.println("/-------- API Data Change ---------/");
    Serial.print("   Time              : ");
    Serial.print(api_TimeHour);
    Serial.print(":");
    Serial.println(api_TimeMinute);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_api_TimeHour = api_TimeHour;
    Information_api_TimeMinute = api_TimeMinute;
  }

  if (Information_api_SunDown != api_SunDown) {
    Serial.println("/-------- API Data Change ---------/");
    Serial.print("   Sun Down          : ");
    Serial.println(api_SunDown);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_api_SunDown = api_SunDown;
  }

#endif


#ifdef DEBUG_MQTT_PARAMETER

  //------------------- Parameter [mqtt_Global_MasterPresent] -------------------//
  if (mqtt_Global_MasterPresent != Information_mqtt_Global_MasterPresent) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Master Present       : ");
    Serial.println(mqtt_Global_MasterPresent);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_MasterPresent = mqtt_Global_MasterPresent;
  }

  //------------------- Parameter [mqtt_Global_Color_Fadespeed] -------------------//
  if (mqtt_Global_Color_Fadespeed != Information_mqtt_Global_Color_Fadespeed) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Fadespeed Color      : ");
    Serial.println(mqtt_Global_Color_Fadespeed);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Color_Fadespeed = mqtt_Global_Color_Fadespeed;
  }

  //------------------- Parameter [mqtt_Global_Brightness_Fadespeed] -------------------//
  if (mqtt_Global_Brightness_Fadespeed != Information_mqtt_Global_Brightness_Fadespeed) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Fadespeed Brightness : ");
    Serial.println(mqtt_Global_Brightness_Fadespeed);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Brightness_Fadespeed = mqtt_Global_Brightness_Fadespeed;
  }

  //------------------- Parameter [mqtt_Global_Good_Night_Timeout] -------------------//
  if (mqtt_Global_Good_Night_Timeout != Information_mqtt_Global_Good_Night_Timeout) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Good Night Timeout   : ");
    Serial.println(mqtt_Global_Good_Night_Timeout);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Good_Night_Timeout = mqtt_Global_Good_Night_Timeout;
  }

  //------------------- Parameter [mqtt_Global_Party] -------------------//
  if (mqtt_Global_Party != Information_mqtt_Global_Party) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Party                : ");
    Serial.println(mqtt_Global_Party);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Party = mqtt_Global_Party;
  }

  //------------------- Parameter [mqtt_Global_Weekend] -------------------//
  if (mqtt_Global_Weekend != Information_mqtt_Global_Weekend) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Weekend              : ");
    Serial.println(mqtt_Global_Weekend);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Weekend = mqtt_Global_Weekend;
  }

  //------------------- Parameter [mqtt_Global_Force] -------------------//
  if (mqtt_Global_Force != Information_mqtt_Global_Force) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Force                : ");
    Serial.println(mqtt_Global_Force);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Force = mqtt_Global_Force;
  }

  //------------------- Parameter [mqtt_Global_GoodNight] -------------------//
  if (mqtt_Global_GoodNight != Information_mqtt_Global_GoodNight) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Good Night           : ");
    Serial.println(mqtt_Global_GoodNight);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_GoodNight = mqtt_Global_GoodNight;
  }

  //------------------- Parameter [mqtt_LED_Active] -------------------//
  if (mqtt_LED_Active_1 != Information_mqtt_LED_Active_1) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 1 Active         : ");
    Serial.println(mqtt_LED_Active_1);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Active_1 = mqtt_LED_Active_1;
  }

  //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
  if ((Information_mqtt_LED_Red_1 != mqtt_LED_Red_1) or (Information_mqtt_LED_Green_1 != mqtt_LED_Green_1) or (Information_mqtt_LED_Blue_1 != mqtt_LED_Blue_1)) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 1 Red            : ");
    Serial.println(mqtt_LED_Red_1);
    Serial.print("   LED 1 Green          : ");
    Serial.println(mqtt_LED_Green_1);
    Serial.print("   LED 1 Blue           : ");
    Serial.println(mqtt_LED_Blue_1);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Red_1    = mqtt_LED_Red_1;
    Information_mqtt_LED_Green_1  = mqtt_LED_Green_1;
    Information_mqtt_LED_Blue_1   = mqtt_LED_Blue_1;
  }

  //------------------- Parameter [mqtt_LED_Brightness] -------------------//
  if (mqtt_LED_Brightness_1 != Information_mqtt_LED_Brightness_1) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 1 Brightness     : ");
    Serial.println(mqtt_LED_Brightness_1);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Brightness_1 = mqtt_LED_Brightness_1;
  }

  //------------------- Parameter [mqtt_LED_Active_2] -------------------//
  if (mqtt_LED_Active_2 != Information_mqtt_LED_Active_2) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 2 Active         : ");
    Serial.println(mqtt_LED_Active_2);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Active_2 = mqtt_LED_Active_2;
  }

  //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
  if ((Information_mqtt_LED_Red_2 != mqtt_LED_Red_2) or (Information_mqtt_LED_Green_2 != mqtt_LED_Green_2) or (Information_mqtt_LED_Blue_2 != mqtt_LED_Blue_2)) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 2 Red            : ");
    Serial.println(mqtt_LED_Red_2);
    Serial.print("   LED 2 Green          : ");
    Serial.println(mqtt_LED_Green_2);
    Serial.print("   LED 2 Blue           : ");
    Serial.println(mqtt_LED_Blue_2);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Red_2    = mqtt_LED_Red_2;
    Information_mqtt_LED_Green_2  = mqtt_LED_Green_2;
    Information_mqtt_LED_Blue_2   = mqtt_LED_Blue_2;
  }

  //------------------- Parameter [mqtt_LED_Brightness_2] -------------------//
  if (mqtt_LED_Brightness_2 != Information_mqtt_LED_Brightness_2) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   LED 2 Brightness     : ");
    Serial.println(mqtt_LED_Brightness_2);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_LED_Brightness_2 = mqtt_LED_Brightness_2;
  }

  //------------------- Parameter [mqtt_Motion_Active] -------------------//
  if (mqtt_Global_Motion_Active != Information_mqtt_Global_Motion_Active) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Motion Active        : ");
    Serial.println(mqtt_Global_Motion_Active);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Global_Motion_Active = mqtt_Global_Motion_Active;
  }

  //------------------- Parameter [mqtt_Motion_Timout] -------------------//
  if (mqtt_Motion_Timout != Information_mqtt_Motion_Timout) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   Motion Timeout       : ");
    Serial.println(mqtt_Motion_Timout);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_Motion_Timout = mqtt_Motion_Timout;
  }

#endif


#ifdef DEBUG_MOTION

  if (MOTION_SENSORS >= 1 and LED_STRIP_COUNT == 1) {
    //------------------- PIR Sensor -------------------//
    if (PirSensor1MotionDetected != Information_PirSensor1MotionDetected) {
      Serial.println("/------ PIR Motion Detected -------/");
      Serial.print("   Motion Sensor 1      : ");
      Serial.println(PirSensor1MotionDetected);
      Serial.println("/----------------------------------/");
      Serial.println("");
      Information_PirSensor1MotionDetected = PirSensor1MotionDetected;
    }
  }

  if (MOTION_SENSORS == 2 and LED_STRIP_COUNT == 1) {
    //------------------- PIR Sensor -------------------//
    if (PirSensor2MotionDetected != Information_PirSensor2MotionDetected) {
      Serial.println("/------ PIR Motion Detected -------/");
      Serial.print("   Motion Sensor 2      : ");
      Serial.println(PirSensor2MotionDetected);
      Serial.println("/----------------------------------/");
      Serial.println("");
      Information_PirSensor2MotionDetected = PirSensor2MotionDetected;
    }
  }

#endif


#ifdef DEBUG_IR

  if (IR_RECIVER == 1 and LED_STRIP_COUNT == 1) {
    //------------------- IR Sensor -------------------//
    if (Information_IR_DataReceived) {
      Serial.println("/------ IR Message Received -------/");
      Serial.print("   Message              : ");
      serialPrintUint64(IrRecvResult.value, HEX);
      Serial.println("");
      Serial.println("/----------------------------------/");
      Serial.println("");
    }
  }

#endif


#ifdef DEBUG_DHT

  if (DHT_SENSOR == 1 and LED_STRIP_COUNT == 1) {
    //------------------- DHT Sensor -------------------//
    if (Information_DHT_DataRead) {
      Serial.println("/---------- DHT New Data ----------/");
      Serial.print("   Temperature          : ");
      Serial.print(Temperature);
      Serial.println(" ºC");
      Serial.print("   Humidity             : ");
      Serial.print(Humidity);
      Serial.println(" %");
      Serial.println("/----------------------------------/");
      Serial.println("");
    }
  }

#endif


}
