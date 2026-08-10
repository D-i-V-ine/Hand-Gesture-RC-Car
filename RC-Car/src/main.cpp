#include <Arduino.h>
#include <SoftwareSerial.h>

int frontLeft = 1;
int frontRight = 2;
int backLeft = 3;
int backRight = 4;

int stateX = 0;
int stateY = 0;

SoftwareSerial slaveBT(10, 11); // RX, TX
void setup()
{
  slaveBT.begin(9600);
  pinMode(frontLeft, OUTPUT);
  pinMode(frontRight, OUTPUT);
  pinMode(backLeft, OUTPUT);
  pinMode(backRight, OUTPUT);
}

void loop()
{
  if (slaveBT.available())
  {
    String command = slaveBT.readStringUntil('\n');
    int input = command.toInt(); // Read the incoming data from the Bluetooth serial port

    switch (input)
    {
    case 911:
      if (stateX == 0)
      {
        stateX = 1;
      }
      else if (stateX == -1)
      {
        stateX = 0;
      }
      break;

    case 910:
      if (stateX == 0)
      {
        stateX = -1;
      }
      else if (stateX == 1)
      {
        stateX = 0;
      }
      break;

    case 811:
      if (stateY == 0)
      {
        stateY = 1;
      }
      else if (stateY == -1)
      {
        stateY = 0;
      }
      break;

    case 810:
      if (stateY == 0)
      {
        stateY = -1;
      }
      else if (stateY == 1)
      {
        stateY = 0;
      }
      break;
      
    default:
      break;
    }
  }
}