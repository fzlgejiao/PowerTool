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

}

iDoc::~iDoc()
{

}
void iDoc::test()
{
	//test code
	iBUS* bus1 = new iBUS(1,"bus1", this);
	iBUS* bus2 = new iBUS(2,"bus2", this);
	iBUS* bus3 = new iBUS(3,"bus3", this);
	iBRANCH* branch1 = new iBRANCH(1,1,3,this);														//branch1: bus1->bus3
	bus1->addLink(branch1);
	bus3->addLink(branch1);

	iBRANCH* branch2 = new iBRANCH(2,3,2,this);														//branch2: bus3->bus2
	bus3->addLink(branch2);
	bus2->addLink(branch2);

	listBUS.insert(1,bus1);
	listBUS.insert(2,bus2);
	listBUS.insert(3,bus3);
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
	int TranformerDummylines=0;
	QString dataname;
	if(datatype==T_BUS)
		dataname="BUS";
	else if(datatype==T_BRANCH)
		dataname="BRANCH";
	else if(datatype==T_TRANSFORMER)
		dataname="TRANSFORMER";
	else return;
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
						{
							int from,to;
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							iBRANCH* branch = new iBRANCH(datarows,from,to,this);					//row index is the branch id

							//add branch into bus link list
							iBUS* bus1 = getBUS(from);
							if(bus1)
								bus1->addLink(branch);
							iBUS* bus2 = getBUS(to);
							if(bus2)
								bus2->addLink(branch);
							addBRANCH(branch);														//add branch into branch list
						}
					break;

					case T_TRANSFORMER:																//Transformer Data Area 
						{
							int from,to;
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							iTRANSFORMER* transformer = new iTRANSFORMER(datarows,from,to,this);	//row index is the transformer id

							//add transformer into bus link list
							iBUS* bus1 = getBUS(from);
							if(bus1)
								bus1->addLink(transformer);
							iBUS* bus2 = getBUS(to);
							if(bus2)
								bus2->addLink(transformer);
							addTRANSFORMER(transformer);											//add transformer into transformer list
				
							int k=datalist[2].toInt();
							if(k==0)
							{
								TranformerDummylines=3;
							}else TranformerDummylines=4;

							for(int i=0;i<TranformerDummylines;i++)
								stream.readLine();
						}
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
iData*	iDoc::Uid2Data(int uid)
{
	int type = Uid2Type(uid);
	int id	 = Uid2Id(uid);
	switch(type)
	{
	case T_BUS:
		return getBUS(id);
	case T_BRANCH:
		return getBRANCH(id);
	case T_TRANSFORMER:
		return getTRANSFORMER(id);
	}
	return NULL;
}