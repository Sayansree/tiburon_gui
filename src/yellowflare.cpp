//written by devidutta
#include "tiburon_gui/yellowflare.h"
#include "ui_yellowflare.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
yellowflare::yellowflare(QWidget *parent) : QMainWindow(parent), ui(new Ui::yellowflare) {
  LOGO_PATH = ros::package::getPath("tiburon_gui") + "/utils/logo.jpeg";
  CONFIG_PATH = ros::package::getPath("tiburon_gui")+ "/config/yellowflare_config.dat";
  HSV[0]=0; HSV[1]=180; HSV[2]=0; HSV[3]=255; HSV[4]=0; HSV[5]=255;
	
  ui->setupUi(this);
  cv::Mat frame_save=cv::imread(LOGO_PATH);
  cv::cvtColor(frame_save, frame_save, CV_BGR2RGB);
  ui->logo->setPixmap(QPixmap::fromImage(QImage(frame_save.data, frame_save.cols, frame_save.rows,frame_save.step, QImage::Format_RGB888)));
  connect(ui->save, SIGNAL(pressed()), this, SLOT(save()));
  connect(ui->channel, SIGNAL(currentIndexChanged(int)),this,SLOT(channel(int)));
  connect(ui->out_channel,SIGNAL(currentIndexChanged(int)),this,SLOT(out_chan(int)));
  connect(ui->hmin,SIGNAL(valueChanged(int)),this,SLOT(h(int)));
  connect(ui->smin,SIGNAL(valueChanged(int)),this,SLOT(s(int)));
  connect(ui->vmin,SIGNAL(valueChanged(int)),this,SLOT(v(int)));
  connect(ui->hmax,SIGNAL(valueChanged(int)),this,SLOT(H(int)));
  connect(ui->smax,SIGNAL(valueChanged(int)),this,SLOT(S(int)));
  connect(ui->vmax,SIGNAL(valueChanged(int)),this,SLOT(V(int)));
  connect(ui->sobel,SIGNAL(valueChanged(int)),this,SLOT(sob(int)));
  connect(ui->open_morph,SIGNAL(valueChanged(int)),this,SLOT(opn(int)));
  connect(ui->HL_th,SIGNAL(valueChanged(int)),this,SLOT(Threshold(int)));
  connect(ui->width,SIGNAL(valueChanged(int)),this,SLOT(wid(int)));
  connect(ui->height,SIGNAL(valueChanged(int)),this,SLOT(hei(int))); 
  connect(ui->pause,SIGNAL(pressed()),this,SLOT(pause()));
  connect(ui->close,SIGNAL(pressed()),this,SLOT(close()));
 

}

yellowflare::~yellowflare() { delete ui; }

void yellowflare::hsv(int value,int index){HSV[index]=value;}
void yellowflare::channel(int index){ch_index=index;}
void yellowflare::h(int val){HSV[0]=val;}
void yellowflare::s(int val){HSV[1]=val;}
void yellowflare::v(int val){HSV[2]=val;}
void yellowflare::H(int val){HSV[3]=val;}
void yellowflare::S(int val){HSV[4]=val;}
void yellowflare::V(int val){HSV[5]=val;}
void yellowflare::opn(int val){S_op=val;}
void yellowflare::sob(int val){S_sob=val;}
void yellowflare::out_chan(int val){out_ch=val;}
void yellowflare::Threshold(int val){threshold=val;}
void yellowflare::wid(int val){width=val;}
void yellowflare::hei(int val){height=val;}

void yellowflare::feed(cv::Mat img2)
{
if(!img2.empty()) {
//cout<<"inside yellow flare\n";
	cv::Mat img,dx,maskl,maskl2,maskr,maskr2,mask,fin_img, grad,last_img;
	img=img2.clone();
	cv::Scalar maxHSV,minHSV;
	channel_show(img2);
	cv::GaussianBlur( img, mask, cv::Size( 9, 9 ), 0, 0 );
        cv::cvtColor(mask, mask, cv::COLOR_BGR2HSV);

        minHSV = cv::Scalar(HSV[0],HSV[1],HSV[2]);
        maxHSV = cv::Scalar(HSV[3],HSV[4],HSV[5]);
        cv::inRange(mask, minHSV, maxHSV, maskl);
	mask=maskl;

	if(out_ch==0)cv::cvtColor(mask, fin_img, cv::COLOR_GRAY2RGB);
	cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( S_op,S_op ),cv::Point(S_op/2,S_op/2) );
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
	if(out_ch==1)cv::cvtColor(mask, fin_img, cv::COLOR_GRAY2RGB);
	cv::Sobel(mask, dx,  CV_16S, 1,0, S_sob);
        cv::convertScaleAbs(dx, mask);
	if(out_ch==2)cv::cvtColor(mask, fin_img, cv::COLOR_GRAY2RGB);
	
	Canny(mask,grad, threshold,threshold*2, 3);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(grad,contours, hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f> centres(contours.size());
	for(size_t i=0;i<contours.size();i++){

		approxPolyDP(contours[i],contours_poly[i],9,true);
		boundRect[i]=boundingRect(contours_poly[i]);
	}
	for(size_t i=0;i<contours.size();i++){
		if(boundRect[i].width>width && boundRect[i].height>height)
		rectangle(img,boundRect[i].tl(), boundRect[i].br(),Scalar(255,0,0),2);
	}

	if(out_ch==3)fin_img=img;
    ui->out->setPixmap(QPixmap::fromImage(QImage(fin_img.data, fin_img.cols, fin_img.rows,fin_img.step, QImage::Format_RGB888)));
  
  }  	

}
void yellowflare::save()
{
std::cout<<"Parameters saved\n";
        ifstream f(CONFIG_PATH);
        if(f.eof())
	{
		std::cout<<"Config File missing\n";
                ofstream ofs(CONFIG_PATH);
                ofs.close();
	}
        else
	{
		ofstream f(CONFIG_PATH,ios::ate);
		f << "HSV THRESHOLDING PARAMETERS:\nLow_H = " << HSV[0];
		f << "\nLow_S = " << HSV[1];
		f << "\nLow_V = " << HSV[2];
		f << "\nHigh_H = " << HSV[3];
		f << "\nHigh_S = " << HSV[4];
		f << "\nHigh_V = " << HSV[5];
		f << "\nSOBEL PARAMETERS:\nKernel_size = " << S_sob;
		f << "\nMORPH PARAMETERS:\nKernel_size = " << S_op;
		f << "\nCANNY THRESHOLD PARAMETERS:\nThreshold = " << threshold;
		f << "\nRatio = " << 2;
		f << "\nKernel_size = " << 3;
		f << "\nCONTOUR RECTANGLE PARAMETERS:\nHeight = " << height;
		f << "\nWidth = " << width;
		f.close(); 
	}
}

void yellowflare::pause(){
//mainwinui->pau();
}


void yellowflare::channel_show(cv::Mat img)
{
	std::vector<cv::Mat> Channels(3);
   	 cv::Mat zero, fin_img,temp;
    	 zero = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_8UC1);
	switch(ch_index)
	{
		case 0:
		
	 	fin_img=img;
		break;

		case 1:
		split(img, Channels);
		Channels[0]=Channels[2];
		Channels[1]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;

		case 2:
		split(img, Channels);
		Channels[0]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;
		
		case 3:
		split(img, Channels);
		Channels[2]=Channels[0];
		Channels[0]=zero;
		Channels[1]=zero;
		merge(Channels, fin_img);
		break;

		case 4:
		cv::cvtColor(img, fin_img, CV_BGR2HSV);
		break;

		case 5:
		cv::cvtColor(img, temp, CV_BGR2HSV);
		split(temp, Channels);

		Channels[1]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;

		case 6:
		cv::cvtColor(img, temp, CV_BGR2HSV);
		split(temp, Channels);
		Channels[0]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;
		
		case 7:
		cv::cvtColor(img, temp, CV_BGR2HSV);
		split(temp, Channels);
		Channels[0]=zero;
		Channels[1]=zero;
		merge(Channels, fin_img);
		break;

		case 8:
		cv::cvtColor(img, fin_img, CV_BGR2GRAY);
		Channels[0]=fin_img;
		Channels[1]=fin_img;
		Channels[2]=fin_img;
		merge(Channels, fin_img);
		break;
	}
        ui->channel_show->setPixmap(QPixmap::fromImage(QImage(fin_img.data, fin_img.cols, fin_img.rows,fin_img.step, QImage::Format_RGB888)));
}

