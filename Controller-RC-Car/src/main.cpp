#include <Arduino.h>
#include <MPU6500.h>
#include <SoftwareSerial.h>

class Filter
{
public:
    float prevValue = 0;
    float alpha = 0.2;

    float lowPassFilter(float currentVlaue) // Low pass filter function
    {
        float filteredVlaue = (alpha * prevValue) + (1 - alpha) * currentVlaue; // Calculate the filtered value
        prevValue = filteredVlaue;                                              // Update the prevValue for the next iteration
        return filteredVlaue;
    };
};

MPU6500 imu;                      // Create an instance of the MPU6500 class
Filter filterX, filterY, filterZ; // Create an instance of the Filter class
SoftwareSerial masterBT(10, 11);  // RX, TX

int moveCmd(float value);

void setup()
{
    Serial.begin(115200); // Initialize the serial communication
    masterBT.begin(9600); // Initialize the serial communication for Bluetooth

    // Check if the MPU6500 is connected
    if (!imu.begin())
    {
        Serial.println("Failed to initialize MPU6500");
        while (true)
        {
            delay(1000);
        }
    }
    Serial.println("MPU6500 initialized");
}

void loop()
{
    imu.update();
    Vec3 gyro = imu.gyroDps();

    int X = moveCmd(filterX.lowPassFilter(gyro.x));
    int Y = moveCmd(filterY.lowPassFilter(gyro.y));
    // int Z = moveCmd(filterZ.lowPassFilter(gyro.z));

    if ((X != 0) && (Y == 0))
    {
        masterBT.println(900 + X);
        Serial.println(900 + X);
    }

    if ((Y != 0) && (X == 0))
    {
        masterBT.println(800 + Y);
        Serial.println(800 + Y);
    }
    // masterBT.print(moveCmd(filterZ.lowPassFilter(gyro.z)));
    delay(100);
}

int moveCmd(float value)
{
    if (value > 90)
    {
        return 11;
    }
    else if (value < -90)
    {
        return 10;
    }
    else
    {
        return 0;
    }
}