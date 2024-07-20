#include "ros/ros.h"
#include "std_msgs/String.h"

void replyCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_ERROR("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Subscriber reply_sub = n.subscribe("reply", 1, replyCallback); // Set the queue size to 1
    ros::Rate loop_rate(1); // Set loop rate to 1 Hz
    int count = 0;
    while (ros::ok())
    {
        std_msgs::String msg;
        msg.data = "hello world " + std::to_string(count);
        ROS_INFO("%s", msg.data.c_str());
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep(); // Sleep to limit loop rate
        count++;
    }
    return 0;
}
