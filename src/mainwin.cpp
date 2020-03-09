//written by sayansree paria
#include "tiburon_gui/mainwin.h"
#include "ui_mainwin.h"
#include <iostream>
#include <cmath>
mainwin::mainwin(image_transport::ImageTransport *it,QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwin),it(it){
  LOGO_PATH = ros::package::getPath("tiburon_gui") + "/utils/logo.jpeg";
  VIDEO_PATH = ros::package::getPath("tiburon_gui")+"/src/gate.avi";
  FRONT_CAM_PATH =  "/front_camera/image_rect_color";
  BOTTOM_CAM_PATH=  "/bottom_camera/image_rect_color";

 	ui->setupUi(this);

	//ros::NodeHandle nh;

 	sub = it->subscribe("cam1", 1, mainwin::rosimg);

	ui->video_text->setText(QString::fromStdString(VIDEO_PATH));
  ui->front_cam_text->setText(QString::fromStdString(FRONT_CAM_PATH));
  ui->bottom_cam_text->setText(QString::fromStdString(BOTTOM_CAM_PATH));
	timer = new QTimer(this);
	cap.open(VIDEO_PATH);
  	cv::Mat frame_save=cv::imread(LOGO_PATH);
  	cv::cvtColor(frame_save, frame_save, CV_BGR2RGB);
  	ui->logo->setPixmap(QPixmap::fromImage(QImage(frame_save.data, frame_save.cols, frame_save.rows,frame_save.step, QImage::Format_RGB888)).scaled(ui->logo->size()));
  play_mode=false;
	connect(timer, SIGNAL(timeout()),this,SLOT(loop()));
	connect(ui->rate, SIGNAL(currentIndexChanged(int)),this,SLOT(play_speed(int)));
	connect(ui->inputtype, SIGNAL(currentIndexChanged(int)),this,SLOT(inp(int)));
  connect(ui->feed_control, SIGNAL(currentIndexChanged(int)),this,SLOT(feed_cont(int)));
	connect(ui->play, SIGNAL(pressed()), this, SLOT(play()));
	connect(ui->load_video, SIGNAL(pressed()), this, SLOT(load()));
  connect(ui->load_video_2, SIGNAL(pressed()), this, SLOT(load2()));
  connect(ui->load_video_3, SIGNAL(pressed()), this, SLOT(load3()));
  connect(ui->gate, SIGNAL(pressed()), this, SLOT(Gate()));
  connect(ui->redBucket, SIGNAL(pressed()), this, SLOT(RedBucket()));
	connect(ui->blueBucket, SIGNAL(pressed()), this, SLOT(BlueBucket()));
	connect(ui->redFlare, SIGNAL(pressed()), this, SLOT(RedFlare()));
	connect(ui->yellowFlare, SIGNAL(pressed()), this, SLOT(YellowFlare()));
  timer->start(speed);


}

mainwin::~mainwin() { delete ui; }

void mainwin::feed_cont(int val){BottomCam=val;cam_change();}
void mainwin::hide(){
  ui->load_video->setEnabled(!rosf);
  ui->video_text->setEnabled(!rosf);
  ui->load_video_2->setEnabled(rosf);
  ui->load_video_3->setEnabled(rosf);
  ui->front_cam_text->setEnabled(rosf);
  ui->bottom_cam_text->setEnabled(rosf);
  ui->feed_control->setEnabled(rosf);
}
void mainwin::cam_change()
{
  sub.shutdown();
  if(BottomCam)
  sub = it->subscribe(BOTTOM_CAM_PATH, 1, mainwin::rosimg);
  else
  sub = it->subscribe(FRONT_CAM_PATH, 1, mainwin::rosimg);

}
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
    if(play_mode)
	frame= cv_ptr->image;
}

void mainwin::inp(int val)
{
	if(val==0)
	{
		rosf=false;
    sub.shutdown();
		cap.open(VIDEO_PATH);
	}else{
		rosf=true;
		cap.release();
    cam_change();
	}
  hide();
}

void mainwin::play_speed(int val)
{
	speed=(int)60*pow(2,2-val);
	if(timer->isActive())
	timer->start(speed);
}
void mainwin::play()
{

if (play_mode){	// if(timer->isActive())
	// {
	//	timer->stop();    //feed continues with still frame thresholding
		ui->play->setText("play");

	}else{
	//	timer->start(speed);
		ui->play->setText("pause");

	}
  play_mode=!play_mode;
}
void mainwin::load()
{

	VIDEO_PATH=ui->video_text->text().toStdString();
	if(!rosf)
  {cap.release();
    cap.open(VIDEO_PATH);}
}
void mainwin::load2()
{
	FRONT_CAM_PATH=ui->front_cam_text->text().toStdString();
	if(rosf&&!BottomCam)cam_change();
}
void mainwin::load3()
{
	cap.release();
	BOTTOM_CAM_PATH=ui->bottom_cam_text->text().toStdString();
if(rosf&&BottomCam)cam_change();
}
void mainwin::loop()
{
  cv::Mat src;
  	if( !rosf&& cap.isOpened()&&play_mode)
		 cap >> frame;
     if(!frame.empty()) {
  if(gateui>0)
    gateui->feed(frame.clone());
  if(redbucketui>0)
    redbucketui->feed(frame.clone());
  if(bluebucketui>0)
    bluebucketui->feed(frame.clone());
  if(redflareui>0)
    redflareui->feed(frame.clone());
  if(yellowflareui>0)
    yellowflareui->feed(frame.clone());
    cv::cvtColor(frame, src, CV_BGR2RGB);
  ui->vid->setPixmap(QPixmap::fromImage(QImage(src.data, src.cols, src.rows,src.step, QImage::Format_RGB888)).scaled(ui->vid->size()));
    	// 	 if(!src.empty()) {
			// if(gateui>0)
			// 	gateui->feed(src.clone());
			// if(redbucketui>0)
			// 	redbucketui->feed(src.clone());
			// if(bluebucketui>0)
			// 	bluebucketui->feed(src.clone());
			// if(redflareui>0)
			// 	redflareui->feed(src.clone());
			// if(yellowflareui>0)
			// 	yellowflareui->feed(src.clone());

			//  cv::cvtColor(src, src, CV_BGR2RGB);
			// ui->vid->setPixmap(QPixmap::fromImage(QImage(src.data, src.cols, src.rows,src.step, QImage::Format_RGB888)).scaled(ui->vid->size()));


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
