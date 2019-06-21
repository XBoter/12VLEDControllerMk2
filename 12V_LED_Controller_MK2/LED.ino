
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

  switch (Strip) {

    case 1:
      FadeColorTo(1, mqtt_LED_Red_1, mqtt_LED_Green_1, mqtt_LED_Blue_1);
      FadeBrightnessTo(1, mqtt_LED_Brightness_1);
      break;

    case 2:
      FadeColorTo(2, mqtt_LED_Red_2, mqtt_LED_Green_2, mqtt_LED_Blue_2);
      FadeBrightnessTo(2, mqtt_LED_Brightness_2);
      break;

  }

}

//Party Mode
void PartyLight() {

  unsigned long CurMillis_EffectPartyMode = millis();

  boolean FinishedFadeStrip1 = false;
  boolean FinishedFadeStrip2 = false;

  switch (StatePartyMode) {

    case 0:
      while (true) {
        PartyColorRedStrip1   = (int)(random(0, 3)) * 127;
        PartyColorGreenStrip1 = (int)(random(0, 3)) * 127;
        PartyColorBlueStrip1  = (int)(random(0, 3)) * 127;
        if (!(PartyColorRedStrip1 == 254 and PartyColorGreenStrip1 == 254 and PartyColorBlueStrip1 == 254) and !(PartyColorRedStrip1 == 0 and PartyColorGreenStrip1 == 0 and PartyColorBlueStrip1 == 0)) {
          break;
        }
      }
      while (true) {
        PartyColorRedStrip2   = (int)(random(0, 3)) * 127;
        PartyColorGreenStrip2 = (int)(random(0, 3)) * 127;
        PartyColorBlueStrip2  = (int)(random(0, 3)) * 127;
        if (!(PartyColorRedStrip2 == 254 and PartyColorGreenStrip2 == 254 and PartyColorBlueStrip2 == 254) and !(PartyColorRedStrip2 == 0 and PartyColorGreenStrip2 == 0 and PartyColorBlueStrip2 == 0)) {
          break;
        }
      }
      SetColorTo(1, PartyColorRedStrip1, PartyColorGreenStrip1, PartyColorBlueStrip1);
      SetColorTo(2, PartyColorRedStrip2, PartyColorGreenStrip2, PartyColorBlueStrip2);
      StatePartyMode = 10;
      break;

    case 10:
      FinishedFadeStrip1 = FadeBrightnessTo(1, 100);
      FinishedFadeStrip2 = FadeBrightnessTo(2, 100);
      if (FinishedFadeStrip1 and FinishedFadeStrip2) {
        StatePartyMode = 20;
      }
      break;

    case 20:
      if (CurMillis_EffectPartyMode - PrevMillis_EffectPartyMode >= 200) {
        PrevMillis_EffectPartyMode = CurMillis_EffectPartyMode;
        StatePartyMode = 30;
      }
      break;

    case 30:
      FinishedFadeStrip1 = FadeBrightnessTo(1, 0);
      FinishedFadeStrip2 = FadeBrightnessTo(2, 0);
      if (FinishedFadeStrip1 and FinishedFadeStrip2) {
        StatePartyMode = 0;
      }
      break;

  }

}

//Weekend Mode
void WeekendLight() {

  //Set Brightness to Max
  FadeBrightnessTo(1, 100);
  FadeBrightnessTo(2, 100);

  unsigned long CurMillis_EffectWeekendMode = millis();
  if (CurMillis_EffectWeekendMode - PrevMillis_EffectWeekendMode >= TimeOut_EffectWeekendMode) {
    PrevMillis_EffectWeekendMode = CurMillis_EffectWeekendMode;

    SetColorTo(1, WeekendColorRed, WeekendColorGreen, WeekendColorBlue);
    SetColorTo(2, WeekendColorRed, WeekendColorGreen, WeekendColorBlue);

    switch (StateWeekend) {

      //Plus Green
      case 0:
        WeekendColorGreen++;
        if (WeekendColorGreen == 255) {
          StateWeekend = 10;
        }
        break;

      //Minus Red
      case 10:
        WeekendColorRed--;
        if (WeekendColorRed == 0) {
          StateWeekend = 20;
        }
        break;

      //Plus Blue
      case 20:
        WeekendColorBlue++;
        if (WeekendColorBlue == 255) {
          StateWeekend = 30;
        }
        break;

      //Minus Green
      case 30:
        WeekendColorGreen--;
        if (WeekendColorGreen == 0) {
          StateWeekend = 40;
        }
        break;

      //Plus Red
      case 40:
        WeekendColorRed++;
        if (WeekendColorRed == 255) {
          StateWeekend = 50;
        }
        break;

      //Minus Blue
      case 50:
        WeekendColorBlue--;
        if (WeekendColorBlue == 0) {
          StateWeekend = 0;
        }
        break;

    }

  }

}

//Force Mode
void ForceLight() {

  unsigned long CurMillis_EffectForceMode = millis();

  //Set Brightness to Max
  FadeBrightnessTo(1, 100);
  FadeBrightnessTo(2, 100);

  boolean FinishedFadeStrip1 = false;
  boolean FinishedFadeStrip2 = false;

  switch (StateForce) {

    case 0:
      FinishedFadeStrip1 = FadeColorTo(1, 255, 0, 0);
      FinishedFadeStrip2 = FadeColorTo(2, 255, 0, 0);
      if (FinishedFadeStrip1 and FinishedFadeStrip2) {
        PrevMillis_EffectForceMode = CurMillis_EffectForceMode;
        StateForce = 10;
      }
      break;

    case 10:
      if (CurMillis_EffectForceMode - PrevMillis_EffectForceMode >= TimeOut_EffectForceMode) {
        PrevMillis_EffectForceMode = CurMillis_EffectForceMode;
        StateForce = 20;
      }
      break;

    case 20:
      FinishedFadeStrip1 = FadeColorTo(1, 0, 0, 255);
      FinishedFadeStrip2 = FadeColorTo(2, 0, 0, 255);
      if (FinishedFadeStrip1 and FinishedFadeStrip2) {
        PrevMillis_EffectForceMode = CurMillis_EffectForceMode;
        StateForce = 30;
      }
      break;

    case 30:
      if (CurMillis_EffectForceMode - PrevMillis_EffectForceMode >= TimeOut_EffectForceMode) {
        PrevMillis_EffectForceMode = CurMillis_EffectForceMode;
        StateForce = 0;
      }
      break;

  }

}

//Good Night Mode
void GoodNightLight() {

  //---- Color Orange ----//
  SetColorTo(1, 255, 127, 0);
  SetColorTo(2, 255, 127, 0);

  unsigned long CurMillis_EffectGoodNightMode = millis();
  if (CurMillis_EffectGoodNightMode - PrevMillis_EffectGoodNightMode >= (mqtt_Global_Good_Night_Timeout * 1000)) {
    PrevMillis_EffectGoodNightMode = CurMillis_EffectGoodNightMode;
    if (StateGoodNight <= 100) {
      StateGoodNight += 10;
    }
  }

  switch (StateGoodNight) {

    //Turn Off all other Light Modes
    case 0:
      mqtt_Client.publish(mqtt_command_LED_Active, "0");
      mqtt_Client.publish(mqtt_command_LED_Active_2, "0");
      StateGoodNight = 10;
      break;

    case 10:
      //Brightness 100%
      FadeBrightnessTo(1, 100);
      FadeBrightnessTo(2, 100);
      break;

    case 20:
      //Brightness 75%
      FadeBrightnessTo(1, 75);
      FadeBrightnessTo(2, 75);
      break;

    case 30:
      //Brightness 50%
      FadeBrightnessTo(1, 50);
      FadeBrightnessTo(2, 50);
      break;

    case 40:
      //Brightness 25%
      FadeBrightnessTo(1, 25);
      FadeBrightnessTo(2, 25);
      break;

    case 50:
      //Brightness 0%
      FadeBrightnessTo(1, 0);
      FadeBrightnessTo(2, 0);
      break;

    //Turn Off Good Night Mode
    case 100:
      mqtt_Client.publish(mqtt_command_Global_GoodNight, "0");
      break;

  }

}

//Green blinking effect to show that no WiFi Connections is available
void Error_NoWiFiConnection() {

  unsigned long CurMillis_ErrorNoWiFiConnection = millis();

  //---- Color Green ----//
  SetColorTo(1, 0, 255, 0);
  SetColorTo(2, 0, 255, 0);

  boolean FadeFinishedStrip1 = false;
  boolean FadeFinishedStrip2 = false;

  switch (StateErrorNoWiFiConnection) {

    case 0:
      FadeFinishedStrip1 = FadeBrightnessTo(1, BrightnessErrorNoWiFiConnection);
      FadeFinishedStrip2 = FadeBrightnessTo(2, BrightnessErrorNoWiFiConnection);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoWiFiConnection = 10;
        PrevMillis_ErrorNoWiFiConnection = CurMillis_ErrorNoWiFiConnection;
      }
      break;

    case 10:
      if (CurMillis_ErrorNoWiFiConnection - PrevMillis_ErrorNoWiFiConnection >= 500) {
        PrevMillis_ErrorNoWiFiConnection = CurMillis_ErrorNoWiFiConnection;
        StateErrorNoWiFiConnection = 20;
      }
      break;

    case 20:
      FadeFinishedStrip1 = FadeBrightnessTo(1, 0);
      FadeFinishedStrip2 = FadeBrightnessTo(2, 0);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoWiFiConnection = 30;
        PrevMillis_ErrorNoWiFiConnection = CurMillis_ErrorNoWiFiConnection;
      }
      break;

    case 30:
      if (CurMillis_ErrorNoWiFiConnection - PrevMillis_ErrorNoWiFiConnection >= 1000) {
        PrevMillis_ErrorNoWiFiConnection = CurMillis_ErrorNoWiFiConnection;
        StateErrorNoWiFiConnection = 0;
      }
      break;
  }

}

//Blue blinking effect to show that no MQTT Connections is available
void Error_NoMqttConnection() {

  unsigned long CurMillis_ErrorNoMqttConnection = millis();

  //---- Color Blue ----//
  SetColorTo(1, 0, 0, 255);
  SetColorTo(2, 0, 0, 255);

  boolean FadeFinishedStrip1 = false;
  boolean FadeFinishedStrip2 = false;

  switch (StateErrorNoMqttConnection) {

    case 0:
      FadeFinishedStrip1 = FadeBrightnessTo(1, BrightnessErrorNoMqttConnection);
      FadeFinishedStrip2 = FadeBrightnessTo(2, BrightnessErrorNoWiFiConnection);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoMqttConnection = 10;
        PrevMillis_ErrorNoMqttConnection = CurMillis_ErrorNoMqttConnection;
      }
      break;

    case 10:
      if (CurMillis_ErrorNoMqttConnection - PrevMillis_ErrorNoMqttConnection >= 500) {
        PrevMillis_ErrorNoMqttConnection = CurMillis_ErrorNoMqttConnection;
        StateErrorNoMqttConnection = 20;
      }
      break;

    case 20:
      FadeFinishedStrip1 = FadeBrightnessTo(1, 0);
      FadeFinishedStrip2 = FadeBrightnessTo(2, 0);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoMqttConnection = 30;
        PrevMillis_ErrorNoMqttConnection = CurMillis_ErrorNoMqttConnection;
      }
      break;

    case 30:
      if (CurMillis_ErrorNoMqttConnection - PrevMillis_ErrorNoMqttConnection >= 1000) {
        PrevMillis_ErrorNoMqttConnection = CurMillis_ErrorNoMqttConnection;
        StateErrorNoMqttConnection = 0;
      }
      break;
  }

}

//Red blinking effect to show that an Error has occurred
void Error_GerneralError() {

  unsigned long CurMillis_ErrorGeneral = millis();

  //---- Color Red ----//
  SetColorTo(1, 255, 0, 0);
  SetColorTo(2, 255, 0, 0);

  boolean FadeFinishedStrip1 = false;
  boolean FadeFinishedStrip2 = false;

  switch (StateErrorGeneral) {

    case 0:
      FadeFinishedStrip1 = FadeBrightnessTo(1, BrightnessErrorGeneral);
      FadeFinishedStrip2 = FadeBrightnessTo(2, BrightnessErrorNoWiFiConnection);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorGeneral = 10;
        PrevMillis_ErrorGeneral = CurMillis_ErrorGeneral;
      }
      break;

    case 10:
      if (CurMillis_ErrorGeneral - PrevMillis_ErrorGeneral >= 500) {
        PrevMillis_ErrorGeneral = CurMillis_ErrorGeneral;
        StateErrorGeneral = 20;
      }
      break;

    case 20:
      FadeFinishedStrip1 = FadeBrightnessTo(1, 0);
      FadeFinishedStrip2 = FadeBrightnessTo(2, 0);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorGeneral = 30;
        PrevMillis_ErrorGeneral = CurMillis_ErrorGeneral;
      }
      break;

    case 30:
      if (CurMillis_ErrorGeneral - PrevMillis_ErrorGeneral >= 1000) {
        PrevMillis_ErrorGeneral = CurMillis_ErrorGeneral;
        StateErrorGeneral = 0;
      }
      break;
  }

}

boolean FadeBrightnessTo(uint8_t Strip, uint8_t NewBrightness) {

  unsigned long CurMillis_BrightnessFadeSpeed = millis();
  boolean FadeFinished = false;

  switch (Strip) {

    case 1:
      if (CurMillis_BrightnessFadeSpeed - PrevMillis_BrightnessFadeSpeedStrip1 >= mqtt_Global_Brightness_Fadespeed) {
        PrevMillis_BrightnessFadeSpeedStrip1 = CurMillis_BrightnessFadeSpeed;
        if (ActualBrightnessStrip_1 < NewBrightness) {
          ActualBrightnessStrip_1++;
        }
        if (ActualBrightnessStrip_1 > NewBrightness) {
          ActualBrightnessStrip_1--;
        }
        if (ActualBrightnessStrip_1 == NewBrightness) {
          ActualBrightnessStrip_1 = NewBrightness;
          FadeFinished = true;
        }
      }
      break;

    case 2:
      if (CurMillis_BrightnessFadeSpeed - PrevMillis_BrightnessFadeSpeedStrip2 >= mqtt_Global_Brightness_Fadespeed) {
        PrevMillis_BrightnessFadeSpeedStrip2 = CurMillis_BrightnessFadeSpeed;
        if (ActualBrightnessStrip_2 < NewBrightness) {
          ActualBrightnessStrip_2++;
        }
        if (ActualBrightnessStrip_2 > NewBrightness) {
          ActualBrightnessStrip_2--;
        }
        if (ActualBrightnessStrip_2 == NewBrightness) {
          ActualBrightnessStrip_2 = NewBrightness;
          FadeFinished = true;
        }
      }
      break;
  }

  return FadeFinished;
}


void SetColorTo(uint8_t Strip, uint8_t NewColorRed, uint8_t NewColorGreen, uint8_t NewColorBlue) {

  switch (Strip) {

    case 1:
      ActualColorRedStrip_1 = NewColorRed;
      ActualColorGreenStrip_1 = NewColorGreen;
      ActualColorBlueStrip_1 = NewColorBlue;
      break;

    case 2:
      ActualColorRedStrip_2 = NewColorRed;
      ActualColorGreenStrip_2 = NewColorGreen;
      ActualColorBlueStrip_2 = NewColorBlue;
      break;
  }

}

boolean FadeColorTo(uint8_t Strip , uint8_t NewColorRed, uint8_t NewColorGreen, uint8_t NewColorBlue) {

  unsigned long CurMillis_ColorFadeSpeed = millis();
  boolean FadeFinished = false;

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

        if ((ActualColorRedStrip_1 == NewColorRed) and (ActualColorGreenStrip_1 == NewColorGreen) and (ActualColorBlueStrip_1 == NewColorBlue)) {
          FadeFinished =  true;
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

        if ((ActualColorRedStrip_2 == NewColorRed) and (ActualColorGreenStrip_2 == NewColorGreen) and (ActualColorBlueStrip_2 == NewColorBlue)) {
          FadeFinished = true;
        }

      }
      break;
  }

  return FadeFinished;

}
