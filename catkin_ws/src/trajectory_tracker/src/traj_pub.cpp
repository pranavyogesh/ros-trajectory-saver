#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/MarkerArray.h>
#include <fstream>
#include <sstream>
#include<cmath>




visualization_msgs::MarkerArray marker_array;
int id=0;
// Function to add items to the marker array
void addItemToMarkerArray(visualization_msgs::Marker marker) {
   // Create a marker and add it to the array
   // visualization_msgs::Marker marker;
   // Populate the marker fields as needed
   // marker.header.frame_id = "base_link";
   // marker.header.stamp = ros::Time::now();
   // Set other fields like marker type, position, etc.
   // ...


   // Add the marker to the array
   marker_array.markers.push_back(marker);
}
int main(int argc, char** argv) {
   ros::init(argc, argv, "marker_publisher");
   ros::NodeHandle nh;


   ros::Publisher marker_pub = nh.advertise
       <visualization_msgs::Marker>("visualization_marker", 10);


   ros::Publisher marker_array_pub = nh.advertise<visualization_msgs::MarkerArray>("/visualization_marker", 10);


   // Initialize the marker
   visualization_msgs::Marker marker;
   visualization_msgs::Marker marker_prev;//to get displacement values per iteration
   marker.header.frame_id = "odom"; // Frame ID of the marker
   marker.header.stamp = ros::Time::now(); // Timestamp
   marker.ns = "my_namespace"; // Namespace of the marker
   marker.type = visualization_msgs::Marker::SPHERE; // Type of the marker
   marker.action = visualization_msgs::Marker::ADD; // Action to take (ADD, DELETE, etc.)


   // Set the pose of the marker
   marker.pose.position.x = 0;
   marker.pose.position.y = 0;
   marker.pose.position.z = 0;
   marker.pose.orientation.x = 0.0;
   marker.pose.orientation.y = 0.0;
   marker.pose.orientation.z = 0.0;
   marker.pose.orientation.w = 1.0;


   // Set the scale of the marker
   marker.scale.x = 0.1;
   marker.scale.y = 0.1;
   marker.scale.z = 0.1;


   // Set the color of the marker
   marker.color.r = 1.0;
   marker.color.g = 0.0;
   marker.color.b = 0.0;
   marker.color.a = 1.0;


   // Set the lifetime of the marker
   // marker.lifetime = ros::Duration();


   // Publish the marker
   // while (ros::ok()) {
   //     marker.header.stamp = ros::Time::now(); // Update timestamp
   //     marker_pub.publish(marker); // Publish the marker
   //     ros::spinOnce();
   //     // Sleep for some time
   //     ros::Duration(1.0).sleep();
   // }


   tf::TransformListener listener;


   ros::Rate rate(10.0); // Set the frequency of the loop


   while (nh.ok()) {
       tf::StampedTransform transform;
       try {
           // listener.lookupTransform("/base_footprint", "/odom", ros::Time(0), transform);
           listener.lookupTransform("/odom", "/base_footprint", ros::Time(0), transform);
           ROS_INFO("Translation: x = %f, y = %f, z = %f", transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
           ROS_INFO("Rotation: x = %f, y = %f, z = %f, w = %f", transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z(), transform.getRotation().w());
           marker.id = id;
           id=id+1;
           marker.header.stamp = ros::Time::now(); // Update timestamp
           marker.pose.position.x = transform.getOrigin().x();
           marker.pose.position.y = transform.getOrigin().y();
           marker.pose.position.z = transform.getOrigin().z();
           marker.pose.orientation.x = transform.getRotation().x();
           marker.pose.orientation.y = transform.getRotation().y();
           marker.pose.orientation.z = transform.getRotation().z();
           marker.pose.orientation.w = transform.getRotation().w();
           marker_pub.publish(marker); // Publish the marker
           if(pow((pow(marker.pose.position.x-marker_prev.pose.position.x,2)+pow(marker.pose.position.y-marker_prev.pose.position.y,2)),0.5)>0.1)//for every 10cm displacement, create new marker
           {
               addItemToMarkerArray(marker);
               std::ofstream outFile("marker_array.csv", std::ofstream::out | std::ofstream::trunc);
               if (outFile.is_open()) {
                   // Write header
                   outFile << "Marker ID, Position X, Position Y, Position Z, Orientation X, Orientation Y, Orientation Z, Orientation W\n";
                   outFile.close();
               } else {
                   ROS_ERROR("Unable to open marker_array.csv for writing.");
               }
               marker_prev=marker;
               std::ofstream outFileAppend("marker_array.csv", std::ofstream::app);
       if (outFileAppend.is_open()) {
           for (const auto& marker : marker_array.markers) {
               outFileAppend << marker.id << ", "
                             << marker.pose.position.x << ", "
                             << marker.pose.position.y << ", "
                             << marker.pose.position.z << ", "
                             << marker.pose.orientation.x << ", "
                             << marker.pose.orientation.y << ", "
                             << marker.pose.orientation.z << ", "
                             << marker.pose.orientation.w << "\n";
           }
           outFileAppend.close();
           ROS_INFO("Marker array updated in marker_array.csv");
       } else {
           ROS_ERROR("Unable to open marker_array.csv for writing.");
       }
           }
       } catch (tf::TransformException &ex) {
           ROS_ERROR("%s",ex.what());
       }


       ros::spinOnce();
       // Sleep for some time
       ros::Duration(0.2).sleep();


       rate.sleep();
   }


       return 0;
   }
