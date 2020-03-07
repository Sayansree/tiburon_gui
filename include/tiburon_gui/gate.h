//written by sayansree paria
#ifndef GATE_H
#define GATE_H

#include "ui_gate.h"
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <ros/package.h>
#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int8.h>
#include <string>

namespace Ui {
class gate;
}

class gate : public QMainWindow {
  Q_OBJECT

public:
  	explicit gate(QWidget *parent = 0);
  	~gate();
	void feed(cv::Mat);
	Ui::gate *ui;

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
 	void h2(int);
	void s2(int);
	void v2(int);
	void H2(int);
	void S2(int);
	void V2(int);
	void opn(int );
	void sob(int );
	void out_chan(int );
  void out_chan_2(int );
	void Threshold(int );
	void minLine(int );
	void minGap(int );
  void sob_thres(int);
  void out_chan_3(int);
  void median_ker(int);
  void CheckHSV(int);
  void CheckGray(int);
  void Merge(int);

private:
  enum merging { OR,AND,XOR};
  	void channel_show(cv::Mat);
    cv::Mat gate2(cv::Mat,cv::Mat*);
    cv::Mat gate1(cv::Mat,cv::Mat*);
	int HSV[12];
	int S_op=1;
	int S_sob=1;
	int out_ch=-1;
  	int ch_index=0;
	int threshold=0;
	int minLineLength=0;
	int minLineGap=0;
  int th_sob=0;
  int med_kernal=1;
  bool hsvcheck=false;
  bool graycheck=false;
  int merg=0;
	std::fstream outf;
  cv::VideoCapture cap;
  std::string LOGO_PATH, CONFIG_PATH;

};

#endif // FRAMER_H
