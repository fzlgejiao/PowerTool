#include "mainwindow.h"
#include <QtGui/QApplication>

//(GSCD)Geographical System Connection Diagram,电力系统地理接线图
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QObject::tr("GSCD"));
	a.setApplicationVersion(QObject::tr("X00.00.07"));
	MainWindow w;
	w.show();
	return a.exec();
}
