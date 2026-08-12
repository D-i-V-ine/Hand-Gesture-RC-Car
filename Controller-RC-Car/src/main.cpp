//---------------------------------------//
//  All the comments work is done by AI  //
//---------------------------------------//


#include <Arduino.h>
#include <MPU6500.h>
#include <SoftwareSerial.h>

// Low-pass filter for smoothing gyro readings
class Filter
{
public:
    float prevValue = 0;
    float alpha = 0.2;

    float lowPassFilter(float currentVlaue) // Low pass filter function
    {
        float filteredVlaue = (alpha * prevValue) + (1 - alpha) * currentVlaue;
        prevValue = filteredVlaue;
        return filteredVlaue;
    };
};

// IMU, filters and Bluetooth interface
MPU6500 imu;
Filter filterX, filterY, filterZ;
SoftwareSerial masterBT(10, 11); // RX, TX

int moveCmd(float value);

void setup()
{
    Serial.begin(115200);
    masterBT.begin(9600);

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
}

void loop()
{
    // Read current gyro data
    imu.update();
    Vec3 gyro = imu.gyroDps();

    // Filter gyro data and convert it into movement commands
    int X = moveCmd(filterX.lowPassFilter(gyro.x));
    int Y = moveCmd(filterY.lowPassFilter(gyro.y));

    // X-axis commands: 911 / 910
    if ((X != 0) && (Y == 0))
    {
        masterBT.println(900 + X);
        Serial.println(900 + X);
    }

    // Y-axis commands: 811 / 810
    if ((Y != 0) && (X == 0))
    {
        masterBT.println(800 + Y);
        Serial.println(800 + Y);
    }
    delay(100);
}

// Convert gyro direction into a 2-digit movement state.
//
// 1 = motor ON / movement
// 0 = motor OFF / stop
//
// The extra leading 1 is added to distinguish these movement states
// from normal Boolean values (0/1):
//
//     11 -> ON  (positive gyro direction)
//     10 -> OFF (negative gyro direction / stop)
//
// The encoded value is later combined with the axis identifier (900,800)
// to form the final Bluetooth command.
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