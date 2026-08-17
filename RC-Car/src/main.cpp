#include <Arduino.h>
#include <SoftwareSerial.h>

int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;

int stateX = 0;
int stateY = 0;

void forward();
void stop();
void reverse();
void right();
void left();

SoftwareSerial slaveBT(10, 11); // RX, TX

void setup()
{
  slaveBT.begin(9600);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
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
      stateY = 0;
      if (stateX == 0)
      {
        forward();
        stateX = 1;
      }
      else if (stateX == -1)
      {
        stop();
        stateX = 0;
      }
      break;

    case 910:
      stateY = 0;
      if (stateX == 0)
      {
        reverse();
        stateX = -1;
      }
      else if (stateX == 1)
      {
        stop();
        stateX = 0;
      }
      break;

    case 811:
      stateX = 0;
      if (stateY == 0)
      {
        right();
        stateY = 1;
      }
      else if (stateY == -1)
      {
        stop();
        stateY = 0;
      }
      break;

    case 810:
      stateX = 0;
      if (stateY == 0)
      {
        left();
        stateY = -1;
      }
      else if (stateY == 1)
      {
        stop();
        stateY = 0;
      }
      break;
    default:
      stop();
      break;
    }
  }
}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
};

void stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
};
void reverse()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
};

void left()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
};
void right()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
};


