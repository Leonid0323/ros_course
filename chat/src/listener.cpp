#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::Publisher reply_pub = n.advertise<std_msgs::String>("reply", 1); // Set queue size to 1
    ros::Subscriber chatter_sub = n.subscribe("chatter", 1, chatterCallback); // Set queue size to 1
    ros::Rate loop_rate(1);

    int count = 0;
    while (ros::ok())
    {
        std_msgs::String msg;
        msg.data = "how are you " + std::to_string(count);
        ROS_INFO("%s", msg.data.c_str());
        reply_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}
