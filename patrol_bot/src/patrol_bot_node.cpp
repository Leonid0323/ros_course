#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PointStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <vector>
using
MoveBaseClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;
//умный указатель на объект - клиент
boost::shared_ptr<MoveBaseClient> moveBaseClientPtr;
std::vector<geometry_msgs::PointStamped> route_vector = {};
int curr_point_ix = 0;
bool goal_achieved = true;
void active_callback()
{
	ROS_INFO_STREAM("goal is started");
}
void feedback_callback(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback)
{
	ROS_INFO_STREAM("feedback " <<
		" robot pose x" << feedback->base_position.pose.position.x <<
		" y = " << feedback->base_position.pose.position.y);
}
void done_callback(const actionlib::SimpleClientGoalState& state,
	const move_base_msgs::MoveBaseResultConstPtr& result)
{
	if (state == actionlib::SimpleClientGoalState::SUCCEEDED)
	{
		ROS_INFO("Target is reached");
		curr_point_ix += 1;
		if (curr_point_ix > route_vector.size() - 1) {
			curr_point_ix = 0;
		}
		goal_achieved = true;
		ROS_DEBUG_STREAM("Current target" << route_vector[curr_point_ix].point.x << "
			"<<route_vector[curr_point_ix].point.y);
	}
	else
	{
		ROS_ERROR("move_base has failed");
	}
}
void clickPointCallback(const geometry_msgs::PointStamped& point)
{
	ROS_INFO_STREAM(" get point " << point.point.x << " " << point.point.y);
	route_vector.push_back(point);
}
void nextGoal() {
	if (goal_achieved && route_vector.size() > 0) {
		goal_achieved = false;
		//задаем ориентацию в целевой точке
		double target_angle = M_PI / 2;
		//формируем структуру цели для move_base Action
		move_base_msgs::MoveBaseGoal goal;
		goal.target_pose.pose.position = route_vector[curr_point_ix].point;
		//задаем кватернион, соответствующий ориентации
		goal.target_pose.pose.orientation.z = sin(target_angle / 2);
		goal.target_pose.pose.orientation.w = cos(target_angle / 2);
		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();
		//отправляем цель
		moveBaseClientPtr->sendGoal(goal,
			done_callback,
			active_callback,
			feedback_callback
		);
	}
}
int main(int argc, char** argv)
{
	ros::init(argc, argv, "control_node");
	ros::NodeHandle node("~");
	moveBaseClientPtr.reset(new MoveBaseClient("/move_base", false));
	while (!moveBaseClientPtr->isServerConnected())
	{
		ros::spinOnce();
	}
	ROS_INFO_STREAM("server move_base is connected");
	ros::Subscriber point_sub = node.subscribe("/clicked_point", 1, clickPointCallback);
	ros::Rate r(10);
	while (true) {
		nextGoal();
		ros::spinOnce();
		r.sleep();
	}
	return 0;
}
