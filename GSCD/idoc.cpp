#include "idoc.h"

iDoc::iDoc(QObject *parent)
	: QObject(parent)
{
	//test code
	listBUS.insert(1,new iBUS(1,"bus1", this));
	listBUS.insert(2,new iBUS(2,"bus2", this));
}

iDoc::~iDoc()
{

}
bool	iDoc::openDataFile(const QString& file)
{

	//todo:: process data file

	return true;
}
bool	iDoc::openMapFile(const QString& file)
{
	//todo:: process map file

	return true;
}
void	iDoc::close()
{
	qDeleteAll(listBUS);
	listBUS.clear();
	qDeleteAll(listBRANCH);
	listBRANCH.clear();
	qDeleteAll(listTRANSFORMER);
	listTRANSFORMER.clear();
}
void	iDoc::getAvailableBus(QList<iBUS *>& list)
{
	foreach(iBUS *bus,listBUS)
	{
		if(bus->isAdded() == false)
			list.append(bus);
	}
}