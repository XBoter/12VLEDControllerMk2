
//API Controlled Brightness for Motion Detection
void MotionBrightnessControl() {

  //If sun is below horizon adjust Brightnes for Motion Detection
  if (api_SunDown) {

    switch (api_TimeHour) {
      case 0: MotionBrightness = 40;
        break;
      case 1: MotionBrightness = 40;
        break;
      case 2: MotionBrightness = 40;
        break;
      case 3: MotionBrightness = 40;
        break;
      case 4: MotionBrightness = 40;
        break;
      case 5: MotionBrightness = 40;
        break;
      case 6: MotionBrightness = 40;
        break;
      case 7: MotionBrightness = 40;
        break;
      case 8: MotionBrightness = 40;
        break;
      case 9: MotionBrightness = 40;
        break;
      case 10: MotionBrightness = 40;
        break;
      case 11: MotionBrightness = 40;
        break;
      case 12: MotionBrightness = 40;
        break;
      case 13: MotionBrightness = 40;
        break;
      case 14: MotionBrightness = 40;
        break;
      case 15: MotionBrightness = 40;
        break;
      case 16: MotionBrightness = 40;
        break;
      case 17: MotionBrightness = 40;
        break;
      case 18: MotionBrightness = 40;
        break;
      case 19: MotionBrightness = 40;
        break;
      case 20: MotionBrightness = 40;
        break;
      case 21: MotionBrightness = 40;
        break;
      case 22: MotionBrightness = 40;
        break;
      case 23: MotionBrightness = 40;
        break;

      default:
        MotionBrightness = 0;
        break;
    }

  }

}


//PIR Controlled Motion Detection
void MotionDetection() {

  //If sun is below horizon activate Motion Detection
  if (api_SunDown) {

  }

}
