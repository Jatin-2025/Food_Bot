#include <ros.h>
#include <geometry_msgs/Twist.h>

// Define motor control pins for Cytron motor driver
const int pwm1Pin = 3;  // PWM pin for motor 1 speed
const int dir1Pin = 4;  // Direction pin for motor 1

const int pwm2Pin = 5;  // PWM pin for motor 2 speed
const int dir2Pin = 6;  // Direction pin for motor 2

// Initialize ROS node
ros::NodeHandle nh;

// Define callback function to handle incoming motor commands
void motorCallback(const geometry_msgs::Twist& msg) {
  // Extract linear and angular velocities from Twist message
  float linearVelocity = msg.linear.x;
  float angularVelocity = msg.angular.z;

  // Determine motor directions based on sign of linearVelocity and angularVelocity
  if (linearVelocity >= 0) {
    digitalWrite(dir1Pin, HIGH);
    digitalWrite(dir2Pin, HIGH);
  } else {
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, LOW);
  }

  // Map linearVelocity from [-1, 1] to [0, 255] for PWM control
  int pwmValue = int(abs(linearVelocity) * 255);

  // Map angularVelocity from [-1, 1] to adjust differential drive
  int angularAdjust = int(angularVelocity * 255);

  // Adjust PWM values for each motor based on the angular velocity
  int pwm1Value = pwmValue - angularAdjust;
  int pwm2Value = pwmValue + angularAdjust;

  // Ensure PWM values are within the valid range
  pwm1Value = constrain(pwm1Value, 0, 255);
  pwm2Value = constrain(pwm2Value, 0, 255);

  // Control motor speeds using PWM
  analogWrite(pwm1Pin, pwm1Value);
  analogWrite(pwm2Pin, pwm2Value);
}

// Initialize ROS subscriber
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &motorCallback);

void setup() {
  // Initialize serial communication
  nh.initNode();

  // Set motor control pins as outputs
  pinMode(pwm1Pin, OUTPUT);
  pinMode(dir1Pin, OUTPUT);
  pinMode(pwm2Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);

  // Subscribe to cmd_vel topic
  nh.subscribe(sub);
}

void loop() {
  // Handle ROS communication
  nh.spinOnce();
}
