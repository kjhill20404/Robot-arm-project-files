#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

int pwm_width_2 = 0;
int pwm_width_3 = 0;
int pwm_width_4 = 0;
int pwm_width_5 = 0;
int pwm_width_6 = 0;
int pwm_width_7 = 0;
int pwm_width_8 = 0;
int pwm_width_9 = 0;
int KillSwitch;

Servo PWM2;
Servo PWM3;
Servo PWM4;
Servo PWM5;
Servo PWM6;
Servo PWM7;

struct MyData {
  byte KillSwitch;
  byte Chan1;
  byte Chan2;
  byte Chan3;
  byte Chan4;
};

MyData data;

const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

void resetData()
{
  data.Chan1 = 127;
  data.Chan2 = 127;
  data.Chan3 = 127;
  data.Chan4 = 127;
  Serial.println("Data Reset");
}

void setup()
{
  PWM2.attach(2);
  PWM3.attach(3);
  PWM4.attach(4);
  PWM5.attach(5);
  PWM6.attach(6);
  PWM7.attach(7);

  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);

  radio.startListening();
  Serial.begin(115200);

}

unsigned long lastRecvTime = 0;
void recvData()
{
  while ( radio.available() ) {
    radio.read(&data, sizeof(MyData));
    lastRecvTime = millis();
  }
}

void loop(){
  KillSwitch = data.KillSwitch;
  recvData();
  unsigned long now = millis();
  if ( now - lastRecvTime > 100 ) {
    resetData();
  }
  if (KillSwitch == 1) {
    resetData();
  }

  pwm_width_2 = map(data.Chan1, 0, 255, 1000, 2000);
  pwm_width_3 = map(data.Chan2, 0, 255, 1000, 2000);
  pwm_width_4 = map(data.Chan3, 0, 255, 2000, 1000);
  pwm_width_5 = map(data.Chan4, 0, 255, 1000, 2000);
  //pwm_width_6 = map(data.FR, 0, 255, 1000, 2000);
  //pwm_width_7 = map(data.FR, 0, 255, 1000, 2000);


  PWM2.writeMicroseconds(pwm_width_2);
  PWM3.writeMicroseconds(pwm_width_3);
  PWM4.writeMicroseconds(pwm_width_4);
  PWM5.writeMicroseconds(pwm_width_5);
  //PWM6.writeMicroseconds(pwm_width_2);
  //PWM7.writeMicroseconds(pwm_width_2);

  debug();
}


void debug() {
  Serial.println("RX: ");
  Serial.print("KillSwitch: ");
  Serial.println(KillSwitch);
  Serial.print("Chan1: ");
  Serial.println(data.Chan1);
  Serial.print("Chan2: ");
  Serial.println(data.Chan2);
  Serial.print("Chan3: ");
  Serial.println(data.Chan3);
  Serial.print("Chan4: ");
  Serial.println(data.Chan4);

  Serial.println("========================");
  delay(10);

}
