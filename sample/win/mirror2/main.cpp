/**
 * FacialExpression can [only] be run in video or continuous 
 * image sequence model and [only] track the biggest face
 */

#include <algorithm>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "glesbox.hpp"
#include "simpleimage.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#define repeatii(cnt) for (int ii=0; ii<(cnt); ii++)
typedef struct {
  int stream_type; // 0 camera; 1, video; 2 picture list
  bool need_print_screen;
  char* stream_path;
} Configuration;

int main(int argc, char** argv) {
  Configuration setting = { 0, false };
  for (int i=0; i<argc; i++) {
    if (0==strcmp(argv[i], "-v") && i+1<argc) {
      setting.stream_type = 1;
      setting.stream_path = argv[i+1];
    }

    if (0==strcmp(argv[i], "-i") && i+1<argc) {
      setting.stream_type = 2;
      setting.stream_path = argv[i+1];
    }

    if (0==strcmp(argv[i], "-p") && i+1<argc) {
      setting.need_print_screen = atoi(argv[i+1]);
    }
  }

  cv::VideoCapture vc;
  std::vector<std::string> imglist;
  switch (setting.stream_type) {
  case 0:{
    vc.open(0);
    if (!vc.isOpened())
      return -1;

    double rate = vc.get(CV_CAP_PROP_FPS);
    int delay = 1000/rate;
    break;}
  case 1:{
    vc.open(setting.stream_path);
    if (!vc.isOpened())
      return -1;

    double rate = vc.get(CV_CAP_PROP_FPS);
    int delay = 1000/rate;
    break;}
  case 2:{ //for image sequences:image_001.jpg
    break;}
  default:
    break;
  }

  cv::Mat frame;
  const int stream_frame_count = vc.get(CV_CAP_PROP_FRAME_COUNT);
  auto readFrame = [setting, stream_frame_count, &vc, &frame,
    &imglist](int index)->bool {
    if (setting.stream_type == 0) {
      vc >> (frame);
      return true;
    }

    if (setting.stream_type == 1 && index < stream_frame_count) {
      vc >> (frame);
      return true;
    } else if (setting.stream_type == 2 && index < imglist.size()) {
      frame = cv::imread(imglist[index]);
      return true;
    } else { 
      return false;
    }
  };

  cv::VideoWriter vw;
  if (setting.need_print_screen) {
    int width = vc.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = vc.get(CV_CAP_PROP_FRAME_HEIGHT);
    int fourcc = vc.get(CV_CAP_PROP_FOURCC);
    fourcc = cv::VideoWriter::fourcc('X','V','I','D');
    cv::Size imsize = cv::Size(width, height);
    int fps = 30; // vc.get(CV_CAP_PROP_FPS);
    vw.open("screen.avi", fourcc, fps, imsize);
    if (!vw.isOpened()) {
      printf("open video writer failed!/n");
      return -1;
    }
  }

  vc >> frame;
  cv::imshow("mirror2", frame); //just initial a show windows

  libgb::SimpleImage face;
  libgb::GlesBox glesbox;
  libgb::GBConfig iner_conf;
  iner_conf.type = libgb::GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT;
  iner_conf.screen_native_id = (unsigned long)GetActiveWindow();
  iner_conf.screen_angle = 0.0f;
  iner_conf.screen_x = 0;
  iner_conf.screen_y = 0;
  iner_conf.screen_width = 640;
  iner_conf.screen_height = 480;

  bool stop(false);
  int frame_index = 0;
  int failed_count = 0;
  while (!stop) {
    if (!readFrame(frame_index++)) {
      std::cout<<"No More Pictures, Guy:&"<<std::endl;
      break;
    }

    cv::flip(frame, frame, 2);
    cv::cvtColor(frame, frame, CV_BGR2RGB);

    glesbox.draw_begin(iner_conf);
    face.setTextData(frame.cols, frame.rows, 3, frame.data);
    face.draw(iner_conf);
    glesbox.draw_end(iner_conf);
    auto key = cv::waitKey(1);
    if (key == 27) stop = true;
  }

  return 0;
}
