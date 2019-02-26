#include "mainwindow.h"
#include <QtGui/QApplication>

//(GSCD)Geographical System Connection Diagram,电力系统地理接线图
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QObject::tr("GSCD"));
	a.setApplicationVersion(QObject::tr("X00.00.08"));

	QString szMap,szRaw,szPF;
	QStringList	argvs = QApplication::arguments();
	int count = argvs.count();
	if(count > 2)
	{
		int idx = argvs.indexOf("-map");
		if(count > (idx+1))
			szMap = argvs.at(idx+1);
		idx = argvs.indexOf("-data");
		if(count > (idx+1))
			szRaw = argvs.at(idx+1);
		idx = argvs.indexOf("-pf");
		if(count > (idx+1))
			szPF = argvs.at(idx+1);
	}
	MainWindow w;
	w.show();

	if(szMap.isEmpty() == false && szRaw.isEmpty() == false && szPF.isEmpty() == false)				//with command line
	{
		w.openCmdFiles(szMap,szRaw,szPF);
	}
	return a.exec();
}
