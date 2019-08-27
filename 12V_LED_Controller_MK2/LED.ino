
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

  //Set Brightness to Max
  FadeBrightnessTo(1, 100);
  FadeBrightnessTo(2, 100);

  switch (StatePartyMode) {

    case 0:
      while (true) {
        PartyColorRedStrip1   = (int)(random(0, 3)) * 127;
        PartyColorGreenStrip1 = (int)(random(0, 3)) * 127;
        PartyColorBlueStrip1  = (int)(random(0, 3)) * 127;
        if (!(PartyColorRedStrip1 == 254 and PartyColorGreenStrip1 == 254 and PartyColorBlueStrip1 == 254) and !(PartyColorRedStrip1 == 0 and PartyColorGreenStrip1 == 0 and PartyColorBlueStrip1 == 0)) {
          PartyColorRedStrip2 = PartyColorRedStrip1;
          PartyColorGreenStrip2 = PartyColorGreenStrip1;
          PartyColorBlueStrip2 = PartyColorBlueStrip1;
          break;
        }
      }
      StatePartyMode = 10;
      break;

    case 10:
      FinishedFadeStrip1 = FastFadeColorTo(1, PartyColorRedStrip1, PartyColorGreenStrip1, PartyColorBlueStrip1);
      FinishedFadeStrip2 = FastFadeColorTo(2, PartyColorRedStrip2, PartyColorGreenStrip2, PartyColorBlueStrip2);
      if (FinishedFadeStrip1 and FinishedFadeStrip2) {
        StatePartyMode = 20;
      }
      break;

    case 20:
      if (CurMillis_EffectPartyMode - PrevMillis_EffectPartyMode >= 500) {
        PrevMillis_EffectPartyMode = CurMillis_EffectPartyMode;
        StatePartyMode = 0;
      }
      break;

  }

}

//Weekend Mode
void WeekendLight() {
  boolean check1 = false, check2 = false;
  unsigned long CurMillis_EffectWeekendMode = 0;
  //Set Brightness to Max
  FadeBrightnessTo(1, 100);
  FadeBrightnessTo(2, 100);

  switch (PreStateWeekend) {

    case 0:
      check1 = FadeColorTo(1, WeekendColorRed, WeekendColorGreen, WeekendColorBlue);
      check2 = FadeColorTo(2, WeekendColorRed, WeekendColorGreen, WeekendColorBlue);
      //Check and Wait 5 sec before starting weekend mode
      CurMillis_EffectWeekendMode = millis();
      if (CurMillis_EffectWeekendMode - PrevMillis_EffectWeekendMode >= 5000) {
        if (check1 and check2) {
          PreStateWeekend = 10;
          PrevMillis_EffectWeekendMode = CurMillis_EffectWeekendMode;
        }
      }
      break;

    case 10:
      CurMillis_EffectWeekendMode = millis();
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
      break;

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
void GoodNightMode() {

  boolean check1 = false, check2 = false;

  unsigned long CurMillis_GoodNightMode = millis();
  if (CurMillis_GoodNightMode - PrevMillis_GoodNightMode >= (mqtt_Global_Good_Night_Timeout * 1000)) {
    if (StateGoodNight >= 10 and WaitFadeStrip1GoodNight and WaitFadeStrip2GoodNight) {
      PrevMillis_GoodNightMode = CurMillis_GoodNightMode;
      StateGoodNight += 10;
      WaitFadeStrip1GoodNight = false;
      WaitFadeStrip2GoodNight = false;
    }
  }

  switch (StateGoodNight) {

    case 0:
      //Fade Color to Orange

      if (mqtt_LED_Active_1) {
        check1 = FadeColorTo(1, 255, 127, 0);
      } else {
        check1 = true;
      }

      if (mqtt_LED_Active_2) {
        check2 = FadeColorTo(2, 255, 127, 0);
      } else {
        check2 = true;
      }

      if (check1 and check2) {
        StateGoodNight = 10;
        PrevMillis_GoodNightMode       = millis();
        StartBrightnessStrip1GoodNight = ActualBrightnessStrip_1;
        StartBrightnessStrip2GoodNight = ActualBrightnessStrip_2;
      }
      break;

    case 10:
      //Brightness 100% of Starting Brightness
      if (mqtt_LED_Active_1) {
        WaitFadeStrip1GoodNight = FadeBrightnessTo(1, uint8_t(StartBrightnessStrip1GoodNight * 1.0));
      } else {
        WaitFadeStrip1GoodNight = true;
      }
      if (mqtt_LED_Active_2) {
        WaitFadeStrip2GoodNight = FadeBrightnessTo(2, uint8_t(StartBrightnessStrip2GoodNight * 1.0));
      } else {
        WaitFadeStrip2GoodNight = true;
      }
      break;

    case 20:
      //Brightness 75% of Starting Brightness
      if (mqtt_LED_Active_1) {
        WaitFadeStrip1GoodNight = FadeBrightnessTo(1, uint8_t(StartBrightnessStrip1GoodNight * 0.75));
      } else {
        WaitFadeStrip1GoodNight = true;
      }
      if (mqtt_LED_Active_2) {
        WaitFadeStrip2GoodNight = FadeBrightnessTo(2, uint8_t(StartBrightnessStrip2GoodNight * 0.75));
      } else {
        WaitFadeStrip2GoodNight = true;
      }
      break;

    case 30:
      //Brightness 50% of Starting Brightness
      if (mqtt_LED_Active_1) {
        WaitFadeStrip1GoodNight = FadeBrightnessTo(1, uint8_t(StartBrightnessStrip1GoodNight * 0.5));
      } else {
        WaitFadeStrip1GoodNight = true;
      }
      if (mqtt_LED_Active_2) {
        WaitFadeStrip2GoodNight = FadeBrightnessTo(2, uint8_t(StartBrightnessStrip2GoodNight * 0.5));
      } else {
        WaitFadeStrip2GoodNight = true;
      }
      break;

    case 40:
      //Brightness 25% of Starting Brightness
      if (mqtt_LED_Active_1) {
        WaitFadeStrip1GoodNight = FadeBrightnessTo(1, uint8_t(StartBrightnessStrip1GoodNight * 0.25));
      } else {
        WaitFadeStrip1GoodNight = true;
      }
      if (mqtt_LED_Active_2) {
        WaitFadeStrip2GoodNight = FadeBrightnessTo(2, uint8_t(StartBrightnessStrip2GoodNight * 0.25));
      } else {
        WaitFadeStrip2GoodNight = true;
      }
      break;

    case 50:
      //Brightness 0% of Starting Brightness
      if (mqtt_LED_Active_1) {
        WaitFadeStrip1GoodNight = FadeBrightnessTo(1, uint8_t(StartBrightnessStrip1GoodNight * 0.0));
      } else {
        WaitFadeStrip1GoodNight = true;
      }
      if (mqtt_LED_Active_2) {
        WaitFadeStrip2GoodNight = FadeBrightnessTo(2, uint8_t(StartBrightnessStrip2GoodNight * 0.0));
      } else {
        WaitFadeStrip2GoodNight = true;
      }
      break;

    //Turn Off Strips
    case 60:
      //Reset and Strip turn of
      WaitFadeStrip1GoodNight = false;
      WaitFadeStrip2GoodNight = false;
      StateGoodNight = 0;
      mqtt_Global_GoodNight = false;
      if (mqtt_LED_Active_1) {
        mqtt_LED_Active_1 = false;
        mqtt_Client.publish(mqtt_state_LED_Active, "0");
      }
      if (mqtt_LED_Active_2) {
        mqtt_LED_Active_2 = false;
        mqtt_Client.publish(mqtt_state_LED_Active_2, "0");
      }
      break;

  }

}

//Good Morning Mode
void GoodMorningMode() {

  boolean check1 = false, check2 = false;

  unsigned long CurMillis_GoodMorningMode = millis();
  if (CurMillis_GoodMorningMode - PrevMillis_GoodMorningMode >= (mqtt_Global_Good_Morning_Timeout * 1000)) {
    if (StateGoodMorning <= 100 and WaitFadeStrip1GoodMorning and WaitFadeStrip2GoodMorning) {
      PrevMillis_GoodMorningMode = CurMillis_GoodMorningMode;
      StateGoodMorning += 10;
      WaitFadeStrip1GoodMorning = false;
      WaitFadeStrip2GoodMorning = false;
    }
  }

  switch (StateGoodMorning) {

    case 0:
      //Set Color to Orange
      if (!mqtt_LED_Active_1) {
        SetColorTo(1, 255, 127, 0);
      }
      if (!mqtt_LED_Active_2) {
        SetColorTo(2, 255, 127, 0);
      }

      //Brightness 25%
      if (!mqtt_LED_Active_1) {
        WaitFadeStrip1GoodMorning = FadeBrightnessTo(1, 25);
      } else {
        WaitFadeStrip1GoodMorning = true;
      }
      if (!mqtt_LED_Active_2) {
        WaitFadeStrip2GoodMorning = FadeBrightnessTo(2, 25);
      } else {
        WaitFadeStrip2GoodMorning = true;
      }
      break;

    case 10:
      //Brightness 50%
      if (!mqtt_LED_Active_1) {
        WaitFadeStrip1GoodMorning = FadeBrightnessTo(1, 50);
      } else {
        WaitFadeStrip1GoodMorning = true;
      }
      if (!mqtt_LED_Active_2) {
        WaitFadeStrip2GoodMorning = FadeBrightnessTo(2, 50);
      } else {
        WaitFadeStrip2GoodMorning = true;
      }
      break;

    case 20:
      //Brightness 75%
      if (!mqtt_LED_Active_1) {
        WaitFadeStrip1GoodMorning = FadeBrightnessTo(1, 75);
      } else {
        WaitFadeStrip1GoodMorning = true;
      }
      if (!mqtt_LED_Active_2) {
        WaitFadeStrip2GoodMorning = FadeBrightnessTo(2, 75);
      } else {
        WaitFadeStrip2GoodMorning = true;
      }
      break;

    case 30:
      //Brightness 100%
      if (!mqtt_LED_Active_1) {
        WaitFadeStrip1GoodMorning = FadeBrightnessTo(1, 100);
      } else {
        WaitFadeStrip1GoodMorning = true;
      }
      if (!mqtt_LED_Active_2) {
        WaitFadeStrip2GoodMorning = FadeBrightnessTo(2, 100);
      } else {
        WaitFadeStrip2GoodMorning = true;
      }
      break;

    case 40 ... 100:
      //Dummy Wait
      WaitFadeStrip1GoodMorning = true;
      WaitFadeStrip2GoodMorning = true;
      break;

    case 110:
      //Reset
      WaitFadeStrip1GoodMorning = false;
      WaitFadeStrip2GoodMorning = false;
      StateGoodMorning = 0;
      mqtt_Global_GoodMorning = false;
      if (!mqtt_LED_Active_1) {
        mqtt_LED_Active_1 = true;
        mqtt_Client.publish(mqtt_state_LED_Active, "1");
      }
      if (!mqtt_LED_Active_2) {
        mqtt_LED_Active_2 = true;
        mqtt_Client.publish(mqtt_state_LED_Active_2, "1");
      }
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

//Purple blinking effect to show that no MQTT Connections is available
void Error_NoApiConnection() {

  unsigned long CurMillis_ErrorNoApiConnection = millis();

  //---- Color Blue ----//
  SetColorTo(1, 255, 0, 255);
  SetColorTo(2, 255, 0, 255);

  boolean FadeFinishedStrip1 = false;
  boolean FadeFinishedStrip2 = false;

  switch (StateErrorNoApiConnection) {

    case 0:
      FadeFinishedStrip1 = FadeBrightnessTo(1, BrightnessErrorNoApiConnection);
      FadeFinishedStrip2 = FadeBrightnessTo(2, BrightnessErrorNoApiConnection);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoApiConnection = 10;
        PrevMillis_ErrorNoApiConnection = CurMillis_ErrorNoApiConnection;
      }
      break;

    case 10:
      if (CurMillis_ErrorNoApiConnection - PrevMillis_ErrorNoApiConnection >= 500) {
        PrevMillis_ErrorNoApiConnection = CurMillis_ErrorNoApiConnection;
        StateErrorNoApiConnection = 20;
      }
      break;

    case 20:
      FadeFinishedStrip1 = FadeBrightnessTo(1, 0);
      FadeFinishedStrip2 = FadeBrightnessTo(2, 0);
      if (FadeFinishedStrip1 and FadeFinishedStrip2) {
        StateErrorNoApiConnection = 30;
        PrevMillis_ErrorNoApiConnection = CurMillis_ErrorNoApiConnection;
      }
      break;

    case 30:
      if (CurMillis_ErrorNoApiConnection - PrevMillis_ErrorNoApiConnection >= 1000) {
        PrevMillis_ErrorNoApiConnection = CurMillis_ErrorNoApiConnection;
        StateErrorNoApiConnection = 0;
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

boolean FastFadeColorTo(uint8_t Strip , uint8_t NewColorRed, uint8_t NewColorGreen, uint8_t NewColorBlue) {

  unsigned long CurMillis_ColorFadeSpeed = millis();
  boolean FadeFinished = false;

  switch (Strip) {

    case 1:
      if (CurMillis_ColorFadeSpeed - PrevMillis_ColorFadeSpeedStrip1 >= int(mqtt_Global_Color_Fadespeed / 4.0)) {
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
