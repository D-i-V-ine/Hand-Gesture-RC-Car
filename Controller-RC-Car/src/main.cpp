#include <Arduino.h>
#include <MPU6500.h>
#include <SoftwareSerial.h>
#include <MadgwickAHRS.h>

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

// Sensor and communication objects
MPU6500 imu;
Madgwick filter;
SoftwareSerial masterBT(10, 11);
LPF lpfRoll, lpfPitch;

// Timing and status
unsigned long lastTime = 0;
const int redLed = 3;
int sendCounter = 0;

void setup()
{
    pinMode(redLed, OUTPUT);

    masterBT.begin(38400);

    // Stop execution if the IMU cannot be initialized
    if (!imu.begin())
    {
        digitalWrite(redLed, HIGH);
        while (true)
        {
            delay(1000);
        }
    }

    Serial.begin(115200);

    // Configure the IMU and Madgwick filter for 100 Hz
    filter.begin(100);
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

        // Apply accelerometer calibration
        float accX = (acc.x - 0.015720) * 1.001828;
        float accY = (acc.y - 0.013856) * 0.999790;
        float accZ = (acc.z - (-0.034943)) * 1.001411;

        // Apply gyroscope bias correction
        float gyroX = gyro.x - (-0.10);
        float gyroY = gyro.y - 5.94;
        float gyroZ = gyro.z - 1.17;

        // Estimate orientation from the calibrated IMU data
        filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);

        // Smooth the estimated orientation
        float roll = lpfRoll.lowPassFilter(filter.getRoll());
        float pitch = lpfPitch.lowPassFilter(filter.getPitch());

        // Limit the orientation range used for gestures
        float finalRoll = constrain(roll, -50.0f, 50.0f);
        float finalPitch = constrain(pitch, -50.0f, 50.0f);

        sendCounter++;

        // Transmit orientation data at 20 Hz
        if (sendCounter >= 5)
        {
            sendCounter = 0;

            masterBT.print(finalRoll, 2);
            masterBT.print(",");
            masterBT.println(finalPitch, 2);

            Serial.print("SENT: ");
            Serial.print(finalRoll);
            Serial.print(",");
            Serial.println(finalPitch);
        }
    }
}