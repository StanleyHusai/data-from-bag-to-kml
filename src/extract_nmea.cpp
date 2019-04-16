#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include <vector>
#include <iostream>

#include<eigen3/Eigen/Dense>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Geometry>
#include <fstream>
#include<sstream>
#include <stdlib.h>
#include <iomanip>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>


#include <math.h>
#include <time.h>
#include <algorithm>

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/NavSatFix.h>

#include <std_msgs/Time.h>
#include <nav_msgs/Odometry.h>
#include <nmea_msgs/Sentence.h>
#include <sensor_msgs/PointCloud2.h>
#include <visualization_msgs/MarkerArray.h>

using namespace std;


class extract_nmea
{
public:

  double lat;
  double lon;
  double alt;

  extract_nmea()
  {
    nmea_trajectory_pub = n.advertise<sensor_msgs::NavSatFix>("/nmea_trajectory", 1000);
    sub = n.subscribe("/nmea_sentence", 1000, &extract_nmea::nmeaCallback, this);
  }

  void nmeaCallback(const nmea_msgs::SentenceConstPtr& nmea_msg)  
  {
    sensor_msgs::NavSatFix navfix_ ;

    std::vector<std::string> str_vec_ptr;
    std::string token;
    std::stringstream ss(nmea_msg->sentence);
    bool find_SOL_COMPUTED =0;
    while (getline(ss, token, ',')) // fine characters line by line from 'ss' and store them into 'token', stop where ','
    {
      if(token == "#BESTPOSA") // find specific characters
      {
        find_SOL_COMPUTED = true;
      }
      if( find_SOL_COMPUTED ) // find found
      {
        str_vec_ptr.push_back(token);
      }
    }
        if(find_SOL_COMPUTED)
    {
      std::cout << std::setprecision(17);
      double lat = strtod((str_vec_ptr[11]).c_str(), NULL);
      double lon = strtod((str_vec_ptr[12]).c_str(), NULL);
      double alt = strtod((str_vec_ptr[13]).c_str(), NULL);
      std::cout << std::setprecision(17);

      navfix_.latitude = lat;
      navfix_.longitude = lon;
      navfix_.altitude = alt;

      nmea_trajectory_pub.publish(navfix_);

      cout<<navfix_<<endl;
    }
  } 

private:
  ros::NodeHandle n;
  ros::Publisher nmea_trajectory_pub;
  ros::Subscriber sub;
};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "nmea_plot");
  extract_nmea extract_nmea_;
  ros::spin();
  while (ros::ok()) {
  }
	return 0;
}

