
void IRControl() {

  if (IrRecv.decode(&IrRecvResult)) {

    //For Brightness MQTT Commands
    char CommandStrip1[4];
    char CommandStrip2[4];

    //Column from Right to Left
    switch (IrRecvResult.value) {

      //------ Column 1 ------//
      case 0xF7C03F:
        mqtt_Client.publish(mqtt_command_LED_Active, "1");
        mqtt_Client.publish(mqtt_command_LED_Active_2, "1");
        break;
      case 0xF7E01F:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,255,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,255,255");
        break;
      case 0xF7D02F:
        break;
      case 0xF7F00F:
        break;
      case 0xF7C837:
        break;
      case 0xF7E817:
        break;

      //------ Column 2 ------//
      case 0xF740BF:
        mqtt_Client.publish(mqtt_command_LED_Active, "0");
        mqtt_Client.publish(mqtt_command_LED_Active_2, "0");
        break;
      case 0xF7609F:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,0,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,0,255");
        break;
      case 0xF750AF:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,63,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,63,255");
        break;
      case 0xF7708F:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,127,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,127,255");
        break;
      case 0xF748B7:
        mqtt_Client.publish(mqtt_command_LED_Color, "63,0,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "63,0,255");
        break;
      case 0xF76897:
        mqtt_Client.publish(mqtt_command_LED_Color, "127,0,255");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "127,0,255");
        break;

      //------ Column 3 ------//
      case 0xF7807F:
        //Brightness Strip 1
        if ((mqtt_LED_Brightness_1 - 10) < 0) {
          mqtt_LED_Brightness_1 = 0;
        } else {
          mqtt_LED_Brightness_1 -= 10;
        }
        sprintf(CommandStrip1, "%ld", mqtt_LED_Brightness_1);

        //Brightness Strip 2
        if ((mqtt_LED_Brightness_2 - 10) < 0) {
          mqtt_LED_Brightness_2 = 0;
        } else {
          mqtt_LED_Brightness_2 -= 10;
        }
        sprintf(CommandStrip2, "%ld", mqtt_LED_Brightness_2);

        mqtt_Client.publish(mqtt_command_LED_Brightness, CommandStrip1);
        mqtt_Client.publish(mqtt_command_LED_Brightness_2, CommandStrip2);
        break;
      case 0xF7A05F:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,255,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,255,0");
        break;
      case 0xF7906F:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,255,63");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,255,63");
        break;
      case 0xF7B04F:
        mqtt_Client.publish(mqtt_command_LED_Color, "0,255,127");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "0,255,127");
        break;
      case 0xF78877:
        mqtt_Client.publish(mqtt_command_LED_Color, "63,255,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "63,255,0");
        break;
      case 0xF7A857:
        mqtt_Client.publish(mqtt_command_LED_Color, "127,255,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "127,255,0");
        break;


      //------ Column 4 ------//
      case 0xF700FF:
        //Brightness Strip 1
        if ((mqtt_LED_Brightness_1 + 10) > 100) {
          mqtt_LED_Brightness_1 = 100;
        } else {
          mqtt_LED_Brightness_1 += 10;
        }
        sprintf(CommandStrip1, "%ld", mqtt_LED_Brightness_1);

        //Brightness Strip 2
        if ((mqtt_LED_Brightness_2 + 10) > 100) {
          mqtt_LED_Brightness_2 = 100;
        } else {
          mqtt_LED_Brightness_2 += 10;
        }
        sprintf(CommandStrip2, "%ld", mqtt_LED_Brightness_2);

        mqtt_Client.publish(mqtt_command_LED_Brightness, CommandStrip1);
        mqtt_Client.publish(mqtt_command_LED_Brightness_2, CommandStrip2);
        break;
      case 0xF720DF:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,0,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,0,0");
        break;
      case 0xF710EF:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,0,63");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,0,63");
        break;
      case 0xF730CF:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,0,127");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,0,127");
        break;
      case 0xF708F7:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,63,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,63,0");
        break;
      case 0xF728D7:
        mqtt_Client.publish(mqtt_command_LED_Color, "255,127,0");
        mqtt_Client.publish(mqtt_command_LED_Color_2, "255,127,0");
        break;

    }

    IrRecv.resume();  // Receive the next value
  }

}
