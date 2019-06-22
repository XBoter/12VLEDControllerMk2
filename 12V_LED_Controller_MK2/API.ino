
void GetApiSunData() {

  unsigned long CurMillis_ApiUpdateRate = millis();

  //Only Update when Connected to API
  if (http_Client.connected()) {

    if (CurMillis_ApiUpdateRate - PrevMillis_ApiSunUpdateRate >= TimeOut_ApiUpdateRate) {
      PrevMillis_ApiSunUpdateRate = CurMillis_ApiUpdateRate;

      String timeData;
      String lastRead;

      //------ Get Sun Data ------//
      String sun;
      http_Client.println("GET http://192.168.99.99:8123/api/states/sun.sun?api_password=124558458467145");
      http_Client.println();

      for (int i = 0; i < 1000; i++) {
        if (lastRead == "state") {
          timeData = http_Client.readStringUntil('"');
          sun = http_Client.readStringUntil('"');    //Get Sun State
          if (sun.equals("below_horizon")) {
            api_SunDown = 1;
          } else {
            api_SunDown = 0;
          }
          break;
        } else {
          timeData = http_Client.readStringUntil('"');
        }
        if (timeData == NULL) {
          break;
        }
        lastRead = timeData;
      }

    }

  }

}


void GetApiTimeData() {

  unsigned long CurMillis_ApiUpdateRate = millis();

  //Only Update when Connected to API
  if (http_Client.connected()) {

    if (CurMillis_ApiUpdateRate - PrevMillis_ApiTimeUpdateRate >= TimeOut_ApiUpdateRate) {
      PrevMillis_ApiTimeUpdateRate = CurMillis_ApiUpdateRate;

      String timeData;
      String lastRead;

      //------ Get Time Data ------//
      String hour;
      String minute;
      http_Client.println("GET http://192.168.99.99:8123/api/states/sensor.time?api_password=124558458467145");
      http_Client.println();

      for (int i = 0; i < 1000; i++) {
        if (lastRead == "state") {
          timeData = http_Client.readStringUntil('"');
          hour = http_Client.readStringUntil(':');    //Time Hour
          minute = http_Client.readStringUntil('"');    //Time Minutes
          api_TimeHour = hour.toInt();
          api_TimeMinute = minute.toInt();
          break;
        } else {
          timeData = http_Client.readStringUntil('"');
        }
        if (timeData == NULL) {
          break;
        }
        lastRead = timeData;
      }

    }

  }

}
