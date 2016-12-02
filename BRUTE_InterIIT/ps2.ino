#include "PS2X_lib.h"  //for v1.6
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17
#define pressures   true
//#define pressures   false
#define rumble      true
//#define rumble      false

PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;
void setup(){
  Serial.begin(57600);
  delay(300);
  Serial1.begin(38400);
  delay(100);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  type = ps2x.readType(); 
  error=0;
}

void loop() {
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
    {
      Serial.println("StartHello");
      Serial1.write(98);
      Serial1.write(1);
    }
    else if(ps2x.Button(PSB_SELECT))
    {
      Serial.println("Select");
      Serial1.write(98);
      Serial1.write(2);
    }
    else if(ps2x.Button(PSB_PAD_UP))
    {
      Serial.println("up");
      Serial1.write(98);
      Serial1.write(5);
    }
    else if(ps2x.Button(PSB_PAD_DOWN))
    {
      Serial.println("down");
      Serial1.write(98);
      Serial1.write(6);
    }
     else if(ps2x.Button(PSB_PAD_RIGHT))
    { 
      Serial.println("right");
      Serial1.write(98);
      Serial1.write(11);
    }
     else if(ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.println("left");
      Serial1.write(98);
      Serial1.write(12);
    }
     else if(ps2x.Button(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
     {
      Serial.println("Circle");  
      Serial1.write(98);
      Serial1.write(11);
     }
     else if(ps2x.Button(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
     {
      Serial.println("Cross");
      Serial1.write(98);
      Serial1.write(12);
     }
     else if(ps2x.Button(PSB_SQUARE))              //will be TRUE if button was JUST released
     {
      Serial.println("Square");     
      Serial1.write(98);
      Serial1.write(17);
     }
     else if(ps2x.Button(PSB_TRIANGLE))              //will be TRUE if button was JUST released
     { 
      Serial.println("Triangle");
      Serial1.write(98);
      Serial1.write(18);
     }
     else if(ps2x.Button(PSB_L1))
     { 
      Serial.println("l1");
      Serial1.write(98);
      Serial1.write(11);
     }
     else if(ps2x.Button(PSB_L2))
     { 
      Serial.println("l2");  //not working
      Serial1.write(98);
      Serial1.write(12);
     }
     else if(ps2x.Button(PSB_R1))
     {
      Serial.println("r1");
      Serial1.write(98);
      Serial1.write(5);
     }
     else if(ps2x.Button(PSB_R2))
     { 
      Serial.println("r2");
      Serial1.write(98);
      Serial1.write(6);
     }
    
    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      Serial1.write(98);
      Serial1.write(9);
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      Serial1.write(98);
      Serial1.write(11);
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      Serial1.write(98);
      Serial1.write(12);
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      Serial1.write(98);
      Serial1.write(10);
    }   
    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");        
    }

    
    if(ps2x.Button(PSB_AL1) || ps2x.Button(PSB_AR1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
      
      if(ps2x.Analog(PSS_LY) < 5)
      {
        Serial.println("Move Up L");
        Serial1.write(98);
        Serial1.write(1);
      }
      else if(ps2x.Analog(PSS_LY) > 250)
      {
        Serial.println("Move Down L");
        Serial1.write(98);
        Serial1.write(2);
      }
      else if(ps2x.Analog(PSS_LX) < 5)
      {
        Serial.println("Move Left L");
        Serial1.write(98);
        Serial1.write(4);
      }
      else if(ps2x.Analog(PSS_LX) > 250)
      {
        Serial.println("Move Right L");
        Serial1.write(98);
        Serial1.write(3);
      }  
      else if(ps2x.Analog(PSS_RY) < 5)
      {
        Serial.println("Move Up R");
        Serial1.write(98);
        Serial1.write(23);
      }
      else if(ps2x.Analog(PSS_RY) > 250)
      {
        Serial.println("Move Down R");
        Serial1.write(98);
        Serial1.write(24);
      }
      else if(ps2x.Analog(PSS_RX) < 5)
      {
        Serial.println("Move Left R");
        Serial1.write(98);
        Serial1.write(25);
      }
      else if(ps2x.Analog(PSS_RX) > 250)
      {
        Serial.println("Move Right R");
        Serial1.write(98);
        Serial1.write(26);
      }
      else 
      {
        Serial.println("Stop");
        Serial1.write(98);
        Serial1.write(0);
      }
    }     
  delay(100);  
}
