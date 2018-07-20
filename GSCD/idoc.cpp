#include "idoc.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>

iDoc::iDoc(QObject *parent)
	: QObject(parent)
	,Prefix_keyword("BEGIN ")
	,Suffix_keyword("0 / END OF ")
	,ColumnName_keyword("@!")
{
	//test code
	listBUS.insert(1,new iBUS(1,"bus1", this));
	listBUS.insert(2,new iBUS(2,"bus2", this));
}

iDoc::~iDoc()
{

}


bool iDoc::openDataFile(const QString& file)
{
	//todo:: process data file
	if(OpenDataFile(file))
	{
	GetDataModel(T_BUS);	
	GetDataModel(T_BRANCH);
	GetDataModel(T_TRANSFORMER);
	CloseDataFile();
	}
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
	CloseDataFile();
}


bool iDoc::OpenDataFile(const QString& file )
{
	m_file=new QFile(file);
	if(m_file->open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		return true;
	}else
	{
		return false;
	}

}

void iDoc::CloseDataFile()
{
	if(m_file->isOpen())
	{		
		m_file->close();
	}
}

void iDoc::GetDataModel(T_DATA datatype)
{
	int datarows=0;		
	QString dataname;
	if(datatype==T_BUS)
		dataname="BUS";
	else if(datatype==T_BRANCH)
		dataname="BRANCH";
	else if(datatype==T_TRANSFORMER)
		dataname="TRANSFORMER";

	QTextStream stream(m_file);
	stream.seek(0);
	while(!stream.atEnd())
	{
		QString Beginingline=stream.readLine();
		if(Beginingline.contains(Prefix_keyword+dataname))
		{
				QString readline=stream.readLine();
				QStringList columnames;
				int columnCnt=0;
				int RowsCnt=0;
				while(readline.startsWith(ColumnName_keyword))									//Get columns 
				{				
					columnames.append(readline.remove(ColumnName_keyword).split(",",QString::SkipEmptyParts));
					columnCnt+=columnames.count();
					readline=stream.readLine();
					RowsCnt++;
				}
										
				//To Do: Next Get the Datas				
				while(!readline.startsWith(Suffix_keyword))
				{
					datarows++;
					//insert values
					QStringList datalist=readline.split(",",QString::SkipEmptyParts);
					switch(datatype)
					{
					case T_BUS:
						addBUS(new iBUS(datalist[0].toInt(),datalist[1].replace(QString("'"),QString("")).trimmed(),this));
					break;

					case T_BRANCH:
						addBRANCH(new iBRANCH(datarows,this));										//the ID maybe to index
						listBRANCH[datarows]->set2FromBUS(datalist[0].toInt());
						listBRANCH[datarows]->set2ToBUS(datalist[1].toInt());					
					break;

					case T_TRANSFORMER:
						addTRANSFORMER(new iTRANSFORMER(datalist[0].toInt(),this));
						break;
					}
					readline=stream.readLine();
				}
				 break;
		}
	}
}


void iDoc::GetBUSData(const QString& dataname)
{	
	
}

void iDoc::GetBRANCHData(const QString& dataname)
{
	
}

void iDoc::GetTRANSMORMERData(const QString& dataname)
{
	
}
void	iDoc::getAvailableBus(QList<iBUS *>& list)
{
	foreach(iBUS *bus,listBUS)
	{
		if(bus->isAdded() == false)
			list.append(bus);
	}
}