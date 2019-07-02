#include <XBOXRECV.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

USB Usb;
XBOXRECV Xbox(&Usb);
Servo leftESC;
Servo rightESC;
Servo weaponESC;

int motorSpeedLeft = 1450;
int motorSpeedRight = 1450;
int motorSpeedWeapon = 0;
int rightOffset = 0;
int leftOffset = 0;
int val = 0;

int x;
int y;

void setup() {
    Serial.begin(9600);

    pinMode(3, OUTPUT);

    weaponESC.attach(7, 1000, 2000);
    
    pinMode(4, OUTPUT);
    pinMode(7, OUTPUT);

    leftESC.attach(3);
    rightESC.attach(4);
    
#if !defined(_MIPSEL_)
    while (!Serial);
#endif

    if (Usb.Init() == -1) {
        Serial.print(F("\r\nOSC did not start"));
        while (1); //halt
    }
}

void loop() {
    Usb.Task();

    motorSpeedLeft = 1450;
    motorSpeedRight = 1450;
    motorSpeedWeapon = 0;
    leftOffset = 0;
    rightOffset = 0;
    val = 0;

    if (Xbox.XboxReceiverConnected) {
        if (Xbox.Xbox360Connected) {
            x = Xbox.getAnalogHat(LeftHatX);
            y = Xbox.getAnalogHat(RightHatY);

            if (Xbox.getButtonPress(L2)) {
                val = Xbox.getButtonPress(L2);
                motorSpeedLeft = map(val, 0, 255, 1450, 2000);
                motorSpeedRight = map(val, 0, 255, 1450, 2000);
            }

            if (Xbox.getButtonPress(R2)) {
                val = Xbox.getButtonPress(R2);
                motorSpeedLeft = map(val, 0, 255, 1450, 1000);
                motorSpeedRight = map(val, 0, 255, 1450, 1000);
            }

            if (x > 7500) {
                leftOffset = map(x, 7512, 32768, 0, 500);
            }

            if (x < -7500) {
                rightOffset = map(x, -7512, -32768, 0, 500);
            }

            if (y > 7500) {
                motorSpeedWeapon = map(y, 7512, 32768, 0, 180);
            }
            
            if (motorSpeedLeft >= 1450) {
                leftESC.writeMicroseconds(motorSpeedLeft - rightOffset);
                rightESC.writeMicroseconds(motorSpeedRight - leftOffset);
            }

            if (motorSpeedLeft <= 1450) {
                leftESC.writeMicroseconds(motorSpeedLeft + rightOffset);
                rightESC.writeMicroseconds(motorSpeedRight + leftOffset);
            }

            weaponESC.write(motorSpeedWeapon);
        }
    }   
}
