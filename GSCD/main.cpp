#include "mainwindow.h"
#include <QtGui/QApplication>

//(GWD)Geographical wiring diagram 
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QObject::tr("GWD"));
	a.setApplicationVersion(QObject::tr("X00.00.01"));
	MainWindow w;
	w.show();
	return a.exec();
}
