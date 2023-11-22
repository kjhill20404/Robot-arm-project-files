#include <Servo.h>

// OLED test.
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// Define the initial angle position of steering gear test.
int pos = 50;
// Define the minimum angle position of steering gear test.
int posMin = 20;
// Define the maximum angle position of steering gear test.
int posMax = 60;

// Uniformly define all pins (test load steering gear).
int pin[] = {9, 6, 5, 3, 11, 10};


// Define the number of test pins.
const int PIN_COUNT = sizeof(pin) / sizeof(pin[0]);
// create servo objects to control servos
Servo s[PIN_COUNT];
// Define control mode: 0, independent control;      1, Full control.
int control = 1;

// Define which Servo to control from.
int tmpkey = 0;
bool is_first = true;

//It is controlled by the button on the drive board.
const int buttonPin = 4;

// Define flipper(make the Servo rotate back and forth without returning directly)
int flip = 0;
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // OLED test.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);//Sets the font display color
  display.clearDisplay();//cls
  //Set the font size
  display.setTextSize(2);
  //Set the display location
  display.setCursor(0, 0);
  //String displayed
  display.println(F("OLED OK"));
  //Began to show
  display.display();
  delay(3000);
}

void loop() {
  int timer = 0;
  if (digitalRead(buttonPin) == LOW) {
    while (digitalRead(buttonPin) == LOW) {
      delay(100);
      timer++;
      if (timer >= 20) {
        timer = 20;
      }
    }
    if (timer == 20) {
      // Switch control manual or automatic control mode
      control = 1 - control;
    } else {
      tmpkey++;
      if (tmpkey >= PIN_COUNT) tmpkey = 0;
    }
  }

  if (pos > posMax) {
    // Except that the sequence number of the first key name is not automatically increased, 
    // all the others are automatically increased
    if (is_first) {
      is_first = false;
    } else {
      if (control == 1) tmpkey++;
    }
    if (tmpkey == PIN_COUNT) {
      if (control == 1) detachAll();
      tmpkey = 0;
    }
    flip = 1;

    if (control == 0) {
      attachOne(pin[tmpkey]);
      Serial.print("Individually attached: ");
    } else {
      attachAll();
      Serial.print("All attached : ");
    }
    Serial.print(tmpkey + 1); //The sequence number in the program starts from zero. 
    // Here +1 is used to keep consistent with the servo sequence number in the document.
    
    Serial.print(" (Pin:");
    Serial.print(pin[tmpkey]);
    Serial.print(") Control Model: ");
    Serial.print(control);
    Serial.print(", PIN_COUNT: ");
    Serial.println(PIN_COUNT);

  } else if (pos < posMin) {
    flip = 0;
  } else {
    moveServo(pos);
  }
  if (flip == 0) {
    pos = pos + 5;
  } else {
    pos = pos - 5;
  }
  delay(100);

  // OLED test.
  display.clearDisplay();
  //Set the font size
  display.setTextSize(2);
  //Set the display location
  display.setCursor(0, 0);
  //String displayed
  display.print("S:");
  display.print(tmpkey + 1);
  display.print(",");
  //Display the values of 5 potentiometers
  display.print("A:");
  display.print("0_");
  display.print(analogRead(0));
  display.print(",1_");
  display.print(analogRead(1));
  display.print(",2_");
  display.print(analogRead(2));
  display.print(",3_");
  display.print(analogRead(3));
  display.print(",6_");
  display.print(analogRead(6));

  //Began to show
  display.display();

}

// Associated single pin
void attachOne(int pin) {
  s[tmpkey].attach(pin);
}

// Disassociate a single pin
void detachOne() {
  s[tmpkey].detach();
}

// Associate all pins
void attachAll() {
  for (int i = 0; i < PIN_COUNT; ++i) {
    s[i].attach(pin[i]);
  }
}

// Disassociate all pins
void detachAll() {
  for (int i = 0; i < PIN_COUNT; ++i) {
    s[i].detach();
  }
}

// Drive Servo
void moveServo(int angle) {
  s[tmpkey].write(angle);
}
