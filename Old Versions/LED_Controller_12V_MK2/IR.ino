#ifdef IR_Function
//#######################################IR Sensor#########################################//
//-------------------------------------Use IR Message------------------------------------//
void IrMessageUse() {
  switch (IrMessageValue) {

    //First Row from Remote
    case 16187647 :
      //Brightness Up
      if (LED_Brightness < 100) {
        if (LED_Brightness + 10 < 100) {
          LED_Brightness += 10;
        } else {
          LED_Brightness = 100;
        }
      }
      char temp_buff2[3];
      itoa(LED_Brightness, temp_buff2, 10);
      mqtt_Client.publish(mqtt_LED_getbrightness, temp_buff2);
      break;
    case 16195807 :
      //Red
      LED_Color[0] = 255;
      LED_Color[1] = 0;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,0,0");
      break;
    case 16191727 :
      //Red Light
      LED_Color[0] = 255;
      LED_Color[1] = 63;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,63,0");
      break;
    case 16199887 :
      //Red Light Light
      LED_Color[0] = 255;
      LED_Color[1] = 127;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,127,0");
      break;
    case 16189687 :
      //Red Light Light Light
      LED_Color[0] = 255;
      LED_Color[1] = 191;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,191,0");
      break;
    case 16197847 :
      //Red Light Light Light Light
      LED_Color[0] = 255;
      LED_Color[1] = 255;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,255,0");
      break;

    //Secont Row from Remote
    case 16220287 :
      //Brightness Down
      if (LED_Brightness > 0) {
        if (LED_Brightness - 10 > 0) {
          LED_Brightness -= 10;
        } else {
          LED_Brightness = 0;
        }
      }
      char temp_buff1[3];
      itoa(LED_Brightness, temp_buff1, 10);
      mqtt_Client.publish(mqtt_LED_getbrightness, temp_buff1);
      break;
    case 16228447 :
      //Green
      LED_Color[0] = 0;
      LED_Color[1] = 255;
      LED_Color[2] = 0;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,255,0");
      break;
    case 16224367 :
      //Green Light
      LED_Color[0] = 0;
      LED_Color[1] = 255;
      LED_Color[2] = 63;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,255,63");
      break;
    case 16232527 :
      //Green Light Light
      LED_Color[0] = 0;
      LED_Color[1] = 255;
      LED_Color[2] = 127;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,255,127");
      break;
    case 16222327 :
      //Green Light Light Light
      LED_Color[0] = 0;
      LED_Color[1] = 255;
      LED_Color[2] = 191;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,255,191");
      break;
    case 16230487 :
      //Green Light Light Light Light
      LED_Color[0] = 0;
      LED_Color[1] = 255;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,255,255");
      break;

    //Third Row from Remote
    case 16203967 :
      //OFF
      LED_Active = false;
      mqtt_Client.publish(mqtt_LED_getstatus, "OFF");
      break;
    case 16212127 :
      //Blue
      LED_Color[0] = 0;
      LED_Color[1] = 0;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "0,0,255");
      break;
    case 16208047 :
      //Blue Light
      LED_Color[0] = 63;
      LED_Color[1] = 0;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "63,0,255");
      break;
    case 16216207 :
      //Blue Light Light
      LED_Color[0] = 127;
      LED_Color[1] = 0;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "127,0,255");
      break;
    case 16206007 :
      //Blue Light Light Light
      LED_Color[0] = 191;
      LED_Color[1] = 0;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "191,0,255");
      break;
    case 16214167 :
      //Blue Light Light Light Light
      LED_Color[0] = 255;
      LED_Color[1] = 0;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,0,255");
      break;

    //Fourth Row from Remote
    case 16236607 :
      //ON
      LED_Active = true;
      mqtt_Client.publish(mqtt_LED_getstatus, "ON");
      break;
    case 16244767 :
      //White
      LED_Color[0] = 255;
      LED_Color[1] = 255;
      LED_Color[2] = 255;
      mqtt_Client.publish(mqtt_LED_getcolor, "255,255,255");
      break;
    case 16773135 : ; break; //Not Used
    case 16771095 : ; break; //Not Used
    case 16767015 : ; break; //Not Used
    case 16762935 : ; break; //Not Used
  }

  //Reset
  IrMessageValue = 0;
}
//-----------------------------------End Use IR Message----------------------------------//

//------------------------------------Check IR Message-----------------------------------//
void checkIrMessage() {
  if (IrDetector.decode(&IrDetectorResults)) {
    IrMessageValue = IrDetectorResults.value;
    IrDetector.resume();
#ifdef DEBUG_IR
    Serial.print("IR Message = ");
    Serial.println(IrMessageValue);
#endif
  }
}
//---------------------------------End Check IR Message---------------------------------//
//#######################################End IR Sensor#########################################//
#endif
