
void ReadDHTSensorData() {

  unsigned long CurMillis_ReatDHTSensorData = millis();
  if (CurMillis_ReatDHTSensorData - PrevMillis_ReatDHTSensorData >= TimeOut_ReatDHTSensorData) {
    PrevMillis_ReatDHTSensorData = CurMillis_ReatDHTSensorData;

    if (DHT_SENSOR == 1) {

      Information_DHT_DataRead = true;

      char HumidityData[10];
      char TemperatureData[10];

      //Read DHT Sensor Data
      Humidity = dht.readHumidity();
      Temperature = dht.readTemperature();

      sprintf(HumidityData, "%ld", Humidity);
      sprintf(TemperatureData, "%ld", Temperature);

      mqtt_Client.publish(mqtt_value_sensor_humidity, HumidityData);
      mqtt_Client.publish(mqtt_value_sensor_temp, TemperatureData);

    }

  } else {
    Information_DHT_DataRead = false;
  }

}
