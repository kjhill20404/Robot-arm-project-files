
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <Wire.h>

#include <ArduinoNunchuk.h> 
ArduinoNunchuk nunchuk = ArduinoNunchuk();//for the library to know its nickname
//}

int changeBy; //this var is used to keep track of how many degreess to move the servos at once
int tracker2; //this is used to prevent servo5 from moving when it shoulden't

int restrictionPin = 22; //there is a latching switch connected to this pin
bool restrictMovement; //bool var used to know when to allow the robot to move past a limit determined later
// tell the Arduino that this var is a servo from the <Servo.h> library {
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
//}

void setup() {
  //this portion of code is to assign the servos to the DP used to control them { 
  servo1.attach(9);
  servo2.attach(6);
  servo3.attach(5);
  servo4.attach(3);
  servo5.attach(11);
  //}
  pinMode(restrictionPin,INPUT);
  nunchuk.init(); // tell the <ArduinoNunchuk.h> library to start looking for controllers 
  Serial.begin(19200);// start the debug viewer  

}

void loop() {
  nunchuk.update();// the values of all the inputs from the controller
  changeBy = 1; //giving this a defalt value

  //this is to deturmine if movement should be restricted by checking the switch
  if(digitalRead(restrictionPin) == LOW){ //if the switch ??? the following code will be run
    restrictMovement = 1; //restrictMovement is set to 1
  }else{ //if the switch is set to !??? the following code is run
    restrictMovement = 0; //restrictMovement is set to 0
  }

  //set changeBy and tracker2 values based on the pressed pressed
  if (nunchuk.zButton == HIGH) { // the following code will only run if the Z button is pressed 
    if (nunchuk.cButton == HIGH) { // the following code will only run if the C button is pressed
      changeBy = 3; //changeBy is set to 3 to increse the speed of the arms movement
      tracker2 = 1; //tracker2 is set to 1 
    } 
  } 

  else { //the following line of code will run if the Z button is any value other than HIGH (LOW)
    changeBy = 1; //changeBy is set to 1 to move the servos at a normal speed
  }
  
  
  if (changeBy == 1){ //the following code will only run if changeBy is equal to 1 
    if ( nunchuk.zButton == HIGH) { //the following code will only run if the Z button is pressed
      tracker2 = 2; //tracker2 is set to 2
      if (nunchuk.analogY > 140) { //the following code will only run if the joysticks Y-axis potentiometer is greater than 140
        servo3.write(servo3.read() - changeBy); //move servo3 by -changeBy degrees
        servo2.write(servo2.read()); //keep servo2 at the same value it is currently at NOTE: I put this here becuse the servo was moving when it shoulden't have
      }
      if (nunchuk.analogY < 110) { //the following code will only run if the joysticks Y-axis of the potentiometer is less than 110
        servo3.write(servo3.read() + changeBy); //move servo3 by +changeBy degrees
        servo2.write(servo2.read()); //as stated in the previous condional
      }
      if (nunchuk.analogX < 100) { //the following code will only run if the joysticks X-axis of the potentiometer is less than 100
        servo5.write(servo5.read() + changeBy); //move servo5 by +changeBy degrees
      }
      if (nunchuk.analogX > 145) { //the following code will only run if the joysticks X-axis of the potentiometer is greater than 145
        servo5.write(servo5.read() - changeBy); //move servo5 by -changeBy degreess
      }
    } else { //the following code will run if the Z button is not pressed  
      tracker2 = 1; //set tracker2 to 1
    }
  }

  //set all servos to the middle of all there range
  if (nunchuk.zButton == LOW && nunchuk.cButton == HIGH) { //the following code will only run if the Z buttion is not pressed and the C button is pressed
    //the following code setts all the servos to a 90 degrees from there 0 degree value {
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    servo5.write(50);
    //}
  }
  

  //limit how low the arm can go to prevent it from wanting more current than is being supplied {
  if (tracker2 == 1) { //the following code will only run if tracker2 is equal to 1
    if (nunchuk.analogY > 150 ) { //the following code will only run if the joysticks Y-axis value is greater than 145
      if (servo2.read() == 155 && restrictMovement == 1) { //the following code will only run if servo2 is at 155 degrees and restrictMovement is 1
        servo3.write(servo3.read() - changeBy); //move servo3 by -changeBy degrees
      }
      else { //the following code will run if servo2 is not at 155 degrees
        servo2.write(servo2.read() + changeBy); //move servo2 by +changeBy degrees
      }
    }

    if (nunchuk.analogY < 110) { //the following code will only run if the joystick Y-axis value is less than 110
      if (servo2.read() == 45 && restrictMovement == 1) { //the following code will only run if servo2 is at 45 degrees and restrictMovement is 1
        servo3.write(servo3.read() + changeBy); //move servo3 by +changeBy
      }
      else { //the following code will run if servo2 in not at 45 degrees
        servo2.write(servo2.read() - changeBy); //move servo2 by -changeBy degrees
      }
    }
    //axis 1 controle (turn table)
    if (nunchuk.analogX < 100) { //the following code will only run if the joystick X-axis is less than 100
      servo1.write(servo1.read() + changeBy); //move servo1 by +changeBy degrees
    }
    if (nunchuk.analogX > 145) { //the following code will only run if the joystick X-axis is greater than 145
      servo1.write(servo1.read() - changeBy); //move servo1 by -changeBy degrees
    }
  }
  //debug(); //call the debug function

  delay(20); //delay 20 microseconds
} //end loop function

void debug() { //this is a funtion to print values to the Serial line
  Serial.print("joyY: "); 
  Serial.print(nunchuk.analogY);
  Serial.print(" | joyX: ");
  Serial.print(nunchuk.analogX);
  Serial.print(" | cButtion:");
  Serial.print(nunchuk.cButton);
  Serial.print(" | restrictMovement: ");
  Serial.print(restrictMovement);
  Serial.print(" | servo: ");
  Serial.println(servo5.read());
}
