#include <XBOXONE.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXONE Xbox(&Usb);

int valX = 0;
int valY = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXBOX USB Library Started"));

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}
void loop() {
  Usb.Task();

  valX = 0;
  valY = 0;
  
  if (Xbox.XboxOneConnected) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        int t = Xbox.getAnalogHat(LeftHatX);
        if (t > 0){
          valX = map(t, 7512, 32768, 0, 256);
          digitalWrite(2, HIGH);
        } else {
          valX = map(t, -7512, -32768, 0, 255);
          digitalWrite(4, HIGH);
        }  
        Serial.println(valX);      
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        int q = Xbox.getAnalogHat(LeftHatY);
        if (q > 0) {
          valY = map(q, 7512, 32768, 0, 256);
          digitalWrite(7, HIGH);
        } else {
          valY = map(q, -7512, -32768, 0, 255);
          digitalWrite(8, HIGH);
        }
        Serial.println(valY);
      }
      Serial.println();
    }

    if (Xbox.getButtonPress(L2) > 0 || Xbox.getButtonPress(R2) > 0) {
      if (Xbox.getButtonPress(L2) > 0) {
        Serial.print(F("L2: "));
        Serial.print(Xbox.getButtonPress(L2));
        Serial.print("\t");
      }
      if (Xbox.getButtonPress(R2) > 0) {
        Serial.print(F("R2: "));
        Serial.print(Xbox.getButtonPress(R2));
        Serial.print("\t");
      }
      Serial.println();
    }

    if (Xbox.getButtonClick(L1))
      Serial.println(F("L1"));
    if (Xbox.getButtonClick(R1))


    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
  }

  if (valX == 0) {
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
  }

  if (valY == 0) {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }

  analogWrite(6, valY);
  analogWrite(9, valX);
}
