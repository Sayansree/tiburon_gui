#include "tiburon_gui/mainwin.h"
#include "ui_mainwin.h"
#include <iostream>
#include <cmath>
mainwin::mainwin(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwin){
  LOGO_PATH = ros::package::getPath("tiburon_gui") + "/utils/logo.jpeg";
  VIDEO_PATH = ros::package::getPath("tiburon_gui")+"/src/gate.avi";
  //ROS_PATH =  "cam1";
	
 	ui->setupUi(this);
	
	//ros::NodeHandle nh;
  	//image_transport::Subscriber sub;
 	//sub = it.subscribe("cam1", 1, rosimg);
		
	ui->video_text->setText(QString::fromStdString(VIDEO_PATH));
	timer = new QTimer(this);
	cap.open(VIDEO_PATH);
  	cv::Mat frame_save=cv::imread(LOGO_PATH);
  	cv::cvtColor(frame_save, frame_save, CV_BGR2RGB);
  	ui->logo->setPixmap(QPixmap::fromImage(QImage(frame_save.data, frame_save.cols, frame_save.rows,frame_save.step, QImage::Format_RGB888)));

	connect(timer, SIGNAL(timeout()),this,SLOT(loop()));
	connect(ui->rate, SIGNAL(currentIndexChanged(int)),this,SLOT(play_speed(int)));
	connect(ui->inputtype, SIGNAL(currentIndexChanged(int)),this,SLOT(inp(int)));
	connect(ui->play, SIGNAL(pressed()), this, SLOT(play()));
	connect(ui->load_video, SIGNAL(pressed()), this, SLOT(load()));
  	connect(ui->gate, SIGNAL(pressed()), this, SLOT(Gate()));
  	connect(ui->redBucket, SIGNAL(pressed()), this, SLOT(RedBucket()));
	connect(ui->blueBucket, SIGNAL(pressed()), this, SLOT(BlueBucket()));
	connect(ui->redFlare, SIGNAL(pressed()), this, SLOT(RedFlare()));
	connect(ui->yellowFlare, SIGNAL(pressed()), this, SLOT(YellowFlare()));

 

}

mainwin::~mainwin() { delete ui; }
 void mainwin::rosimg(const sensor_msgs::ImageConstPtr& msg)
 {
	cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
	return;
    }
	frame= cv_ptr->image;
}

void mainwin::inp(int val)
{
	if(val==0)
	{
		rosf=false;
		cap.open(VIDEO_PATH);
	}else{
		rosf=true;
		cap.release();
	}
}

void mainwin::play_speed(int val)
{
	speed=(int)60*pow(2,2-val);
	if(timer->isActive())
	timer->start(speed);
}
void mainwin::play()
{
	if(timer->isActive())
	{
		timer->stop();
		ui->play->setText("play");
	}else{
		timer->start(speed);
		ui->play->setText("pause");
	}
}
void mainwin::load()
{
	cap.release();
	VIDEO_PATH=ui->video_text->text().toStdString();
	cap.open(VIDEO_PATH);
}
void mainwin::loop()
{
	cv::Mat src;
  	if( rosf)
	{
    		 if(!frame.empty()) {
	    		cv::cvtColor(frame, src, CV_BGR2RGB);
			if(gateui>0)
				gateui->feed(frame);
 		 	
			ui->vid->setPixmap(QPixmap::fromImage(QImage(src.data, src.cols, src.rows,src.step, QImage::Format_RGB888)));
			
		}

	}else if( cap.isOpened())
	{
		
		 cap >> src;
    		 if(!src.empty()) {
			if(gateui>0)
				gateui->feed(src);
 		 	cv::cvtColor(src, src, CV_BGR2RGB);
			ui->vid->setPixmap(QPixmap::fromImage(QImage(src.data, src.cols, src.rows,src.step, QImage::Format_RGB888)));
			
		}

	}
}


void mainwin::Gate()
{
	if(gateui>0){
	gateui->close();
	gateui=NULL;
	}else{
	gateui=new gate();
	gateui->show();
	}
}
void mainwin::RedBucket()
{
	if(redbucketui>0){
	redbucketui->close();
	redbucketui=NULL;
	}else{

	redbucketui=new redbucket();
	redbucketui->show();
	}
}
void mainwin::BlueBucket()
{
	if(bluebucketui>0){
	bluebucketui->close();
	bluebucketui=NULL;
	}else{
	bluebucketui=new bluebucket();
	bluebucketui->show();
	}
}void mainwin::RedFlare()
{
	if(redflareui>0){
	redflareui->close();
	redflareui=NULL;
	}else{
	redflareui=new redflare();
	redflareui->show();
	}
}void mainwin::YellowFlare()
{
	if(yellowflareui>0){
	yellowflareui->close();
	yellowflareui=NULL;
	}else{
	yellowflareui=new yellowflare();
	yellowflareui->show();
	}
}
