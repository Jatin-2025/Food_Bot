#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

class JoystickControl
{
private:
    ros::NodeHandle joy_nh;
    ros::Publisher twist_pub;
    ros::Subscriber joy_sub;

    bool enable = true;
    geometry_msgs::Twist twist_msg;

public:
    JoystickControl()
    {
        joy_sub = joy_nh.subscribe("/joy", 1, &JoystickControl::joy_callback, this);
        twist_pub = joy_nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    }

    void joy_callback(const sensor_msgs::Joy &msg)
    {
        bool button_pressed = msg.buttons[1]; // Adjust index as needed

        if (enable && button_pressed)
        {
            twist_msg.linear.x = msg.axes[1];  // Assuming axes[1] for forward/backward movement
            twist_msg.angular.z = msg.axes[2];  // Assuming axes[2] for angular movement
        }
        else
        {
            twist_msg.linear.x = 0.0;
            twist_msg.angular.z = 0.0;
        }

        ROS_INFO("Linear = %f, Angular = %f", twist_msg.linear.x, twist_msg.angular.z);
        twist_pub.publish(twist_msg);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_control");
    JoystickControl jc;
    ros::spin();
    return 0;
}
