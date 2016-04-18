// Copyright (c) 2016 Intel Corporation.
// Author:  binfeix.li@intel.com (2016.v2.0)

#include <iostream>
#include <vector>
#include <stdint.h>

#include <windows.h>

#include "GL/glut.h"
#include "opencv2/opencv.hpp"

#include "glesbox.hpp"
#include "SimpleImage.hpp"

using namespace std;
using namespace libgb;
//-------------------------------------------------------------------------------------------------
#define VIDEO_CHANEL 3
#define IMAGE_CHANEL 4

uint32_t video_width = 320*2, video_height = 240*2;
uint32_t win_width = 640, win_height = 480;
cv::VideoCapture vc;
GlesBox engine;
vector<uint8_t> texture_data;
vector<uint8_t> texture_offline_data;
SimpleImage render;
SimpleImage render_offline;

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
  texture_offline_data.resize(win_width * win_height * IMAGE_CHANEL);
  return true;
}

void Display() {
  glClearColor(0.58,0.73,0.89,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, win_width, win_height);

  //capture camera picture
  cv::Mat frame;
  vc >> frame;
  cv::flip(frame, frame, 1);

  GBConfig conf;
  conf.type = GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT;
  conf.screen_width = win_width;
  conf.screen_height = win_height;
  conf.screen_native_id = (unsigned long)GetActiveWindow();
  conf.screen_angle = 0.0f;
  conf.offline_width = win_width;
  conf.offline_height = win_height;
  conf.offline_channel = IMAGE_CHANEL;
  conf.offline_type = GB_IMAGE_RGBA32;
  conf.offline_data = texture_offline_data.data();
  engine.draw_begin(conf);
  //directly draw
  //glViewport(0, 0, win_width, win_height);
  memcpy(texture_data.data(), frame.data, texture_data.size());
  render.setTextData(frame.cols, frame.rows, VIDEO_CHANEL, texture_data);
  render.draw(conf);
  //draw image
  //glViewport(win_width, 0, win_width, win_height);
  //render_offline.setTextData(
  //  conf.offline_width, conf.offline_height, IMAGE_CHANEL, texture_offline_data);
  //render_offline.draw(conf);
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