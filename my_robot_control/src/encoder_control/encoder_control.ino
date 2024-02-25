#include <ros.h>
// #include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>


// Define motor control pins
const int motorPWM = 3;  // PWM pin for controlling motor speed
const int motorDir1 = 4;  // Direction pin 1
const int motorDir2 = 5;  // Direction pin 2

// Initialize ROS node
ros::NodeHandle nh;

// Define callback function to handle incoming motor commands
void motorCallback(const geometry_msgs::Twist& msg) {
  // Extract motor command from message
  float linearVelocity = msg.linear.x;
  float angularVelocity = msg.angular.z;

  // Determine motor direction based on sign of motorSpeed
  if (linearVelocity >= 0) {
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
  } else {
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
  }

  // Map motorSpeed from [-1, 1] to [0, 255] for PWM control
  int pwmValue = int(abs(linearVelocity) * 255);
  analogWrite(motorPWM, pwmValue);
}

// Initialize ROS subscriber
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &motorCallback);

void setup() {
  // Initialize serial communication
  nh.initNode();

  // Set motor control pins as outputs
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);

  // Subscribe to cmd_vel topic
  nh.subscribe(sub);
}

void loop() {
  // Handle ROS communication
  nh.spinOnce();
}