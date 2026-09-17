# MPU6500 Calibration Tool

Arduino utility for calibrating an MPU6500 IMU.

The sketch measures **gyroscope bias** and calculates **accelerometer offset and scale factors** using a six-position calibration routine.

## Requirements

- Arduino-compatible board
- MPU6500
- Arduino IDE
- Compatible `MPU6500` library

## Setup

Connect the MPU6500 to your Arduino and upload the sketch.

Open the Serial Monitor at:

```text
115200 baud
```

Keep the sensor stationary when the gyro calibration starts.

## Calibration

The calibration runs automatically after initialization.

### Gyroscope

Keep the MPU6500 completely stationary while the program collects the samples.

The resulting gyro bias is printed to the Serial Monitor.

### Accelerometer

The program will prompt you to place the sensor in six orientations:

```text
+x UP
+y UP
+z UP
-x UP
-y UP
-z UP
```

Press **Enter** after positioning the sensor as requested.

The program collects `500` samples for each position.

## Output

At the end of the calibration, the calculated values are printed:

```text
Gyro Bias :
X : ...
Y : ...
Z : ...

acceleration offset and scale :

offset :
x : ...
y : ...
z : ...

scale :
x : ...
y : ...
z : ...
```

Save these values and use them in your main IMU application.

## Configuration

The default sample count is:

```cpp
static int samples = 500;
```

Adjust it if you want to trade calibration time for additional averaging.

## Notes

- Keep the sensor still during gyro calibration.
- Use a stable, level surface for the accelerometer positions.
- Make sure each requested axis is pointing directly upward.
- Repeat the calibration if the measurements are inconsistent.
- The sketch currently outputs the calibration values over Serial; it does not store them in EEPROM or other non-volatile memory.

## Calibration Type

| Sensor        | Calibration    |
| ------------- | -------------- |
| Gyroscope     | Bias           |
| Accelerometer | Offset + Scale |

This is a basic six-position calibration and does not account for temperature drift, axis misalignment, or cross-axis errors.
