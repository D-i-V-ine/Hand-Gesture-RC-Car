#include <Arduino.h>
#include <MPU6500.h>

MPU6500 imu;

static int samples = 500;
struct values
{
  float offset = 0;
  float scale = 0;
};

void waitforit()
{
  Serial.println("Press Enter when ready");
  while (Serial.available())
  {
    Serial.read();
  }
  while (!Serial.available())
  {
    delay(10);
  }
  delay(10);
  while (Serial.available())
  {
    Serial.read();
  }
};

float axisAvg(char axis)
{
  double sum = 0;

  for (int i = 0; i < samples; i++)
  {
    imu.update();
    Vec3 a = imu.accelG();
    if (axis == 'x')
    {
      sum += a.x;
    }
    else if (axis == 'y')
    {
      sum += a.y;
    }
    else
    {
      sum += a.z;
    }
    delay(5);
  }
  return (sum / samples);
}

double gx = 0, gy = 0, gz = 0;

void setup()
{
  Serial.begin(115200);
  if (!imu.begin())
  {
    Serial.println("MPU6500 not found");
    while (true)
    {
      delay(100);
    }
  }
  Serial.println("MPU6500 initialized\n");
  
  Serial.println("Keep the MPU6500 stationary \nGyro Bias Calibrating...\n");

  for (int i = 0; i < samples; i++)
  {
    imu.update();

    Vec3 g = imu.gyroDps();
    gx += (g.x);
    gy += (g.y);
    gz += (g.z);
    delay(5);
  }
  
  Serial.println("Gyro Bias : ");
  Serial.print("X : ");
  Serial.print(gx / (float)samples);
  Serial.print(" || Y : ");
  Serial.print(gy / (float)samples);
  Serial.print(" || Z : ");
  Serial.println(gz / (float)samples);
  delay(1000);
  
  
  Serial.println("\nAccelerometer Calibration...");
  
  Serial.println("\nKeep the +x UP...");
  waitforit();
  Serial.print("X : ");
  float posX = axisAvg('x');
  Serial.println(posX, 6);
  delay(500);

  Serial.println("\nKeep the +y UP...");
  waitforit();
  Serial.print("Y : ");
  float posY = axisAvg('y');
  Serial.println(posY, 6);
  delay(500);

  Serial.println("\nKeep the +z UP...");
  waitforit();
  Serial.print("Z : ");
  float posZ = axisAvg('z');
  Serial.println(posZ, 6);
  delay(500);

  Serial.println("\nKeep the -x UP...");
  waitforit();
  Serial.print("-x : ");
  float negX = axisAvg('x');
  Serial.println(negX, 6);
  delay(500);

  Serial.println("\nKeep the -y UP...");
  waitforit();
  Serial.print("-y : ");
  float negY = axisAvg('y');
  Serial.println(negY, 6);
  delay(500);
  
  Serial.println("\nKeep the -z UP...");
  waitforit();
  Serial.print("-z : ");
  float negZ = axisAvg('z');
  Serial.println(negZ, 6);
  delay(500);

  values x, y, z;
  x.offset = (posX + negX) / 2.0;
  y.offset = (posY + negY) / 2.0;
  z.offset = (posZ + negZ) / 2.0;

  x.scale = 2.0 / (posX - negX);
  y.scale = 2.0 / (posY - negY);
  z.scale = 2.0 / (posZ - negZ);

  Serial.println("\n\nacceleration offset and scale : \n");
  Serial.println("offset : ");
  Serial.print("x : ");
  Serial.print(x.offset, 6);
  Serial.print(" || y : ");
  Serial.print(y.offset, 6);
  Serial.print(" || z : ");
  Serial.println(z.offset, 6);

  Serial.println("\nscale : ");
  Serial.print("x : ");
  Serial.print(x.scale, 6);
  Serial.print(" || y : ");
  Serial.print(y.scale, 6);
  Serial.print(" || z : ");
  Serial.println(z.scale, 6);

}

void loop()
{
}
