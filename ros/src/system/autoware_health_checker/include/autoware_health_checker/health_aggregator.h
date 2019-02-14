#ifndef HEALTH_AGGREGATOR_H_INCLUDED
#define HEALTH_AGGREGATOR_H_INCLUDED

// headers in ROS
#include <diagnostic_msgs/DiagnosticArray.h>
#include <jsk_rviz_plugins/OverlayText.h>
#include <ros/ros.h>

// headers in Autoware
#include <autoware_health_checker/constants.h>
#include <autoware_system_msgs/NodeStatus.h>
#include <autoware_system_msgs/SystemStatus.h>

// headers in boost
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>

// headers in STL
#include <map>
#include <mutex>

class HealthAggregator {
public:
  HealthAggregator(ros::NodeHandle nh, ros::NodeHandle pnh);
  ~HealthAggregator();
  void run();

private:
  ros::NodeHandle nh_;
  ros::NodeHandle pnh_;
  ros::Publisher system_status_pub_;
  std::map<uint8_t, ros::Publisher> text_pub_;
  ros::Subscriber node_status_sub_;
  ros::Subscriber diagnostic_array_sub_;
  void publishSystemStatus();
  void nodeStatusCallback(const autoware_system_msgs::NodeStatus::ConstPtr msg);
  void
  diagnosticArrayCallback(const diagnostic_msgs::DiagnosticArray::ConstPtr msg);
  std::string
  generateText(std::vector<autoware_system_msgs::DiagnosticStatus> status);
  jsk_rviz_plugins::OverlayText
  generateOverlayText(autoware_system_msgs::SystemStatus status, uint8_t level);
  std::vector<autoware_system_msgs::DiagnosticStatus>
  filterNodeStatus(autoware_system_msgs::SystemStatus status, uint8_t level);
  boost::optional<autoware_system_msgs::HardwareStatus>
  convert(const diagnostic_msgs::DiagnosticArray::ConstPtr msg);
  autoware_system_msgs::SystemStatus system_status_;
  std::mutex mtx_;
  void updateConnectionStatus();
};
#endif // HEALTH_AGGREGATOR_H_INCLUDED