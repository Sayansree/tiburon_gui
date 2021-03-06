//written by Sayansree paria
#include "tiburon_gui/gate.h"
#include "ui_gate.h"
#include <iostream>
#include <fstream>
gate::gate(QWidget *parent) : QMainWindow(parent), ui(new Ui::gate) {
  LOGO_PATH = ros::package::getPath("tiburon_gui") + "/utils/logo.jpeg";
  CONFIG_PATH = ros::package::getPath("tiburon_gui")+ "/config/gate_config.dat";

  ui->setupUi(this);

  cv::Mat frame_save=cv::imread(LOGO_PATH);
  cv::cvtColor(frame_save, frame_save, CV_BGR2RGB);
  ui->logo->setPixmap(QPixmap::fromImage(QImage(frame_save.data, frame_save.cols, frame_save.rows,frame_save.step, QImage::Format_RGB888)).scaled(ui->logo->size()));

  	outf.open(CONFIG_PATH,std::ios::in);
	for(int i=0;i<12;i++)
	outf>>HSV[i];
	outf>>S_op;
	outf>>S_sob;
	outf>>threshold;
	outf>>minLineLength;
	outf>>minLineGap;
  outf>>th_sob;
  outf>>med_kernal;
	outf.close();

  connect(ui->save, SIGNAL(pressed()), this, SLOT(save()));
    connect(ui->channel, SIGNAL(currentIndexChanged(int)),this,SLOT(channel(int)));
  connect(ui->out_channel,SIGNAL(activated(int)),this,SLOT(out_chan(int)));
  connect(ui->out_channel_2,SIGNAL(activated(int)),this,SLOT(out_chan_2(int)));
  connect(ui->out_channel_3,SIGNAL(activated(int)),this,SLOT(out_chan_3(int)));
  connect(ui->merge,SIGNAL(currentIndexChanged(int)),this,SLOT(Merge(int)));

  connect(ui->GrayCheck,SIGNAL(stateChanged(int)),this,SLOT(CheckGray(int)));
  connect(ui->HSVCheck,SIGNAL(stateChanged(int)),this,SLOT(CheckHSV(int)));

	  ui->hmin->setSliderPosition(HSV[0]);
  connect(ui->hmin,SIGNAL(valueChanged(int)),this,SLOT(h(int)));
	  ui->smin->setSliderPosition(HSV[1]);
  connect(ui->smin,SIGNAL(valueChanged(int)),this,SLOT(s(int)));
	  ui->vmin->setSliderPosition(HSV[2]);
  connect(ui->vmin,SIGNAL(valueChanged(int)),this,SLOT(v(int)));
	  ui->hmax->setSliderPosition(HSV[3]);
  connect(ui->hmax,SIGNAL(valueChanged(int)),this,SLOT(H(int)));
	  ui->smax->setSliderPosition(HSV[4]);
  connect(ui->smax,SIGNAL(valueChanged(int)),this,SLOT(S(int)));
	  ui->vmax->setSliderPosition(HSV[5]);
  connect(ui->vmax,SIGNAL(valueChanged(int)),this,SLOT(V(int)));
	  ui->hmin_2->setSliderPosition(HSV[6]);
  connect(ui->hmin_2,SIGNAL(valueChanged(int)),this,SLOT(h2(int)));
	  ui->smin_2->setSliderPosition(HSV[7]);
  connect(ui->smin_2,SIGNAL(valueChanged(int)),this,SLOT(s2(int)));
	  ui->vmin_2->setSliderPosition(HSV[8]);
  connect(ui->vmin_2,SIGNAL(valueChanged(int)),this,SLOT(v2(int)));
	  ui->hmax_2->setSliderPosition(HSV[9]);
  connect(ui->hmax_2,SIGNAL(valueChanged(int)),this,SLOT(H2(int)));
	  ui->smax_2->setSliderPosition(HSV[10]);
  connect(ui->smax_2,SIGNAL(valueChanged(int)),this,SLOT(S2(int)));
	  ui->vmax_2->setSliderPosition(HSV[11]);
  connect(ui->vmax_2,SIGNAL(valueChanged(int)),this,SLOT(V2(int)));
	  ui->sobel->setSliderPosition(S_sob);
  connect(ui->sobel,SIGNAL(valueChanged(int)),this,SLOT(sob(int)));
    ui->sob_thres->setSliderPosition(th_sob);
connect(ui->sob_thres,SIGNAL(valueChanged(int)),this,SLOT(sob_thres(int)));
	  ui->open_morph->setSliderPosition(S_op);
  connect(ui->open_morph,SIGNAL(valueChanged(int)),this,SLOT(opn(int)));
	  ui->HL_th->setSliderPosition(threshold);
  connect(ui->HL_th,SIGNAL(valueChanged(int)),this,SLOT(Threshold(int)));
	  ui->HL_mll->setSliderPosition(minLineLength);
  connect(ui->HL_mll,SIGNAL(valueChanged(int)),this,SLOT(minLine(int)));
	  ui->HL_mlg->setSliderPosition(minLineGap);
  connect(ui->HL_mlg,SIGNAL(valueChanged(int)),this,SLOT(minGap(int)));
  ui->median_kernal->setSliderPosition(med_kernal);
connect(ui->median_kernal,SIGNAL(valueChanged(int)),this,SLOT(median_ker(int)));
  //connect(ui->hmin,&QSlider::valueChanged(int),[this](int v){this->hsv(v,0);});


}


gate::~gate() { delete ui; }


void gate::hsv(int value,int index){HSV[index]=value;}
void gate::channel(int index){ch_index=index;}
void gate::h(int val){HSV[0]=val;}
void gate::s(int val){HSV[1]=val;}
void gate::v(int val){HSV[2]=val;}
void gate::H(int val){HSV[3]=val;}
void gate::S(int val){HSV[4]=val;}
void gate::V(int val){HSV[5]=val;}
void gate::h2(int val){HSV[6]=val;}
void gate::s2(int val){HSV[7]=val;}
void gate::v2(int val){HSV[8]=val;}
void gate::H2(int val){HSV[9]=val;}
void gate::S2(int val){HSV[10]=val;}
void gate::V2(int val){HSV[11]=val;}
void gate::opn(int val){S_op=val;}
void gate::sob(int val){S_sob=val;}
void gate::out_chan(int val){out_ch=val;}
void gate::out_chan_2(int val){out_ch=3+val;}
void gate::out_chan_3(int val){out_ch=8+val;}
void gate::Threshold(int val){threshold=val;}
void gate::minLine(int val){minLineLength=val;}
void gate::minGap(int val){minLineGap=val;}
void gate::sob_thres(int val){th_sob=val;}
void gate::median_ker(int val){med_kernal=val;}
void gate::CheckGray(int val)
{
  graycheck=val;
  ui->out_channel_2->setEnabled(graycheck);
  ui->out_channel_3->setEnabled(graycheck|hsvcheck);
  ui->merge->setEnabled(graycheck&hsvcheck);

}
void gate::CheckHSV(int val)
{
  hsvcheck=val;
  ui->out_channel->setEnabled(hsvcheck);
  ui->out_channel_3->setEnabled(graycheck|hsvcheck);
  ui->merge->setEnabled(graycheck&hsvcheck);
}
void gate::Merge(int val){merg=val;}

cv::Mat gate::gate2(cv::Mat img, cv::Mat* fin_img) ///suggested by vaibhav bhaiya
{
  cv::Mat dx,mask1,mask2,mask;
  cv::cvtColor(img, mask, cv::COLOR_BGR2GRAY);
  cv::Sobel(mask, dx,  CV_16S, 1,0, S_sob);
  cv::convertScaleAbs(dx, mask);
	if(out_ch==3)cv::cvtColor(mask, *fin_img, cv::COLOR_GRAY2RGB);
  cv::inRange(mask, th_sob, 255, mask1);
  if(out_ch==4)cv::cvtColor(mask1, *fin_img, cv::COLOR_GRAY2RGB);
	cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( S_op,S_op ),cv::Point(S_op/2,S_op/2) );
  cv::dilate(mask1, mask1, kernel);
  if(out_ch==5)cv::cvtColor(mask1, *fin_img, cv::COLOR_GRAY2RGB);
	cv::morphologyEx(mask1, mask1, cv::MORPH_OPEN, kernel);
  if(out_ch==6)cv::cvtColor(mask1, *fin_img, cv::COLOR_GRAY2RGB);
  cv::medianBlur(mask1, mask2, med_kernal);
  if(out_ch==7)cv::cvtColor(mask2, *fin_img, cv::COLOR_GRAY2RGB);
  return mask2;
}

cv::Mat gate::gate1(cv::Mat img , cv::Mat* fin_img)///made by sayansree paria
{
  cv::Mat dx,maskl,maskl2,maskr,maskr2,mask;
  cv::Scalar maxHSV,minHSV;
  cv::cvtColor(img, mask, cv::COLOR_BGR2HSV);
	if(HSV[0]>0){
        minHSV = cv::Scalar(HSV[0],HSV[1],HSV[2]);
        maxHSV = cv::Scalar(HSV[3],HSV[4],HSV[5]);
        cv::inRange(mask, minHSV, maxHSV, maskl);
	}else{
	      minHSV = cv::Scalar(0,HSV[1],HSV[2]);
        maxHSV = cv::Scalar(HSV[3],HSV[4],HSV[5]);
        cv::inRange(mask, minHSV, maxHSV, maskl);
	      minHSV = cv::Scalar(180+HSV[0],HSV[1],HSV[2]);
        maxHSV = cv::Scalar(180,HSV[4],HSV[5]);
        cv::inRange(mask, minHSV, maxHSV, maskl2);
	      maskl=maskl|maskl2;
	}
	if(HSV[6]>0){
	      minHSV = cv::Scalar(HSV[6],HSV[7],HSV[8]);
        maxHSV = cv::Scalar(HSV[9],HSV[10],HSV[11]);
        cv::inRange(mask, minHSV, maxHSV, maskr);
	}else{
	      minHSV = cv::Scalar(0,HSV[7],HSV[8]);
        maxHSV = cv::Scalar(HSV[9],HSV[10],HSV[11]);
        cv::inRange(mask, minHSV, maxHSV, maskr);
	      minHSV = cv::Scalar(180+HSV[6],HSV[7],HSV[8]);
        maxHSV = cv::Scalar(180,HSV[10],HSV[11]);
        cv::inRange(mask, minHSV, maxHSV, maskr2);
	      maskr=maskr|maskr2;
	}
	mask=maskl|maskr;
	if(out_ch==0)cv::cvtColor(mask, *fin_img, cv::COLOR_GRAY2RGB);
	cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( S_op,S_op ),cv::Point(S_op/2,S_op/2) );
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
	if(out_ch==1)cv::cvtColor(mask, *fin_img, cv::COLOR_GRAY2RGB);
	cv::Sobel(mask, dx,  CV_16S, 1,0, S_sob);
  cv::convertScaleAbs(dx, mask);
	if(out_ch==2)cv::cvtColor(mask, *fin_img, cv::COLOR_GRAY2RGB);

  return mask;
}

void gate::feed(cv::Mat img)
{

if(!img.empty()) {
	cv::Mat mask,fin_img,img2,img1,imgc;

	channel_show(img);
  cv::GaussianBlur( img, mask, cv::Size( 9, 9 ), 0, 0 );
  //model selection
  if(hsvcheck)
  img1=gate1(mask,&fin_img);
  if(graycheck)
  img2=gate2(mask,&fin_img);
  //merging
  if(graycheck&&hsvcheck)
  {
  switch(merg)
  {
    case 0:imgc=img1|img2;break;
    case 1:imgc=img1&img2;break;
    case 2:imgc=img1^img2;break;
  }}
  else if(graycheck)
  imgc=img2;
  else if (hsvcheck)
  imgc=img1;
  else return;
  if(out_ch==8)
   cv::cvtColor(imgc,fin_img,cv::COLOR_GRAY2RGB);
	std::vector <cv::Vec4i> lines;
  cv::HoughLinesP(imgc, lines, 1, CV_PI/180, threshold,minLineLength,minLineGap );
		for( size_t i = 0; i < lines.size(); i++ )
	    	{

			cv::Vec4i l = lines[i];
			int x1=l[0],x2=l[2],y1=l[1],y2=l[3];
			double th=(x1==x2)?90:atan(1.0*(y2-y1)/(x2-x1))*180/CV_PI;
			if(abs(th)>83)
			{	int xx=(x1+x2)/2;
				line( img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
			}

	    	}
	 if(out_ch==9)
		cv::cvtColor(img,fin_img,cv::COLOR_BGR2RGB);

    ui->out->setPixmap(QPixmap::fromImage(QImage(fin_img.data, fin_img.cols, fin_img.rows,fin_img.step, QImage::Format_RGB888)).scaled(ui->out->size()));

    }
}

void gate::save()
{

	outf.open(CONFIG_PATH,std::ios::out);
	for(int i=0;i<12;i++)
	outf<<HSV[i]<<std::endl;
	outf<<S_op<<std::endl;
	outf<<S_sob<<std::endl;
	outf<<threshold<<std::endl;
	outf<<minLineLength<<std::endl;
	outf<<minLineGap<<std::endl;
  outf<<th_sob<<std::endl;
  outf<<med_kernal<<std::endl;
	outf.close();
}
void gate::channel_show(cv::Mat img)
{
	std::vector<cv::Mat> Channels(3);
   	 cv::Mat zero, fin_img;
    	 zero = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_8UC1);
	switch(ch_index)
	{
		case 0:
	 	cv::cvtColor(img, fin_img, CV_BGR2RGB);
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
		cv::cvtColor(img, fin_img, CV_BGR2HSV);
		split(img, Channels);
		Channels[1]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;

		case 6:
		cv::cvtColor(img, fin_img, CV_BGR2HSV);
		split(img, Channels);
		Channels[0]=zero;
		Channels[2]=zero;
		merge(Channels, fin_img);
		break;

		case 7:
		cv::cvtColor(img, fin_img, CV_BGR2HSV);
		split(img, Channels);
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
        ui->channel_show->setPixmap(QPixmap::fromImage(QImage(fin_img.data, fin_img.cols, fin_img.rows,fin_img.step, QImage::Format_RGB888)).scaled(ui->channel_show->size()));
}
