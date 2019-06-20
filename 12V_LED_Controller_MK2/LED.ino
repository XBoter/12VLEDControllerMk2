
void ShowLED() {

  if (LED_STRIP_COUNT >= 1) {
    //LED_STRIP_1_RED
    analogWrite(PIN_LED_STRIP_1_RED, ((ActualColorRedStrip_1 * 4)  * ActualBrightnessStrip_1) / 100);
    ///LED_STRIP_1_GREEN
    analogWrite(PIN_LED_STRIP_1_GREEN, ((ActualColorGreenStrip_1 * 4) * ActualBrightnessStrip_1) / 100);
    ///LED_STRIP_1_BLUE
    analogWrite(PIN_LED_STRIP_1_BLUE, ((ActualColorBlueStrip_1 * 4) * ActualBrightnessStrip_1) / 100);
  }

  if (LED_STRIP_COUNT >= 2) {
    //LED_STRIP_2_RED
    analogWrite(PIN_LED_STRIP_2_RED, ((ActualColorRedStrip_2 * 4)  * ActualBrightnessStrip_2) / 100);
    ///LED_STRIP_2_GREEN
    analogWrite(PIN_LED_STRIP_2_GREEN, ((ActualColorGreenStrip_2 * 4) * ActualBrightnessStrip_2) / 100);
    ///LED_STRIP_2_BLUE
    analogWrite(PIN_LED_STRIP_2_BLUE, ((ActualColorBlueStrip_2 * 4) * ActualBrightnessStrip_2) / 100);
  }

}

//Normal Light Controll
void NormalLight(uint8_t Strip) {

}

//Party Mode
void PartyLight(uint8_t Strip) {

}

//Weekend Mode
void WeekendLight(uint8_t Strip) {

}

//Force Mode
void ForceLight(uint8_t Strip) {

}

//Good Night Mode
void GoodNightLight(uint8_t Strip) {

}

//Green blinking effect to show that no WiFi Connections is available
void Error_NoWiFiConnection() {

  unsigned long CurMillis_ErrorNoWiFiConnection = millis();
  if (CurMillis_ErrorNoWiFiConnection - PrevMillis_ErrorNoWiFiConnection >= TimeOut_ErrorEffectSpeed) {
    PrevMillis_ErrorNoWiFiConnection = CurMillis_ErrorNoWiFiConnection;

    if (SwapFadeErrorNoWiFiConnection) {

    } else {

    }

  }

}

//Blue blinking effect to show that no MQTT Connections is available
void Error_NoMqttConnection() {


}

//Red blinking effect to show that an Error has occurred
void Error_GerneralError() {


}

boolean FadeBrightnessTo(uint8_t Strip, uint8_t NewBrightness) {

  unsigned long CurMillis_BrightnessFadeSpeed = millis();

  switch (Strip) {

    case 1:
      if (CurMillis_BrightnessFadeSpeed - PrevMillis_BrightnessFadeSpeedStrip1 >= mqtt_Global_Brightness_Fadespeed) {
        PrevMillis_BrightnessFadeSpeedStrip1 = CurMillis_BrightnessFadeSpeed;
        if (ActualBrightnessStrip_1 < NewBrightness) {
          ActualBrightnessStrip_1 += 2;
        }
        if (ActualBrightnessStrip_1 > NewBrightness) {
          ActualBrightnessStrip_1 -= 2;
        }
        if (((ActualBrightnessStrip_1 - 2) >= NewBrightness) and (ActualBrightnessStrip_1 + 2 <= NewBrightness)) {
          ActualBrightnessStrip_1 = NewBrightness;
        }
      }
      break;

    case 2:
      if (CurMillis_BrightnessFadeSpeed - PrevMillis_BrightnessFadeSpeedStrip2 >= mqtt_Global_Brightness_Fadespeed) {
        PrevMillis_BrightnessFadeSpeedStrip2 = CurMillis_BrightnessFadeSpeed;
        if (ActualBrightnessStrip_2 < NewBrightness) {
          ActualBrightnessStrip_2 += 2;
        }
        if (ActualBrightnessStrip_2 > NewBrightness) {
          ActualBrightnessStrip_2 -= 2;
        }
        if (((ActualBrightnessStrip_2 - 2) >= NewBrightness) and (ActualBrightnessStrip_2 + 2 <= NewBrightness)) {
          ActualBrightnessStrip_2 = NewBrightness;
        }
      }
      break;
  }

}


boolean FadeColorTo(uint8_t Strip , uint8_t NewColorRed, uint8_t NewColorGreen, uint8_t NewColorBlue) {

  unsigned long CurMillis_ColorFadeSpeed = millis();

  switch (Strip) {

    case 1:
      if (CurMillis_ColorFadeSpeed - PrevMillis_ColorFadeSpeedStrip1 >= mqtt_Global_Color_Fadespeed) {
        PrevMillis_ColorFadeSpeedStrip1 = CurMillis_ColorFadeSpeed;

        //---- RED ----//
        if (ActualColorRedStrip_1 < NewColorRed) {
          ActualColorRedStrip_1++;
        }
        if (ActualColorRedStrip_1 > NewColorRed) {
          ActualColorRedStrip_1--;
        }
        if (ActualColorRedStrip_1 == NewColorRed) {
          ActualColorRedStrip_1 = NewColorRed;
        }

        //---- GREEN ----//
        if (ActualColorGreenStrip_1 < NewColorGreen) {
          ActualColorGreenStrip_1++;
        }
        if (ActualColorGreenStrip_1 > NewColorGreen) {
          ActualColorGreenStrip_1--;
        }
        if (ActualColorGreenStrip_1 == NewColorGreen) {
          ActualColorGreenStrip_1 = NewColorGreen;
        }

        //---- BLUE ----//
        if (ActualColorBlueStrip_1 < NewColorBlue) {
          ActualColorBlueStrip_1++;
        }
        if (ActualColorBlueStrip_1 > NewColorBlue) {
          ActualColorBlueStrip_1--;
        }
        if (ActualColorBlueStrip_1 == NewColorBlue) {
          ActualColorBlueStrip_1 = NewColorBlue;
        }

      }
      break;

    case 2:
      if (CurMillis_ColorFadeSpeed - PrevMillis_ColorFadeSpeedStrip2 >= mqtt_Global_Color_Fadespeed) {
        PrevMillis_ColorFadeSpeedStrip2 = CurMillis_ColorFadeSpeed;

        //---- RED ----//
        if (ActualColorRedStrip_2 < NewColorRed) {
          ActualColorRedStrip_2++;
        }
        if (ActualColorRedStrip_2 > NewColorRed) {
          ActualColorRedStrip_2--;
        }
        if (ActualColorRedStrip_2 == NewColorRed) {
          ActualColorRedStrip_2 = NewColorRed;
        }

        //---- GREEN ----//
        if (ActualColorGreenStrip_2 < NewColorGreen) {
          ActualColorGreenStrip_2++;
        }
        if (ActualColorGreenStrip_2 > NewColorGreen) {
          ActualColorGreenStrip_2--;
        }
        if (ActualColorGreenStrip_2 == NewColorGreen) {
          ActualColorGreenStrip_2 = NewColorGreen;
        }

        //---- BLUE ----//
        if (ActualColorBlueStrip_2 < NewColorBlue) {
          ActualColorBlueStrip_2++;
        }
        if (ActualColorBlueStrip_2 > NewColorBlue) {
          ActualColorBlueStrip_2--;
        }
        if (ActualColorBlueStrip_2 == NewColorBlue) {
          ActualColorBlueStrip_2 = NewColorBlue;
        }

      }
      break;
  }

}
