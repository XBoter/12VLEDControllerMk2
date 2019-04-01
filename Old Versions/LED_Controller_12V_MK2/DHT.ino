#ifdef DHT_Function
void checkDHT() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
#ifdef DEBUG_DHT
    Serial.println("Error reading temperature!");
#endif
  }
  else {
#ifdef DEBUG_DHT
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
#endif
    mqtt_Client.publish(mqtt_DHT_temperature, String(event.temperature).c_str());
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
#ifdef DEBUG_DHT
    Serial.println("Error reading humidity!");
#endif
  }
  else {
#ifdef DEBUG_DHT
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
#endif
    mqtt_Client.publish(mqtt_DHT_humidity, String(event.relative_humidity).c_str());
  }
}
#endif
