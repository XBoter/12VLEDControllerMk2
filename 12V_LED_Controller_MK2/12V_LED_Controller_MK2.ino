
//+++ Defines for Libarys +++//

//+++ Include Libarys +++//


//+++ Secret Header +++//
//#define Controller_DeskFront
//#define Controller_DeskBack
#define Controller_Bed
//#define Controller_WardrobeMiddle
//#define Controller_Shelf
//#define Controller_TvStand
//#define Controller_Kitchen
//#define Controller_Bath

#include <secret.h>

//-------------------- Basic Information --------------------//
//for Serial print Startup Info
#define Name        "12V LED Controller Mk2"
#define Programmer  "Nico Weidenfeller"
#define Created     "06.06.2019"
#define LastModifed "06.06.2019"
#define Version     "0.0.1"

/*
  Name          :   12V LED Controller Mk2
  Programmer    :   Nico Weidenfeller
  Created       :   06.06.2019
  Last Modifed  :   06.06.2019
  Version       :   0.0.1
  Description   :  

  ToDoList      :   - 

  Bugs          :   - 

  Optimize      :   - 

  Patchnotes    :   Version 0.0.1
                      Init Code for Mk2 LED Controller for 12V LED Strips with DHT, API, IR, Motion Detection Support.
       

*/





















void setup() {
  SetupController();  //Setup gets Called in the Controller Tab. Main Ino is only for Var decleration
}

void loop() {
  LoopController();   //Loop gets Called in the Controller Tab. Main Ino is only for Var decleration
}
