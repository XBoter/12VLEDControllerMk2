
//Include Secret Header
#include <secrets.h>

//for Serial print Startup Info
#define Name        "RGB LED Strip Controller for 12V"
#define Programmer  "Nico Weidenfeller"
#define Created     "01.04.2019"
#define LastModifed "01.04.2019"
#define Version     "0.0.1"

/*
  Name          :   RGB LED Strip Controller for 12V
  Programmer    :   Nico Weidenfeller
  Created       :   01.04.2019
  Last Modifed  :   01.04.2019
  Version       :   0.0.1
  Description   :   RGB LED Strip Controller for 12V LEDs. Gets controlled by mqtt over HomeAssistant

  ToDoList      :   - Heartbeat
                    - Party Mode V3
                    - Weekend Mode V2
                    - Check if User is @Home
                    - Fixed Parameter for Fade etc
                    - API (Brightness Controll etc)
                    - Visual Debugg Options
                    - Motion Detection
                    - DHT Sensor
                    - Infrared Control
                    - Secure Way to activate the Options

  Error Help    :   -

  Patchnotes    :   Version 0.0.1
                      Creation

  EffectList    :   1. Nothing

*/


/*-------------------------------------- Setup --------------------------------------*/
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Name          : ");
  Serial.println(Name);
  Serial.print("Programmer    : ");
  Serial.println(Programmer);
  Serial.print("Created       : ");
  Serial.println(Created);
  Serial.print("Last Modifed  : ");
  Serial.println(LastModifed);
  Serial.print("Version       : ");
  Serial.println(Version);
  Serial.println("");

  Serial.println("Start Setup");

  Serial.println("Finished Setup");
}


/*-------------------------------------- Loop --------------------------------------*/
void loop() {

}
