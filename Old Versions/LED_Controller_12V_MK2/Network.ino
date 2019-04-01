//-------------------------------------Reconnect Wifi------------------------------------//
void reconnect_wifi() {
  delay(50);
  //Start Wifi Connection
#ifdef DEBUG_NETWORK
  Serial.println();
  Serial.print("Connecting to :");
  Serial.println(wifi_ssid);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG_NETWORK
    Serial.print(".");
#endif
  }
  //Finished Wifi Connection
#ifdef DEBUG_NETWORK
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}
//----------------------------------End Reconnect Wifi-----------------------------------//

//---------------------------------Reconnect HttpClient----------------------------------//
void reconnect_HttpClient() {
  delay(50);
  //Start HTTP Connection
  while (!http_Client.connected()) {
    //Serial.print("Attempting HTTP connection...");
    // Attempt to connect
    if (http_Client.connect(http_ip, http_port)) {
      //Serial.println("connected");
    } else {
#ifdef DEBUG_NETWORK
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//-------------------------------End Reconnect HttpClient--------------------------------//

//---------------------------------Reconnect MqttClient----------------------------------//
void reconnect_MqttClient() {
  delay(50);
  //Start MQTT Connection
  while (!mqtt_Client.connected()) {
#ifdef DEBUG_NETWORK
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    if (mqtt_Client.connect(mqtt_client_name, mqtt_username, mqtt_password)) {
#ifdef DEBUG_NETWORK
      Serial.println("connected");
      Serial.println("Start MQTT Channel Subscription");
#endif
      //LED
      mqtt_Client.subscribe( mqtt_LED_setcolor);
      mqtt_Client.subscribe( mqtt_LED_setbrightness);
      mqtt_Client.subscribe( mqtt_LED_setstatus);
      //mqtt_Client.subscribe( mqtt_LED_FadeSpeed);

      //IR
      //mqtt_Client.subscribe( mqtt_IR_Active);

      //DHT
      //mqtt_Client.subscribe( mqtt_DHT_temperature);
      //mqtt_Client.subscribe( mqtt_DHT_humidity);

      //MOTION
      //mqtt_Client.subscribe( mqtt_MOTION_Color);
      //mqtt_Client.subscribe( mqtt_MOTION_TimeOut);
      //mqtt_Client.subscribe( mqtt_MOTION_Detection_Active);
      //mqtt_Client.subscribe( mqtt_MOTION_Brightness_Control_Active);

      //PARTY
      mqtt_Client.subscribe( mqtt_PARTY_Active);

      //API
      mqtt_Client.subscribe( mqtt_API_Update_Allowed);

#ifdef DEBUG_NETWORK
      Serial.println("Finished MQTT Channel Subscription");
#endif
    } else {
#ifdef DEBUG_NETWORK
      Serial.print("failed, rc=");
      Serial.print(mqtt_Client.state());
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  //Finished MQTT Connection
}
//-------------------------------End Reconnect MqttClient--------------------------------//

//---------------------------------------Callback----------------------------------------//
void callback(char* topic, byte * payload, unsigned int length) {
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //---------------------------------------/LED/---------------------------------------//
  //LED Color
  if (String(mqtt_LED_setcolor).equals(topic)) {
    char* red = strtok(message, ",");
    char* green = strtok(NULL, ",");
    char* blue = strtok(NULL, ",");

    LED_Color[0] = atoi(red);
    LED_Color[1] = atoi(green);
    LED_Color[2] = atoi(blue);

    char temp_buff[12];

    snprintf(temp_buff, 12, "%d,%d,%d", LED_Color[0], LED_Color[1], LED_Color[2]);
    mqtt_Client.publish(mqtt_LED_getcolor, temp_buff);
#ifdef DEBUG_NETWORK
    Serial.println("LED Color Update");
    Serial.println(temp_buff);
#endif
  }

  //LED Brightness
  if (String(mqtt_LED_setbrightness).equals(topic)) {
    LED_Brightness = atoi(message);
    char temp_buff[3];
    itoa(LED_Brightness, temp_buff, 10);
    mqtt_Client.publish(mqtt_LED_getbrightness, temp_buff);
#ifdef DEBUG_NETWORK
    Serial.println("LED Brightness Update");
    Serial.println(temp_buff);
#endif
  }

  //LED Control Active
  if (String(mqtt_LED_setstatus).equals(topic)) {
    if (strcmp(message, "ON") == 0) {
      LED_Active = true;
#ifdef DEBUG_NETWORK
      Serial.println("LED Controller ON");
#endif
      mqtt_Client.publish(mqtt_LED_getstatus, "ON");
    }
    else {
      LED_Active = false;
#ifdef DEBUG_NETWORK
      Serial.println("LED Controller OFF");
#endif
      mqtt_Client.publish(mqtt_LED_getstatus, "OFF");
    }
  }

  //LED Fadespeed
  if (String(mqtt_LED_FadeSpeed).equals(topic)) {
    char Buffer[3];
    Buffer[3] = '\0';
    for (int i = 0; i < 3; i++) {
      Buffer[i] = message[i];
    }
    int n = atoi(Buffer);
    LED_FadeSpeed = n;
#ifdef DEBUG_NETWORK
    Serial.println("LED Fade Speed Update");
    Serial.println(LED_FadeSpeed);
#endif
    //mqtt_Client.publish(mqtt_LED_TimeOut, message);
  }

  //---------------------------------------/IR/---------------------------------------//

  //Activated IR Control
  if (String(mqtt_IR_Active).equals(topic)) {
    if (*message == '1') {
#ifdef DEBUG_NETWORK
      Serial.println("IR Control Activated");
#endif
      IR_Active = true;
    }
    if (*message == '0') {
#ifdef DEBUG_NETWORK
      Serial.println("IR Control Deactivated");
#endif
      IR_Active = false;
    }
  }

  //---------------------------------------/DHT/---------------------------------------//


  //---------------------------------------/MOTION/---------------------------------------//
  //Motion Color
  if (String(mqtt_MOTION_Color).equals(topic)) {
    char* red = strtok(message, ",");
    char* green = strtok(NULL, ",");
    char* blue = strtok(NULL, ",");

    MOTION_Color[0] = atoi(red);
    MOTION_Color[1] = atoi(green);
    MOTION_Color[2] = atoi(blue);

    char temp_buff[12];

    snprintf(temp_buff, 12, "%d,%d,%d", MOTION_Color[0], MOTION_Color[1], MOTION_Color[2]);
    //mqtt_Client.publish(mqtt_MOTION_Color, temp_buff);
#ifdef DEBUG_NETWORK
    Serial.println("MOTION Color Update");
    Serial.println(temp_buff);
#endif
  }

  //Motion TimeOut
  if (String(mqtt_MOTION_TimeOut).equals(topic)) {
    char Buffer[3];
    Buffer[3] = '\0';
    for (int i = 0; i < 3; i++) {
      Buffer[i] = message[i];
    }
    int n = atoi(Buffer);
    TimeOut_MotionDeteced = (n * 1000);
#ifdef DEBUG_NETWORK
    Serial.println("Motion Detection Timeout Update");
    Serial.println(TimeOut_MotionDeteced);
#endif
    //mqtt_Client.publish(mqtt_MOTION_TimeOut, message);
  }

  //Activated Motion Detection
  if (String(mqtt_MOTION_Detection_Active).equals(topic)) {
    if (*message == '1') {
#ifdef DEBUG_NETWORK
      Serial.println("Motion Detection Activated");
#endif
      MOTION_Active = true;
    }
    if (*message == '0') {
#ifdef DEBUG_NETWORK
      Serial.println("Motion Detection Deactivated");
#endif
      MOTION_Active = false;
    }
  }

  //Activated Brightness Control
  if (String(mqtt_MOTION_Brightness_Control_Active).equals(topic)) {
    if (*message == '1') {
#ifdef  DEBUG_NETWORK
      Serial.println("Brightness Control Activated");
#endif
      BRIGHTNESS_CONTROL_Active = true;
    }
    if (*message == '0') {
#ifdef DEBUG_NETWORK
      Serial.println("Brightness Control Deactivated");
#endif
      MOTION_Brightness = 100;
      BRIGHTNESS_CONTROL_Active = false;
    }
  }

  //---------------------------------------/PARTY/---------------------------------------//
  //Activated Party Time
  if (String(mqtt_PARTY_Active).equals(topic)) {
    if (*message == '1') {
#ifdef DEBUG_NETWORK
      Serial.println("Party Time Activated");
#endif
      PARTY_Active = true;
    }
    if (*message == '0') {
#ifdef DEBUG_NETWORK
      Serial.println("Party Time Deactivated");
#endif
      PARTY_Active = false;
    }
  }

  //---------------------------------------/API/---------------------------------------//
  //API Update
  if (String(mqtt_API_Update_Allowed).equals(topic)) {
    if (*message == '1') {
#ifdef DEBUG_NETWORK
      Serial.println("API Update Allowed");
#endif DEBUG_NETWORK
      API_Active = true;

      UpdateTimeData();
      UpdateSunData();
    }
    if (*message == '0') {
#ifdef DEBUG_NETWORK
      Serial.println("API Update Not Allowed");
#endif
      API_Active = false;

      API_SunBelowHorizon = true;
      API_hour = 12;
      API_minute = 0;
    }
  }

}
//-------------------------------------End Callback--------------------------------------//
