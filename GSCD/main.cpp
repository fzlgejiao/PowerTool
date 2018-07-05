#include "mainwindow.h"
#include <QtGui/QApplication>

//(GWD)Geographical wiring diagram 
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();

}
