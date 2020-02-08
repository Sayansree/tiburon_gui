#include "tiburon_gui/mainwin.h"
#include <QApplication>
int main(int argc, char *argv[]) {
	ros::init(argc, argv, "tiburon_gui");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
  	image_transport::Subscriber sub = it.subscribe("cam1", 1, mainwin::rosimg);
  	QApplication a(argc, argv);
  	mainwin w;
  	w.show();
	while(w.isVisible())
    	{
        ros::spinOnce();
        a.processEvents();
   	 }
  	return 0;
}
