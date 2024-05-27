#include <AFMotor.h>
#include <math.h>

#define speed 100
#define delay_0 500
#define delay_1 1000


AF_DCMotor engage(1, MOTOR12_8KHZ);
AF_DCMotor driver_1(2, MOTOR12_8KHZ);
AF_DCMotor driver_2(3, MOTOR12_8KHZ);
AF_DCMotor driver_3(4, MOTOR12_8KHZ);

static int scalar = 0;
void setup() {
  // put your setup code here, to run once:
  engage.setSpeed(60);
  driver_1.setSpeed(60);
  driver_2.setSpeed(60);
  driver_3.setSpeed(60);
  Serial.begin(9600);
  Serial.println("Motor test!");
}

void loop() {
  // put your main code here, to run repeatedly:
  scalar = 200;
  roll_angle(60);
}

// Use scalar to control speed. Set roof and floor to ensure motor ratios are consistent
void roll_speed (double ratio_1, double ratio_2, double ratio_3) {
  double motorSpeeds[3];
  // Normalize motor contributions to match the desired speed
  double sumContributions = abs(ratio_1) + abs(ratio_2) + abs(ratio_3);
  motorSpeeds[0] = scalar * (ratio_1 / sumContributions);
  motorSpeeds[1] = scalar * (ratio_2 / sumContributions);
  motorSpeeds[2] = scalar * (ratio_3 / sumContributions);

  // probs change speed to a global variable
  driver_1.setSpeed(motorSpeeds[0]);
  driver_2.setSpeed(motorSpeeds[1]);
  driver_3.setSpeed(motorSpeeds[2]);

  driver_1.run(FORWARD);
  driver_2.run(FORWARD);
  driver_3.run(FORWARD);

  // if (ratio_1 < 0) {
  //   driver_1.run(BACKWARD);
  // } else {
  //   driver_1.run(FORWARD);
  // }
  // if (ratio_2 < 0) {
  //   driver_2.run(BACKWARD);
  // } else {
  //   driver_2.run(FORWARD);
  // }
  // if (ratio_3 < 0) {
  //   driver_3.run(BACKWARD);
  // } else {
  //   driver_3.run(FORWARD);
  // }
}

// Calls roll_speed with percentages as arguments
// Calculates percentage of each motor required for a direction
void roll_angle(uint8_t angle) {
  Serial.println("Test");
  int a = 0;
  int b = 0;
  int c = 0;

  double x = 0;
  double y = 0;

  x = cos(angle * M_PI / 180.0);
  y = sin(angle * M_PI / 180.0);

  // Motor positions in degrees
  double motorA_deg = 90;
  double motorB_deg = 150;
  double motorC_deg = 30;

  // Convert degrees to radians
  double motorX_rad = motorA_deg * M_PI / 180.0;
  double motorY_rad = motorB_deg * M_PI / 180.0;
  double motorZ_rad = motorC_deg * M_PI / 180.0;
  double desiredAngle_rad = angle * M_PI / 180.0;

  // Force vectors for each motor (in Cartesian coordinates)
  double Fx[] = { cos(motorX_rad), cos(motorY_rad), cos(motorZ_rad) };
  double Fy[] = { sin(motorX_rad), sin(motorY_rad), sin(motorZ_rad) };

  // Calculate contributions for each motor to move in the desired direction
  double A = Fx[0] * x + Fy[0] * y;
  double B = Fx[1] * x + Fy[1] * y;
  double C = Fx[2] * x + Fy[2] * y;

  roll_speed (A, B, C);
}

void run_engage(void) {
  //put your main code here, to run repeatedly:
  engage.run(FORWARD);
  Serial.println("Forward!");
  delay(delay_0);
  engage.setSpeed(0);
  delay(delay_1);
  engage.setSpeed(speed);
  engage.run(BACKWARD);
  Serial.println("Backward!");
  delay(delay_0);
  engage.setSpeed(0);
  delay(delay_1);
  engage.setSpeed(speed);
}