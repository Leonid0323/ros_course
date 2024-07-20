#include "notsodummy.h"
void NotSoDummy::setLaserData(const std::vector<float>& data)
{
	/* obstacle = false;
	 for (size_t i = 0; i<data.size(); i++)
	 {
	 if ( data[i] < 0.5 )
	 {
	 obstacle = true;
	 ROS_WARN_STREAM("OBSTACLE!!!");
	 break;
	 }
	 }
	*/
	double kHalfAngle = M_PI / 2.0;
	double kMinRange = 0.4;
	obstacle = false;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] <= kMinRange)
		{
			obstacle = true;
			double angle = -M_PI / 2.0 + i * M_PI / 200;
			if (angle > 0 && angle < kHalfAngle)
			{
				// left = true;
				key = false;
				ROS_WARN_STREAM("Obstacle detected on the left side 2222222222222!");
			}
			else if (angle<0 && angle >-kHalfAngle)
			{
				// right = true;
				key = true;
				ROS_WARN_STREAM("Obstacle detected on the right side 2222222222222!");
			}
			break;
		}
	}
}
//получение управления
void NotSoDummy::getControl(double& v, double& w)
{
	if (!obstacle)
	{
		// ROS_INFO_STREAM("No obstacle detected");
		v = 0.5;
		// cmd_vel_pub.publish(cmd);
		if (error < 15 && key)
		{
			w = -0.5;
			error++;
		}
		else if (error < 15 && !key)
		{
			w = 0.5;
			error++;
		}
	}
	else if (obstacle)
	{
		error = 0;
		if (!key)
		{
			// ROS_INFO_STREAM("Obstacle detected on the left side,
			 // turning right");
			v = -0.1;
			w = -0.5;
			// cmd_vel_pub.publish(cmd);
		}
		else if (key)
		{
			// ROS_INFO_STREAM("Obstacle detected on the right side,
			// turning left");
			v = -0.1;
			w = 0.5;

		}

	}