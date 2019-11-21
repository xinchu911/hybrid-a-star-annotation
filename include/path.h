#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <cstring>
#include <vector>

#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>
#include <visualization_msgs/MarkerArray.h>

#include "node3d.h"
#include "constants.h"
#include "helper.h"
namespace HybridAStar {
/*!
   \brief A class for tracing and visualizing the path generated by the Planner
   用于跟踪和可视化生成的路径的类
*/
class Path {
 public:
  /// The default constructor initializing the path object and setting publishers for the same.
  Path(bool smoothed = false) {
     //ROS发布的topic 名称(没有经过平滑处理)
    std::string pathTopic = "/path";
    std::string pathNodesTopic = "/pathNodes";
    std::string pathVehicleTopic = "/pathVehicle";
   
   //ROS发布的topic 名称(经平滑处理)
    if (smoothed) {
      pathTopic = "/sPath";
      pathNodesTopic = "/sPathNodes";
      pathVehicleTopic = "/sPathVehicle";
      this->smoothed = smoothed;
    }

    // _________________
    // TOPICS TO PUBLISH 创建发布的话题及消息类型
    pubPath = n.advertise<nav_msgs::Path>(pathTopic, 1);
    pubPathNodes = n.advertise<visualization_msgs::MarkerArray>(pathNodesTopic, 1);
    pubPathVehicles = n.advertise<visualization_msgs::MarkerArray>(pathVehicleTopic, 1);

    // CONFIGURE THE CONTAINER
    path.header.frame_id = "path";
  }

  //  // __________
  //  // TRACE PATH
  //  /*!
  //     \brief Given a node pointer the path to the root node will be traced recursively
  //     \param node a 3D node, usually the goal node
  //     \param i a parameter for counting the number of nodes
  //  */
  //  void tracePath(const Node3D* node, int i = 0);
  /*!
     \brief Given a node pointer the path to the root node will be traced recursively
     \param node a 3D node, usually the goal node
     \param i a parameter for counting the number of nodes
     // 对每个3D节点信息，分别进行addSegment、addNode、addVehicle
  */
  void updatePath(std::vector<Node3D> nodePath);
  /*!
     \brief Adds a segment to the path:给路径增加一个segment
     \param node a 3D node: node-为3D节点类
  */
  void addSegment(const Node3D& node);
  /*!
     \brief Adds a node to the path 将node添加到path中
     \param node a 3D node：参数node为待添加的节点（3D类）
     \param i a parameter for counting the number of nodes: i为计算节点数目的参数
  */
  void addNode(const Node3D& node, int i);
  /*!
     \brief Adds a vehicle shape to the path 在路径中添加车的外形
     \param node a 3D node 参数node为3D类节点
     \param i a parameter for counting the number of nodes 参数i是对节点进行计数的参数
  */
  void addVehicle(const Node3D& node, int i);

  // ______________
  // PUBLISH METHODS

  /// Clears the path
  void clear();//清除路径
  /// Publishes the path
  void publishPath() { pubPath.publish(path); }//将path通过话题发布出去
  /// Publishes the nodes of the path
  void publishPathNodes() { pubPathNodes.publish(pathNodes); }//将路径的节点进行发布
  /// Publishes the vehicle along the path
  void publishPathVehicles() { pubPathVehicles.publish(pathVehicles); }//在路径上发布车

 private:
  /// A handle to the ROS node
  ros::NodeHandle n;//ROS的节点句柄
  /// Publisher for the path as a spline
  ros::Publisher pubPath;//ROS发布器，用于发布路径
  /// Publisher for the nodes on the path
  ros::Publisher pubPathNodes;//ROS路径节点发布器
  /// Publisher for the vehicle along the path
  ros::Publisher pubPathVehicles;//用于在路径上发布车子位置
  /// Path data structure for visualization
  nav_msgs::Path path;//路径数据结构，用于可视化
  /// Nodes data structure for visualization
  visualization_msgs::MarkerArray pathNodes;//节点数据结构，用于可视化
  /// Vehicle data structure for visualization
  visualization_msgs::MarkerArray pathVehicles;//车子数据结构，用于可视化
  /// Value that indicates that the path is smoothed/post processed
  bool smoothed = false;
};
}
#endif // PATH_H
