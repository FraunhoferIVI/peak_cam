// Copyright (c) 2020, Sherif Nekkah 
// All rights reserved. 
// 
// DISCLAMER:
//
//
// This package was created and used within an academic project and should
// be considered as experimental code. There may be bugs and deficiencies in the
// software. Feel free for suggestions, pull requests or any possible issue. 
//
//
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met: 
// 
//  * Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer. 
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the distribution. 
//  * Neither the name of  nor the names of its contributors may be used to 
//    endorse or promote products derived from this software without specific 
//    prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE. 
#ifndef PEAK_CAM__PEAK_CAM_NODE_HPP_
#define PEAK_CAM__PEAK_CAM_NODE_HPP_

#include <iostream>
#include <atomic>

//ROS Headers
#include "camera_info_manager/camera_info_manager.h"
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <dynamic_reconfigure/server.h>
#include <nodelet/nodelet.h>

//OpenCV Headers
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cv_bridge/cv_bridge.h>

//IDS Camera Headers
#include <peak_ipl/peak_ipl.hpp>
#include <peak/converters/peak_buffer_converter_ipl.hpp>
#include <peak/peak.hpp>

//Parameters
#include "peak_cam/acquisition_parameters.hpp"
#include "peak_cam/PeakCamConfig.h"


namespace peak_cam
{

class PeakCamNode : public nodelet::Nodelet
{
public:
  PeakCamNode();
  ~PeakCamNode();
  
  ///
  /// Initialize Nodelet member variables
  ///
  /// @return void
  ///
  void onInit();
  // acquisitionLoop function and bool are public to run on particular thread
  void acquisitionLoop(const ros::TimerEvent & event);
  
  // Preventing two threads to acces variable acquisitionLoopRunning
  std::atomic<bool> m_acquisitionLoopRunning{false};

private:
  ros::NodeHandle m_nodeHandle;
  ros::NodeHandle m_nodeHandleMT;
  ros::Publisher m_pubImage, m_pubCameraInfo;

  /// Camera Info Manager
  camera_info_manager::CameraInfoManager *m_cameraInfoManager;

  ros::Timer m_acquisitionTimer;

  std::shared_ptr<dynamic_reconfigure::Server<PeakCamConfig> > m_paramsServer;
  dynamic_reconfigure::Server<PeakCamConfig>::CallbackType m_handleParams;
  std::shared_ptr<peak::core::DataStream> m_dataStream;
  std::shared_ptr<peak::core::Device> m_device;
  std::shared_ptr<peak::core::NodeMap> m_nodeMapRemoteDevice;
  peak::ipl::PixelFormatName m_pixelFormat;
  
  std::string m_image_encoding;
  std::shared_ptr<std_msgs::Header> m_header;

  // Camera Parameters
  Peak_Params m_peakParams;

  uint8_t m_bytesPerPixel;

  void reconfigureRequest(const PeakCamConfig &, uint32_t);
  void openDevice();
  void setDeviceParameters();
  void closeDevice();
};

} // namespace peak_cam

#endif  // PEAK_CAM__PEAK_CAM_NODE_HPP_
