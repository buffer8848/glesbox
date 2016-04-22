// Copyright (c) 2016 Intel Corporation.
// Author:  binfeix.li@intel.com (2016.v2.0)

#include <iostream>
#include <vector>
#include <stdint.h>

#include <windows.h>

#include "glesbox.hpp"
#include "SimpleImage.hpp"
#include "yuvimage.hpp"

#include "GL/glut.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace libgb;
//-------------------------------------------------------------------------------------------------
#define YUV_CHANEL 1
#define VIDEO_CHANEL 3
#define IMAGE_CHANEL 4

uint32_t video_width = 640, video_height = 480;
uint32_t win_width = 512, win_height = 512;
cv::VideoCapture vc;
GlesBox engine;
vector<uint8_t> texture_data;
vector<uint8_t> texture_offline_data;
SimpleImage render;
YuvImage render_offline;

//-------------------------------------------------------------------------------------------------

bool InitializeCamera(bool fromdevice) {
  if (fromdevice) {
    vc.open(0);
    if (!vc.isOpened()) {
      cout << "InitializeCamera failed." << endl;
      return false;
    }
    vc.set(cv::CAP_PROP_FRAME_WIDTH, video_width);
    vc.set(cv::CAP_PROP_FRAME_HEIGHT, video_height);
  }

  return true;
}

bool InitializeOpenGL(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayString( "rgb double depth>=24 samples" );
  glutInitWindowPosition(0,0);
  glutInitWindowSize( win_width*2, win_height );
  glutCreateWindow( "avatar" );

  return true;
}

bool InitializeSDK() {
  texture_data.resize(video_width * video_height * VIDEO_CHANEL);
  texture_offline_data.resize(win_width * win_height * VIDEO_CHANEL);
  return true;
}

void Display() {
  glClearColor(0.58,0.73,0.89,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, win_width, win_height);

  //capture camera picture
  cv::Mat frame;
  vc >> frame;
  cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
  cv::flip(frame, frame, 1);

  GBConfig conf;
  conf.type = GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT;
  conf.screen_width = win_width;
  conf.screen_height = win_height;
  conf.screen_native_id = (unsigned long)GetActiveWindow();
  conf.screen_angle = 0.0f;
  conf.offline_width = win_width;
  conf.offline_height = win_height;
  conf.offline_widthstep = conf.offline_width*YUV_CHANEL;
  conf.offline_channel = YUV_CHANEL;
  conf.offline_type = GB_IMAGE_YUV420;
  conf.offline_angle = 0.0f;
  conf.offline_data = texture_offline_data.data();
  engine.draw_begin(conf);
  //directly draw
  memcpy(texture_data.data(), frame.data, texture_data.size());
  render.setTextData(frame.cols, frame.rows, VIDEO_CHANEL, texture_data.data());
  render.draw(conf);
  engine.draw_end(conf);
  //draw image
  conf.type = GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT;
  conf.screen_x = win_width;
  conf.screen_angle = 180.0f;
  engine.draw_begin(conf);
  render_offline.setTextData(
    conf.offline_width, conf.offline_height, YUV_CHANEL, texture_offline_data.data());
  render_offline.draw(conf);
  engine.draw_end(conf);
  //cv::imshow("fuck", frame);
  //glutSwapBuffers();
}

void Reshape(int w, int h) {
  //win_width = w/2;
  //win_height = h;
}

void Idle() {
  glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y) {
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
  InitializeCamera(true);
  InitializeOpenGL(argc, argv);
  InitializeSDK();

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
  glutIdleFunc(Idle);  //opengl free time callback
  glutMainLoop();  

  return 0;
}