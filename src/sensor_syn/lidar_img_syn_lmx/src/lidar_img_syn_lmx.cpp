#include "ros/ros.h"    
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
// #include <pcl/io/pcd_io.h>
// #include<pcl/io/ply_io.h>

// #include <opencv2\opencv.hpp>      
// #include<opencv2/core/types.hpp>
// #include<opencv2/core/core.hpp>
 
 
#include <iostream>
 
using namespace std;
// using namespace cv;
using namespace sensor_msgs;
using namespace message_filters;
// using namespace pcl;
 
ros::Publisher PointCloudInfo_pub;
ros::Publisher ImageInfo_pub;
 
PointCloud2 syn_pointcloud;
Image syn_iamge;
 
void Syncallback(const PointCloud2ConstPtr& ori_pointcloud, const ImageConstPtr& ori_image)
{
    ROS_INFO("Synchronized PointCloud and Image!");
    syn_pointcloud = *ori_pointcloud;
    syn_iamge = *ori_image;
    ROS_INFO("Synchronized pointcloud timestamp: %f", syn_pointcloud.header.stamp.toSec());
    ROS_INFO("Synchronized image timestamp: %f", syn_iamge.header.stamp.toSec());

    PointCloudInfo_pub.publish(syn_pointcloud);
    ImageInfo_pub.publish(syn_iamge);
}

 
int main(int argc, char **argv)
{
 
    ros::init(argc, argv, "hw1");
    ros::NodeHandle node;
 
    cout << "\033[1;31m hw1! \033[0m" << endl;
 
    // 建立需要订阅的消息对应的订阅器
    message_filters::Subscriber<PointCloud2> PointCloudInfo_sub(node, "/velodyne_points", 1);
    message_filters::Subscriber<Image> ImageInfo_sub(node, "/usb_cam/image_raw", 1);
    
    typedef sync_policies::ApproximateTime<PointCloud2, Image> MySyncPolicy; 
    
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), PointCloudInfo_sub, ImageInfo_sub); //queue size=10
    sync.registerCallback(boost::bind(&Syncallback, _1, _2));
 
 
    PointCloudInfo_pub = node.advertise<PointCloud2>("/syn_velodyne_points", 10);
    ImageInfo_pub = node.advertise<Image>("/syn_usb_cam_image_raw", 10);

    


 
    ros::spin();
    return 0;
}

