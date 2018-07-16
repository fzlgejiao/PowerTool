#include "idoc.h"

iDoc::iDoc(QObject *parent)
	: QObject(parent)
{

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