
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

//------------------------------------- MQTT Control -------------------------------------//
void mqtt() {

  //Wait for WiFi Connection
  if (!mqtt_Client.connected() and WiFi.status() == WL_CONNECTED) {
    //Try to Connect to MQTT and subscribe to the Channels
    if (mqtt_Client.connect(mqtt_client_name, mqtt_username, mqtt_password)) {
      Serial.println("Start channel subscription");

      //Global
      //-- Parameter --//
      mqtt_Client.subscribe( mqtt_value_Global_Fadespeed );
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

      //-- IR --//
      if (IR_RECIVER >= 1) {
        mqtt_Client.subscribe( mqtt_command_IR_Active );
      }

      //-- API --//
      if (MOTION_SENSORS >= 1) {
        mqtt_Client.subscribe( mqtt_command_API_Active );
      }

      //-- Motion --//
      if (MOTION_SENSORS >= 1) {
        mqtt_Client.subscribe( mqtt_command_Motion_Active );
        mqtt_Client.subscribe( mqtt_command_Motion_Second );
        mqtt_Client.subscribe( mqtt_value_Motion_Timeout );
      }

      Serial.println("Finished channel subscription");
    }
  }

}

//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, unsigned int length) {

  //For Information Tab
  Network_NewData = true;

  //Get Message
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## Global ########################################//

  //------------------- Parameter [mqtt_Global_Fadespeed] -------------------//
  if (String(mqtt_value_Global_Fadespeed).equals(topic)) {
    mqtt_Global_Fadespeed = atoi(message);
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
  }

  //######################################## Specific ########################################//

  if (LED_STRIP_COUNT >= 1) {
    //------------------- Parameter [mqtt_LED_Active] -------------------//
    if (String(mqtt_command_LED_Active).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Active, message, true);
      mqtt_LED_Active = atoi(message);
    }

    //------------------- Parameter [mqtt_LED_Red,mqtt_LED_Green,mqtt_LED_Blue] -------------------//
    if (String(mqtt_command_LED_Color).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Color, message, true);
      mqtt_LED_Red    = atoi(strtok(message, ","));
      mqtt_LED_Green  = atoi(strtok(NULL, ","));
      mqtt_LED_Blue   = atoi(strtok(NULL, ","));
    }

    //------------------- Parameter [mqtt_LED_Brightness] -------------------//
    if (String(mqtt_command_LED_Brightness).equals(topic)) {
      mqtt_Client.publish(mqtt_state_LED_Brightness, message, true);
      mqtt_LED_Brightness = atoi(message);
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

  if (IR_RECIVER >= 1) {
    //------------------- Parameter [mqtt_IR_Active] -------------------//
    if (String(mqtt_command_IR_Active).equals(topic)) {
      mqtt_Client.publish(mqtt_state_IR_Active, message, true);
      mqtt_IR_Active = atoi(message);
    }
  }

  if (MOTION_SENSORS >= 1) {
    //------------------- Parameter [mqtt_API_Active] -------------------//
    if (String(mqtt_command_API_Active).equals(topic)) {
      mqtt_Client.publish(mqtt_state_API_Active, message, true);
      mqtt_IR_Active = atoi(message);
    }

    //------------------- Parameter [mqtt_Motion_Active] -------------------//
    if (String(mqtt_command_Motion_Active).equals(topic)) {
      mqtt_Client.publish(mqtt_state_Motion_Active, message, true);
      mqtt_Motion_Active = atoi(message);
    }

    //------------------- Parameter [mqtt_Motion_Second] -------------------//
    if (String(mqtt_command_Motion_Second).equals(topic)) {
      mqtt_Client.publish(mqtt_state_Motion_Second, message, true);
      mqtt_Motion_Second = atoi(message);
    }

    //------------------- Parameter [mqtt_Motion_Timout] -------------------//
    if (String(mqtt_value_Motion_Timeout).equals(topic)) {
      mqtt_Motion_Timout = atoi(message);
    }
  }

}


void HeartBeat() {

}
