
#include <Arduino.h>
#include <ESPRotary.h>
#include "DFRobotDFPlayerMini.h"
#include "Button2.h"

#define FPSerial Serial1

DFRobotDFPlayerMini myPlayer;
ESPRotary encoder1;

Button2 button;

void printDetail(uint8_t type, int value);

// on change
void rotate(ESPRotary &r)
{
    Serial.println(r.getPosition());
    myPlayer.volume(encoder1.getPosition());
}

// out of bounds event
void upper(ESPRotary &r)
{
    Serial.println("upper bound hit");
}

// out of bounds event
void lower(ESPRotary &r)
{
    Serial.println("lower bound hit");
}

// on left or right rotation
void showDirection(ESPRotary &r)
{
    Serial.println(r.directionToString(r.getDirection()));
}

void handleTap(Button2 &b)
{
    myPlayer.next();
}

void setup()
{
    Serial.begin(115200);

    button.begin(3);
    button.setTapHandler(handleTap);

    encoder1.begin(1, 0, 5, 0, 100, 20, 5);
    encoder1.setChangedHandler(rotate);
    encoder1.setLeftRotationHandler(showDirection);
    encoder1.setRightRotationHandler(showDirection);
    encoder1.setLowerOverflowHandler(lower);
    encoder1.setUpperOverflowHandler(upper);

    FPSerial.begin(9600, SERIAL_8N1, /*rx =*/20, /*tx =*/21);
    myPlayer.begin(FPSerial);

    myPlayer.volume(20);
    myPlayer.EQ(DFPLAYER_EQ_POP);
    myPlayer.randomAll();
}

void loop()
{
    encoder1.loop();

    if (myPlayer.available())
    {
        printDetail(myPlayer.readType(), myPlayer.read()); // Print the detail message from DFPlayer to handle different errors and states.
    }
}

void printDetail(uint8_t type, int value)
{
    switch (type)
    {
    case TimeOut:
        Serial.println(F("Time Out!"));
        break;
    case WrongStack:
        Serial.println(F("Stack Wrong!"));
        break;
    case DFPlayerCardInserted:
        Serial.println(F("Card Inserted!"));
        break;
    case DFPlayerCardRemoved:
        Serial.println(F("Card Removed!"));
        break;
    case DFPlayerCardOnline:
        Serial.println(F("Card Online!"));
        break;
    case DFPlayerUSBInserted:
        Serial.println("USB Inserted!");
        break;
    case DFPlayerUSBRemoved:
        Serial.println("USB Removed!");
        break;
    case DFPlayerPlayFinished:
        Serial.print(F("Number:"));
        Serial.print(value);
        Serial.println(F(" Play Finished!"));
        break;
    case DFPlayerError:
        Serial.print(F("DFPlayerError:"));
        switch (value)
        {
        case Busy:
            Serial.println(F("Card not found"));
            break;
        case Sleeping:
            Serial.println(F("Sleeping"));
            break;
        case SerialWrongStack:
            Serial.println(F("Get Wrong Stack"));
            break;
        case CheckSumNotMatch:
            Serial.println(F("Check Sum Not Match"));
            break;
        case FileIndexOut:
            Serial.println(F("File Index Out of Bound"));
            break;
        case FileMismatch:
            Serial.println(F("Cannot Find File"));
            break;
        case Advertise:
            Serial.println(F("In Advertise"));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
