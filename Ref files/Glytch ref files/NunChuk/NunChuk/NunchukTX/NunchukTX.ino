#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>

ArduinoNunchuk nunchuk = ArduinoNunchuk();


const uint64_t pipeOut = 0xE8E8F0F0E1LL;
RF24 radio(10, 9);

int Chan1 = 127;
int Chan2 = 127;
int Chan3 = 127;
int Chan4 = 127;
int KillSwitch = 1;

struct MyData {
  byte KillSwitch;
  byte Chan1;
  byte Chan2;
  byte Chan3;
  byte Chan4;
};

void setup()
{
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);

  Serial.begin(250000);
  nunchuk.init();
}


MyData data;

void killswitch() {
  if (nunchuk.cButton == 1) {
    KillSwitch = 0;
  }
  else {
    KillSwitch = 1;
  }
}


void loop()
{
  nunchuk.update();
  killswitch();
  joystick();
  debug();
  data.Chan1 = Chan1;
  data.Chan2 = Chan2;
  data.Chan3 = Chan3;
  data.Chan4 = Chan4;
  data.KillSwitch = KillSwitch;
  radio.write(&data, sizeof(MyData));
}

void joystick() {
  if (nunchuk.zButton == 0) {
    Chan1 = map(nunchuk.analogX, 28, 240, 197, 57);
    Chan2 = map(nunchuk.analogY, 34, 228, 197, 57);
  }
  else if (nunchuk.zButton == 1) {
    Chan1 = map(nunchuk.analogX, 28, 240, 255, 0);
    Chan2 = map(nunchuk.analogY, 34, 228, 255, 0);
  }
}

void debug() {
  Serial.println("Base: ");
  Serial.print("KillSwitch: ");
  Serial.print(KillSwitch);
  Serial.print(" TurboSwitch: ");
  Serial.println(nunchuk.zButton);
  Serial.print("LR: ");
  Serial.print(Chan1);
  Serial.print(" FR: ");
  Serial.println(Chan2);
  Serial.print("RawJoyX: ");
  Serial.print(nunchuk.analogX);
  Serial.print(" RawJoyY: ");
  Serial.println(nunchuk.analogY);

  Serial.println("=========================================");
  delay(20);
}
