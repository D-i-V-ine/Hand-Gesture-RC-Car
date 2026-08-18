#include <Arduino.h>
#include <MPU6500.h>
#include <SoftwareSerial.h>
#include <MadgwickAHRS.h>

class LPF
{
public:
    float alpha = 0.2; // Higher = smoother, but more lag
    float filteredValue = 0;

    float lowPassFilter(float value)
    {
        filteredValue = alpha * filteredValue + (1 - alpha) * value;
        return filteredValue;
    }
};

MPU6500 imu;                     // IMU object
Madgwick filter;                 // Sensor fusion: gyro + accelerometer
SoftwareSerial masterBT(10, 11); // RX, TX
LPF lpfRoll, lpfPitch;           // Smooth final orientation

unsigned long lastTime = 0; // Used for 100 Hz timing
const int redLed = 3;
int sendCounter = 0;
void setup()
{
    pinMode(redLed, OUTPUT);

    masterBT.begin(38400); // HC-05 baud rate

    if (!imu.begin())
    {
        digitalWrite(redLed, HIGH);
        while (true)
        {
            delay(1000);
        }
    }
    Serial.begin(115200);
    filter.begin(100);        // Madgwick update rate
    imu.setSampleRateHz(100); // IMU sample rate
}

void loop()
{
    if (millis() - lastTime >= 10) // Run at ~100 Hz
    {
        lastTime += 10;

        imu.update();

        Vec3 acc = imu.accelG();   // Acceleration in g
        Vec3 gyro = imu.gyroDps(); // Gyroscope in °/s

        // Accelerometer calibration
        float accX = (acc.x - 0.015720) * 1.001828;
        float accY = (acc.y - 0.013856) * 0.999790;
        float accZ = (acc.z - (-0.034943)) * 1.001411;

        // Gyroscope bias correction
        float gyroX = gyro.x - (-0.10);
        float gyroY = gyro.y - 5.94;
        float gyroZ = gyro.z - 1.17;

        filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);

        // Madgwick → LPF
        float roll = lpfRoll.lowPassFilter(filter.getRoll());
        float pitch = lpfPitch.lowPassFilter(filter.getPitch());

        // Limit the orientation range used for gestures
        int finalRoll = (int)constrain(roll, -50, 50);
        int finalPitch = (int)constrain(pitch, -50, 50);

        sendCounter++;
        if (sendCounter >= 5)
        {
            sendCounter = 0;

            masterBT.print(finalRoll, 2);
            masterBT.print(",");
            masterBT.println(finalPitch, 2);

            Serial.print("SENT: ");
            Serial.print(finalRoll, 2);
            Serial.print(",");
            Serial.println(finalPitch, 2);
        }
    }
}