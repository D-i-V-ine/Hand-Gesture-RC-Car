#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial slaveBT(10, 11); // RX, TX

int ENA = 3;
int IN1 = 4;
int IN2 = 12;

int ENB = 5;
int IN3 = 7;
int IN4 = 8;

unsigned long lastTime = 0;

void setup()
{
  Serial.begin(115200);
  slaveBT.begin(38400);
  slaveBT.setTimeout(20);

  while (slaveBT.available())
  {
    slaveBT.read();
  }
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop()
{
  if (slaveBT.available())
  {
    String input = slaveBT.readStringUntil('\n');

    int comma = input.indexOf(',');

    if (comma != -1)
    {
      int roll = input.substring(0, comma).toInt();
      int pitch = input.substring(comma + 1).toInt();

      lastTime = millis();
      int rollMotorIp;
      int pitchMotorIp;

      if (abs(roll) < 9)
      {
        rollMotorIp = 0;
      }
      else
      {
        rollMotorIp = constrain(map(roll, -50, 50, -255, 255), -255, 255);
      }
      if (abs(pitch) < 9)
      {
        pitchMotorIp = 0;
      }
      else
      {
        pitchMotorIp = constrain(map(pitch, -50, 50, -255, 255), -255, 255);
      }

      Serial.print("Roll: ");
      Serial.println(rollMotorIp);

      Serial.print("Pitch: ");
      Serial.println(pitchMotorIp);

      if (abs(rollMotorIp) + 15 < abs(pitchMotorIp) )
      {
        analogWrite(ENA, abs(pitchMotorIp));
        analogWrite(ENB, abs(pitchMotorIp));
        // right turn
        if (pitchMotorIp > 0)
        {

          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
        // left turn
        else
        {
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        }
      }
      else if (abs(rollMotorIp) > abs(pitchMotorIp)  + 15)
      {
        analogWrite(ENA, abs(rollMotorIp));
        analogWrite(ENB, abs(rollMotorIp));
        if (rollMotorIp > 0)
        {
          // Forward
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
        else
        {
          // Reverse
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        }
      }

      else
      {
        analogWrite(ENA, 0);
        analogWrite(ENB, 0);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }
    }
  }
  if (millis() - lastTime > 100)
  {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(100);
  }
}