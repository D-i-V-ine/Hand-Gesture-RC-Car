#include <Arduino.h>
#include <MPU6500.h>
#include <SoftwareSerial.h>
#include <MadgwickAHRS.h>

Madgwick filter;
class LPF
{
public:
    float alpha = 0.2;
    float filteredValue = 0;

    float lowPassFilter(float value)
    {
        filteredValue = alpha * filteredValue + (1 - alpha) * value;
        return filteredValue;
    }
};

LPF lpfRoll, lpfPitch, lpfYaw;

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;

float roll, pitch, yaw;

unsigned long lastTime = 0;

MPU6500 imu;

SoftwareSerial masterBT(10, 11); // RX, TX

void setup()
{
    Serial.begin(115200);
    masterBT.begin(9600);
    filter.begin(100); // Initialize the Madgwick filter with a sample rate of 100 Hz

    // Initialize and verify MPU6500
    if (!imu.begin())
    {
        Serial.println("Failed to initialize MPU6500");
        while (true)
        {
            delay(1000);
        }
    }
    Serial.println("MPU6500 initialized");
    imu.setSampleRateHz(100);
}
void loop()
{
    if (millis() - lastTime >= 10)
    {
        lastTime += 10;
        imu.update();
        Vec3 acc = imu.accelG();
        Vec3 gyro = imu.gyroDps();

        accX = (acc.x - 0.015720) * 1.001828;
        accY = (acc.y - 0.013856) * 0.999790;
        accZ = (acc.z - (-0.034943)) * 1.001411;

        gyroX = gyro.x - (-0.10);
        gyroY = gyro.y - 5.94;
        gyroZ = gyro.z - 1.17;

        filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
        roll = lpfRoll.lowPassFilter(filter.getRoll());

        pitch = lpfPitch.lowPassFilter(filter.getPitch());

        float finalRoll = constrain(roll, -50, 50);
        float finalPitch = constrain(pitch, -50, 50);

        masterBT.print(finalRoll);
        masterBT.print(",");
        masterBT.print(finalPitch);

        Serial.print(finalRoll);
        Serial.print(",");
        Serial.println(finalPitch);
    }
}