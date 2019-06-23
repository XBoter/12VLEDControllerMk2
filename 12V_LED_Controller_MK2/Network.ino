
//------------------------------------- WiFi Control -------------------------------------//
void wifi() {
  //Start Wifi Connection
  if (StartWifi) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSIDWZ, WPA2WZ);
    StartWifi = false;
  }
  //Delay 1 for WiFi Stack
  delay(1);

  //Check if WiFi is Connected
  if (WiFi.status() == WL_CONNECTED) {
    //Reset
    StartWifi = true;
    NoWiFiCounter = 0;
  }

  //Count how much Seconds the WiFi is not Connected
  unsigned long CurMillis_NoWiFiConnected = millis();
  if (CurMillis_NoWiFiConnected - PrevMillis_NoWiFiConnected >= TimeOut_NoWiFiConnected) {
    PrevMillis_NoWiFiConnected = CurMillis_NoWiFiConnected;
    NoWiFiCounter++;
    Serial.print("No WiFi : ");
    Serial.println(int(NoWiFiCounter));
  }

  //Restart ESP after the Counter hits 10 (10 Seconds long was no WiFi found)
  if (int(NoWiFiCounter) >= 10) {
    ESP.restart();
  }

}

//------------------------------------- API Control -------------------------------------//
void api() {

  //Wait for WiFi Connection
  if (!http_Client.connected() and WiFi.status() == WL_CONNECTED) {
    //Try to Connect to HTTP
    http_Client.connect(http_ip, http_port);
  }

}

//------------------------------------- MQTT Control -------------------------------------//
void mqtt() {

  //Wait for WiFi Connection
  if (!mqtt_Client.connected() and WiFi.status() == WL_CONNECTED) {
    //Try to Connect to MQTT and subscribe to the Channels
    if (mqtt_Client.connect(mqtt_client_name, mqtt_username, mqtt_password)) {
      Serial.println("Start channel subscription");

      //Global
      //-- Master --//
      mqtt_Client.subscribe( mqtt_command_Global_Master_Present );

      //-- Parameter --//
      mqtt_Client.subscribe( mqtt_value_Global_Color_Fadespeed );
      mqtt_Client.subscribe( mqtt_value_Global_Brightness_Fadespeed );
      mqtt_Client.subscribe( mqtt_value_Global_Good_Night_Mode );
      mqtt_Client.subscribe( mqtt_command_Global_Motion_Active );

      //-- Modes --//
      mqtt_Client.subscribe( mqtt_command_Global_Party );
      mqtt_Client.subscribe( mqtt_command_Global_Weekend );
      mqtt_Client.subscribe( mqtt_command_Global_Force );
      mqtt_Client.subscribe( mqtt_command_Global_GoodNight );

      //Controller Specific
      //-- LED --//
      if (LED_STRIP_COUNT >= 1) {
        mqtt_Client.subscribe( mqtt_command_LED_Active );
        mqtt_Client.subscribe( mqtt_command_LED_Color );
        mqtt_Client.subscribe( mqtt_command_LED_Brightness );
      }
      if (LED_STRIP_COUNT >= 2) {
        mqtt_Client.subscribe( mqtt_command_LED_Active_2 );
        mqtt_Client.subscribe( mqtt_command_LED_Color_2 );
        mqtt_Client.subscribe( mqtt_command_LED_Brightness_2 );
      }

      //-- Motion --//
      if (MOTION_SENSORS >= 1) {
        mqtt_Client.subscribe( mqtt_value_Motion_Timeout );
      }

      Serial.println("Finished channel subscription");
    }
  }

}

//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, unsigned int length) {

  //Get Message
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## Global ########################################//

  //------------------- Parameter [mqtt_Global_MasterPresent] -------------------//
  if (String(mqtt_command_Global_Master_Present).equals(topic)) {
    if (strcmp(message, "home") == 0) {
      mqtt_Global_MasterPresent = 1;
    } else {
      mqtt_Global_MasterPresent = 0;
    }
  }

  //------------------- Parameter [mqtt_Global_Color_Fadespeed] -------------------//
  if (String(mqtt_value_Global_Color_Fadespeed).equals(topic)) {
    mqtt_Global_Color_Fadespeed = atoi(message);
  }

  //------------------- Parameter [mqtt_Global_Brightness_Fadespeed] -------------------//
  if (String(mqtt_value_Global_Brightness_Fadespeed).equals(topic)) {
    mqtt_Global_Brightness_Fadespeed = atoi(message);
  }

  //------------------- Parameter [mqtt_Global_Good_Night_Timeout] -------------------//
  if (String(mqtt_value_Global_Good_Night_Mode).equals(topic)) {
    mqtt_Global_Good_Night_Timeout = atoi(message);
  }

  //------------------- Parameter [mqtt_Global_Motion_Active] -------------------//
  if (String(mqtt_command_Global_Motion_Active).equals(topic)) {
    mqtt_Client.publish(mqtt_state_Global_Motion_Active, message);
    mqtt_Global_Motion_Active = atoi(message);
  }

  //------------------- Parameter [mqtt_Global_Party] -------------------//
  if (String(mqtt_command_Global_Party).equals(topic)) {
    mqtt_Global_Party             = atoi(message);
    mqtt_Global_Weekend           = 0;
    mqtt_Global_Force             = 0;
    mqtt_Global_GoodNight         = 0;
    mqtt_Client.publish(mqtt_state_Global_Party, message);
    mqtt_Client.publish(mqtt_state_Global_Weekend, "0");
    mqtt_Client.publish(mqtt_state_Global_Force, "0");
    mqtt_Client.publish(mqtt_state_Global_GoodNight, "0");
  }

  //------------------- Parameter [mqtt_Global_Weekend] -------------------//
  if (String(mqtt_command_Global_Weekend).equals(topic)) {
    mqtt_Global_Party             = 0;
    mqtt_Global_Weekend           = atoi(message);
    mqtt_Global_Force             = 0;
    mqtt_Global_GoodNight         = 0;
    mqtt_Client.publish(mqtt_state_Global_Party, "0");
    mqtt_Client.publish(mqtt_state_Global_Weekend, message);
    mqtt_Client.publish(mqtt_state_Global_Force, "0");
    mqtt_Client.publish(mqtt_state_Global_GoodNight, "0");
  }

  //------------------- Parameter [mqtt_Global_Force] -------------------//
  if (String(mqtt_command_Global_Force).equals(topic)) {
    mqtt_Global_Party             = 0;
    mqtt_Global_Weekend           = 0;
    mqtt_Global_Force             = atoi(message);
    mqtt_Global_GoodNight         = 0;
    mqtt_Client.publish(mqtt_state_Global_Party, "0");
    mqtt_Client.publish(mqtt_state_Global_Weekend, "0");
    mqtt_Client.publish(mqtt_state_Global_Force, message);
    mqtt_Client.publish(mqtt_state_Global_GoodNight, "0");

    //Generate Start Color
    StateForce = (int)(random(0, 2)) * 20;
  }

  //------------------- Parameter [mqtt_Global_GoodNight] -------------------//
  if (String(mqtt_command_Global_GoodNight).equals(topic)) {
    mqtt_Global_Party             = 0;
    mqtt_Global_Weekend           = 0;
    mqtt_Global_Force             = 0;
    mqtt_Global_GoodNight         = atoi(message);
    mqtt_Client.publish(mqtt_state_Global_Party, "0");
    mqtt_Client.publish(mqtt_state_Global_Weekend, "0");
    mqtt_Client.publish(mqtt_state_Global_Force, "0");
    mqtt_Client.publish(mqtt_state_Global_GoodNight, message);

    //Reset GoodNight Mode
    StateGoodNight = 0;
    PrevMillis_EffectGoodNightMode = millis();
  }

  //######################################## Specific ########################################//

  if (LED_STRIP_COUNT >= 1) {
    //------------------- Parameter [mqtt_LED_Active] -------------------//
    if (String(mqtt_command_LED_Active).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Active, message, true);
      mqtt_LED_Active_1 = atoi(message);
    }

    //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
    if (String(mqtt_command_LED_Color).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Color, message, true);
      mqtt_LED_Red_1    = atoi(strtok(message, ","));
      mqtt_LED_Green_1  = atoi(strtok(NULL, ","));
      mqtt_LED_Blue_1   = atoi(strtok(NULL, ","));
    }

    //------------------- Parameter [mqtt_LED_Brightness] -------------------//
    if (String(mqtt_command_LED_Brightness).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Brightness, message, true);
      mqtt_LED_Brightness_1 = atoi(message);
    }
  }

  if (LED_STRIP_COUNT >= 2) {
    //------------------- Parameter [mqtt_LED_Active_2] -------------------//
    if (String(mqtt_command_LED_Active_2).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Active_2, message, true);
      mqtt_LED_Active_2 = atoi(message);
    }

    //------------------- Parameter [mqtt_LED_Red_2,mqtt_LED_Green_2,mqtt_LED_Blue_2] -------------------//
    if (String(mqtt_command_LED_Color_2).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Color_2, message, true);
      mqtt_LED_Red_2    = atoi(strtok(message, ","));
      mqtt_LED_Green_2  = atoi(strtok(NULL, ","));
      mqtt_LED_Blue_2   = atoi(strtok(NULL, ","));
    }

    //------------------- Parameter [mqtt_LED_Brightness_2] -------------------//
    if (String(mqtt_command_LED_Brightness_2).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Brightness_2, message, true);
      mqtt_LED_Brightness_2 = atoi(message);
    }
  }

  //------------------- Parameter [mqtt_Motion_Timout] -------------------//
  if (String(mqtt_value_Motion_Timeout).equals(topic)) {
    mqtt_Motion_Timout = atoi(message);
  }

}


void HeartBeat() {

  //Heartbeat
  unsigned long CurMillis_HeartBeat = millis();
  if (CurMillis_HeartBeat - PrevMillis_HeartBeat >= TimeOut_HeartBeat) {
    PrevMillis_HeartBeat = CurMillis_HeartBeat;
    mqtt_Client.publish(mqtt_Heartbeat, HeartBeatCounter);
    HeartBeatCounter++;
  }

}
