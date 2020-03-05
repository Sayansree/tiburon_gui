#ifndef REDFLARE_H
#define REDFLARE_H

#include "ui_redflare.h"
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <ros/package.h>
#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int8.h>
#include <string>

namespace Ui {
class redflare;
}

class redflare : public QMainWindow {
  Q_OBJECT

public:
  explicit redflare(QWidget *parent = 0);
  ~redflare();
   Ui::redflare *ui;
  void feed(cv::Mat);

public slots:
	void save();
	void channel(int);
	void hsv(int,int);
	void h(int);
	void s(int);
	void v(int);
	void H(int);
	void S(int);
	void V(int);
	void opn(int );
	void sob(int );
	void out_chan(int );
	void Threshold(int );
	void wid(int );
	void hei(int );
	void pause();
	
  

private:
  
  void channel_show(cv::Mat);
	int HSV[6];
	int S_op=1;
	int S_sob=1;
	int out_ch=0;
  	int ch_index=0;
	int threshold=0;
	int width;
	int height;
int f=0;
//	mainwin *mainwinui=NULL;
  cv::VideoCapture cap;
  std::string LOGO_PATH, CONFIG_PATH;
};

#endif // FRAMER_H
