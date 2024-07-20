#pragma once
#include <ros/ros.h>
#include "control.h"
#include <cmath>
class NotSoDummy : public Control
{
private:
	int error = 100;
	double min_range;
	bool obstacle = false;
	double max_vel;
	double min_vel;
	double max_omega;
	bool key;
public:
	//��������� ������ ������
	void setLaserData(const std::vector<float>& data) override;
	//��������� ������� ������� ������ - ��� ������� ���� ���������� �� ��������� -
	������� ������
		void setRobotPose(double x, double y, double theta) override {}
	//��������� ����������
	void getControl(double& v, double& w) override;
	std::string getName() override { return "NotSoDummy"; }
	NotSoDummy(double range = 0.4, double maxv = 0.5, double maxw = 0.5, double minv
		= 0.1) :
		min_range(range),
		max_vel(maxv),
		max_omega(maxw),
		min_vel(minv)

	{
		ROS_DEBUG_STREAM("NotSoDummy constructor");
	}
};