

//PIR Controlled Motion Detection
void MotionDetection() {

  unsigned long CurMillis_MotionDetected = millis();

  boolean MotionDetectedSensor1 = false;
  boolean MotionDetectedSensor2 = false;

  //Read Motion Sensor 1 Data
  if (LED_STRIP_COUNT <= 1 and MOTION_SENSORS >= 1) {
    MotionDetectedSensor1 = digitalRead(PIN_MOTION_SENSOR_1);
  }

  //Read Motion Sensor 2 Data
  if (LED_STRIP_COUNT <= 1 and MOTION_SENSORS == 2) {
    MotionDetectedSensor2 = digitalRead(PIN_MOTION_SENSOR_2);
  }

  //For Information Tab
  if (MotionDetectedSensor1 or MotionDetectedSensor2) {
    PirSensorMotionDetected = true;
  } else {
    PirSensorMotionDetected = false;
  }

  //If sun is below horizon activate Motion Detection
  if (api_SunDown and mqtt_Global_Motion_Active) {

    if (MotionDetectedSensor1 or MotionDetectedSensor2) {
      PrevMillis_MotionDetected = CurMillis_MotionDetected;
      MotionOccured = true;
    }

    if (CurMillis_MotionDetected - PrevMillis_MotionDetected >= (mqtt_Motion_Timout * 1000)) {
      PrevMillis_MotionDetected = CurMillis_MotionDetected;
      MotionOccured = false;
    }

  } else {
    MotionOccured = false;
  }

}


//API Controlled Brightness for Motion Detection
void MotionBrightnessControl() {

  //If sun is below horizon adjust Brightnes for Motion Detection
  if (api_SunDown) {

    switch (api_TimeHour) {
      case 0: MotionBrightness = 30;
        break;
      case 1: MotionBrightness = 30;
        break;
      case 2: MotionBrightness = 30;
        break;
      case 3: MotionBrightness = 40;
        break;
      case 4: MotionBrightness = 40;
        break;
      case 5: MotionBrightness = 50;
        break;
      case 6: MotionBrightness = 60;
        break;
      case 7: MotionBrightness = 70;
        break;
      case 8: MotionBrightness = 80;
        break;
      case 9: MotionBrightness = 100;
        break;
      case 10: MotionBrightness = 100;
        break;
      case 11: MotionBrightness = 100;
        break;
      case 12: MotionBrightness = 100;
        break;
      case 13: MotionBrightness = 100;
        break;
      case 14: MotionBrightness = 100;
        break;
      case 15: MotionBrightness = 100;
        break;
      case 16: MotionBrightness = 100;
        break;
      case 17: MotionBrightness = 100;
        break;
      case 18: MotionBrightness = 100;
        break;
      case 19: MotionBrightness = 90;
        break;
      case 20: MotionBrightness = 80;
        break;
      case 21: MotionBrightness = 60;
        break;
      case 22: MotionBrightness = 40;
        break;
      case 23: MotionBrightness = 30;
        break;

      default:
        MotionBrightness = 0;
        break;
    }

  }

}
