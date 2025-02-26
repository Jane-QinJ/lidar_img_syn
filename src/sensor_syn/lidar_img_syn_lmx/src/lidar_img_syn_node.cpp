#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS


#include "ros/ros.h"    
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
 
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>      
#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <string>
#include <iostream>
//#include<Windows.h>
//#include<direct.h>
#include <fstream>
#include <sstream>

 
using namespace std;
using namespace cv;
using namespace pcl;
using namespace sensor_msgs;
using namespace message_filters;

 
ros::Publisher PointCloudInfo_pub;
ros::Publisher ImageInfo_pub;
 
PointCloud2 syn_pointcloud;
Image syn_iamge;


 
void Syncallback(const PointCloud2ConstPtr& ori_pointcloud, const ImageConstPtr& ori_image)
{
    string dirName = "/home/janeqin/code/lidar_img_syn_lmx/syn_lidar_img/lidar_img";
    ROS_INFO("Synchronized PointCloud and Image!");
    
    syn_pointcloud = *ori_pointcloud;
    pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
    pcl_conversions::toPCL(*ori_pointcloud, *cloud);
    pcl::PointCloud<pcl::PointXYZI>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromPCLPointCloud2(*cloud, *temp_cloud);

    syn_iamge = *ori_image;

    string points_name = to_string(syn_pointcloud.header.stamp.toSec());
    string filename1 = dirName + "/" + points_name + ".pcd";
    string image_name = to_string(syn_iamge.header.stamp.toSec());
    string filename2 = dirName + "/" + image_name + ".jpg";

    // Save PointCloud and Image
    try {
        pcl::io::savePCDFile(filename1, *temp_cloud);
        ROS_INFO("Saved PointCloud: %s", filename1.c_str());
    } catch (const std::exception &e) {
        ROS_ERROR("Failed to save PCD file: %s", e.what());
    }

    try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(ori_image, sensor_msgs::image_encodings::BGR8);
        cv::Mat img = cv_ptr->image;
        cv::imwrite(filename2, img);
        ROS_INFO("Saved Image: %s", filename2.c_str());
    } catch (const std::exception &e) {
        ROS_ERROR("Failed to save image file: %s", e.what());
    }

    // Publish synchronized data
    PointCloudInfo_pub.publish(syn_pointcloud);
    ImageInfo_pub.publish(syn_iamge);
}

 
int main(int argc, char **argv)
{
 
    ros::init(argc, argv, "hw1");
    ros::NodeHandle node;
 
    cout << "\033[1;31m hw1! \033[0m" << endl;
 
    // 建立需要订阅的消息对应的订阅器
    message_filters::Subscriber<PointCloud2> PointCloudInfo_sub(node, "/velodyne_points", 1);//接收数据，用于同步
    message_filters::Subscriber<Image> ImageInfo_sub(node, "/usb_cam/image_raw", 1);
    
    // You are now using the ExactTime policy, which is stricter than ApproximateTime. 
    // Make sure that your sensor messages (LiDAR and camera) are closely timestamped. 
    // If their timestamps are not exactly the same, synchronization will not occur, and the callback won't be triggered.
    typedef sync_policies::ApproximateTime<PointCloud2, Image> MySyncPolicy; 
    // typedef sync_policies::ExactTime<PointCloud2, Image> MySyncPolicy; 

    
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), PointCloudInfo_sub, ImageInfo_sub); //queue size=10
    sync.registerCallback(boost::bind(&Syncallback, _1, _2));
 
 
    PointCloudInfo_pub = node.advertise<PointCloud2>("/syn_velodyne_points", 10);
    ImageInfo_pub = node.advertise<Image>("/syn_usb_cam_image_raw", 10);
 
    ros::spin();
    return 0;
}

