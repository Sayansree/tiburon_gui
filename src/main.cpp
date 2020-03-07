//written by Sayansree paria
#include "tiburon_gui/mainwin.h"
#include <QApplication>
int main(int argc, char *argv[]) {
	ros::init(argc, argv, "tiburon_gui");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	//image_transport::Subscriber subb = it.subscribe("/bottom_camera/image_rect_color", 1, mainwin::rosimg);
  	QApplication a(argc, argv);
  	mainwin w(&it);
  	w.show();
	while(w.isVisible())
    	{
        ros::spinOnce();
        a.processEvents();
   	 }
  	return 0;
}
