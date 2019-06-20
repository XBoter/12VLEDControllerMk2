
//Prints Information about the Programm to the Serial Terminal
void printer() {

#ifdef DEBUG_MAIN_STATE

  if (Information_MainState != MainState) {
    Serial.println("/----------- Main State -----------/");
    Serial.print("   Main State        : ");
    Serial.println(MainState);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_MainState = MainState;
  }

#endif


#ifdef DEBUG_LIGHT_STATE

  if (Information_LightState != LightState) {
    Serial.println("/---------- Light State -----------/");
    Serial.print("   Light State       : ");
    Serial.println(LightState);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_LightState = LightState;
  }

#endif


#ifdef DEBUG_MQTT_PARAMETER

  if (Network_NewData) {
    Serial.println("/----- MQTT Parameter Change ------/");
  }

  //------------------- Parameter [mqtt_Global_Fadespeed] -------------------//
  if (mqtt_Global_Fadespeed != Information_mqtt_Global_Fadespeed) {
    Serial.print("   Fadespeed         : ");
    Serial.println(mqtt_Global_Fadespeed);
    Information_mqtt_Global_Fadespeed = mqtt_Global_Fadespeed;
  }

  //------------------- Parameter [mqtt_Global_Party] -------------------//
  if (mqtt_Global_Party != Information_mqtt_Global_Party) {
    Serial.print("   Party             : ");
    Serial.println(mqtt_Global_Party);
    Information_mqtt_Global_Party = mqtt_Global_Party;
  }

  //------------------- Parameter [mqtt_Global_Weekend] -------------------//
  if (mqtt_Global_Weekend != Information_mqtt_Global_Weekend) {
    Serial.print("   Weekend           : ");
    Serial.println(mqtt_Global_Weekend);
    Information_mqtt_Global_Weekend = mqtt_Global_Weekend;
  }

  //------------------- Parameter [mqtt_Global_Force] -------------------//
  if (mqtt_Global_Force != Information_mqtt_Global_Force) {
    Serial.print("   Force             : ");
    Serial.println(mqtt_Global_Force);
    Information_mqtt_Global_Force = mqtt_Global_Force;
  }

  //------------------- Parameter [mqtt_Global_GoodNight] -------------------//
  if (mqtt_Global_GoodNight != Information_mqtt_Global_GoodNight) {
    Serial.print("   Good Night        : ");
    Serial.println(mqtt_Global_GoodNight);
    Information_mqtt_Global_GoodNight = mqtt_Global_GoodNight;
  }

  //------------------- Parameter [mqtt_LED_Active] -------------------//
  if (mqtt_LED_Active != Information_mqtt_LED_Active) {
    Serial.print("   LED 1 Active      : ");
    Serial.println(mqtt_LED_Active);
    Information_mqtt_LED_Active = mqtt_LED_Active;
  }

  //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
  if ((Information_mqtt_LED_Red != mqtt_LED_Red) or (Information_mqtt_LED_Green != mqtt_LED_Green) or (Information_mqtt_LED_Blue != mqtt_LED_Blue)) {
    Serial.print("   LED 1 Red         : ");
    Serial.println(mqtt_LED_Red);
    Serial.print("   LED 1 Green       : ");
    Serial.println(mqtt_LED_Green);
    Serial.print("   LED 1 Blue        : ");
    Serial.println(mqtt_LED_Blue);
    Information_mqtt_LED_Red    = mqtt_LED_Red;
    Information_mqtt_LED_Green  = mqtt_LED_Green;
    Information_mqtt_LED_Blue   = mqtt_LED_Blue;
  }

  //------------------- Parameter [mqtt_LED_Brightness] -------------------//
  if (mqtt_LED_Brightness != Information_mqtt_LED_Brightness) {
    Serial.print("   LED 1 Brightness  : ");
    Serial.println(mqtt_LED_Brightness);
    Information_mqtt_LED_Brightness = mqtt_LED_Brightness;
  }

  //------------------- Parameter [mqtt_LED_Active_2] -------------------//
  if (mqtt_LED_Active_2 != Information_mqtt_LED_Active_2) {
    Serial.print("   LED 2 Active      : ");
    Serial.println(mqtt_LED_Active_2);
    Information_mqtt_LED_Active_2 = mqtt_LED_Active_2;
  }

  //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
  if ((Information_mqtt_LED_Red_2 != mqtt_LED_Red_2) or (Information_mqtt_LED_Green_2 != mqtt_LED_Green_2) or (Information_mqtt_LED_Blue_2 != mqtt_LED_Blue_2)) {
    Serial.print("   LED 2 Red         : ");
    Serial.println(mqtt_LED_Red_2);
    Serial.print("   LED 2 Green       : ");
    Serial.println(mqtt_LED_Green_2);
    Serial.print("   LED 2 Blue        : ");
    Serial.println(mqtt_LED_Blue_2);
    Information_mqtt_LED_Red_2    = mqtt_LED_Red_2;
    Information_mqtt_LED_Green_2  = mqtt_LED_Green_2;
    Information_mqtt_LED_Blue_2   = mqtt_LED_Blue_2;
  }

  //------------------- Parameter [mqtt_LED_Brightness_2] -------------------//
  if (mqtt_LED_Brightness_2 != Information_mqtt_LED_Brightness_2) {
    Serial.print("   LED 2 Brightness  : ");
    Serial.println(mqtt_LED_Brightness_2);
    Information_mqtt_LED_Brightness_2 = mqtt_LED_Brightness_2;
  }

  //------------------- Parameter [mqtt_IR_Active] -------------------//
  if (mqtt_IR_Active != Information_mqtt_IR_Active) {
    Serial.print("   IR Active         : ");
    Serial.println(mqtt_IR_Active);
    Information_mqtt_IR_Active = mqtt_IR_Active;
  }

  //------------------- Parameter [mqtt_API_Active] -------------------//
  if (mqtt_API_Active != Information_mqtt_API_Active) {
    Serial.print("   API Active        : ");
    Serial.println(mqtt_API_Active);
    Information_mqtt_API_Active = mqtt_API_Active;
  }

  //------------------- Parameter [mqtt_Motion_Active] -------------------//
  if (mqtt_Motion_Active != Information_mqtt_Motion_Active) {
    Serial.print("   Motion 1 Active   : ");
    Serial.println(mqtt_Motion_Active);
    Information_mqtt_Motion_Active = mqtt_Motion_Active;
  }

  //------------------- Parameter [mqtt_Motion_Second] -------------------//
  if (mqtt_Motion_Second != Information_mqtt_Motion_Second) {
    Serial.print("   Motion 2 Active   : ");
    Serial.println(mqtt_Motion_Second);
    Information_mqtt_Motion_Second = mqtt_Motion_Second;
  }

  //------------------- Parameter [mqtt_Motion_Timout] -------------------//
  if (mqtt_Motion_Timout != Information_mqtt_Motion_Timout) {
    Serial.print("   Motion Timeout    : ");
    Serial.println(mqtt_Motion_Timout);
    Information_mqtt_Motion_Timout = mqtt_Motion_Timout;
  }

  if (Network_NewData) {
    Serial.println("/----------------------------------/");
    Serial.println("");
    Network_NewData = false;
  }

#endif

}
