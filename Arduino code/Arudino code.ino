/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

int value = 0;
int offsetRight = 0;
int offsetLeft = 0;

int leftPin = 3;
int rightPin = 5;

int left1 = 2;
int left2 = 4;
int right1 = 7;
int right2 = 6;

bool orientation = true;

USB Usb;
XBOXRECV Xbox(&Usb);

Servo ESC;
int motorSpeed = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 2; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

ESC.attach(71', 1000, 2000);
  
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
}
void loop() {
  Usb.Task();
  value = 0;
  offsetRight = 0;
  offsetLeft = 0;
  motorSpeed = 0;
  
  if (Xbox.XboxReceiverConnected) {
      if (Xbox.Xbox360Connected) {
        if (Xbox.getButtonPress(L2)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2));
          Serial.println();

          value = Xbox.getButtonPress(L2);

          if (orientation) {
            turn_left_motor_forward();
            turn_right_motor_forward();
          } else {
            turn_left_motor_backward();
            turn_right_motor_backward();
          }         
        }

        if (Xbox.getButtonPress(R2)) {
          Serial.print("R2: ");
          Serial.print(Xbox.getButtonPress(R2));
          Serial.println();

          value = Xbox.getButtonPress(R2);

          if (orientation) {
            turn_left_motor_backward();
            turn_right_motor_backward();  
          } else {
            turn_left_motor_forward();
            turn_right_motor_forward();
          }
        }

        if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
          Serial.print(F("LeftHatX: "));
          Serial.print(Xbox.getAnalogHat(LeftHatX));
          Serial.print("\t");
          Serial.println();

          int x = Xbox.getAnalogHat(LeftHatX);

          if (x > 0) {
              if (orientation) {
                offsetRight = map(x, 7512, 32768, 0, 255); 
              } else {
                offsetLeft = map(x, -7512, -32768, 0, 255);
              }
          } 

          if (x < 0) {
            if (orientation) {
              offsetLeft = map(x, -7512, -32768, 0, 255);
            } else {
              offsetRight = map(x, 7512, 32768, 0, 255);
            } 
          }
        }

        if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
          int val = Xbox.getAnalogHat(RightHatX);
          motorSpeed = map(val, 7512, 32768, 0, 180);
        }

        if (Xbox.getButtonClick(X)) {
          Serial.println("X");
          value = 255;
          turn_left_motor_backward();
          turn_right_motor_forward();
        }

        if (Xbox.getButtonClick(Y)) {
          orientation = !orientation;
        }

        analogWrite(leftPin, value - offsetLeft);
        analogWrite(rightPin, value - offsetRight);
        ESC.write(motorSpeed);
      }
    }
}

void turn_left_motor_forward() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void turn_left_motor_backward() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
}

void turn_right_motor_forward() {
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}

void turn_right_motor_backward() {
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}
