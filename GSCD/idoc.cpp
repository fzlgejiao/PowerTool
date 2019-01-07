#include "idoc.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QXmlStreamWriter>
#include <iostream>
#include "diagramitem.h"
#include "diagramtextitem.h"
#include "diagramnoteitem.h"
#include <QChar>
#include <QFileInfo>

iDoc::iDoc(QObject *parent)
	: QObject(parent)
	,Prefix_keyword("BEGIN ")
	,Suffix_keyword("0 / END OF ")
	,ColumnName_keyword("@!")
{
	// my   control panle default settings
	m_controlpanel.showtype=SHOW_POWERFLOW;
	m_controlpanel.isShowStationName=true;
	m_controlpanel.isShowStationValue=true;
	m_controlpanel.isShowBranchLine=true;
	m_controlpanel.isShowBranchValue=true;
	m_controlpanel.isShowReactivePowerValue=true;
	m_controlpanel.isShowAdmittance=false;
	m_controlpanel.isShowVoltageAngle=false;
	m_controlpanel.isShowAllNodeVoltage=false;
	m_controlpanel.unittype=UNIT_ACTUALVALUE;
	
	m_AreaSize=QSize(297,210);
	SBase=1;
	m_wanningmessage.clear();
}

iDoc::~iDoc()
{
	qDeleteAll(listSLINK);																			//already deleted in iSTAT::removeSlinks
	qDeleteAll(listSTAT);
	qDeleteAll(listBUS);
	qDeleteAll(listBRANCH);
	qDeleteAll(listTRANSFORMER);
	qDeleteAll(listAREA);
	qDeleteAll(listNotes);
	qDeleteAll(listFACTSDEVICE);
}
void iDoc::test()
{
	//test code
	iAREA* area1 = new iAREA(1,"area1",this);
	iAREA* area2 = new iAREA(2,"area2",this);

	iBUS* bus1 = new iBUS(1,1,"bus1", this);
	iBUS* bus2 = new iBUS(2,1,"bus2", this);
	iBUS* bus3 = new iBUS(3,1,"bus3", this);
	iBUS* bus4 = new iBUS(4,1,"bus4", this);
	iBUS* bus5 = new iBUS(5,1,"bus5", this);
	iBRANCH* branch1_1 = new iBRANCH(1,bus1->Uid(),bus3->Uid(),this);								//branch1_1: bus1->bus3,parallel_code=1
	branch1_1->frombus=bus1;
	branch1_1->tobus=bus3;
	branch1_1->ParallelCode=1;
	bus1->addLink(branch1_1);
	bus3->addLink(branch1_1);
	iBRANCH* branch1_2 = new iBRANCH(1,bus1->Uid(),bus3->Uid(),this);								//branch1_2: bus1->bus3,parallel_code=2
	branch1_2->frombus=bus1;
	branch1_2->tobus=bus3;
	branch1_2->ParallelCode=2;
	bus1->addLink(branch1_2);
	bus3->addLink(branch1_2);



	iBRANCH* branch2 = new iBRANCH(2,bus2->Uid(),bus4->Uid(),this);									//branch2: bus2->bus4
	branch2->frombus=bus2;
	branch2->tobus=bus4;
	branch2->ParallelCode=1;
	bus2->addLink(branch2);
	bus4->addLink(branch2);

	iBRANCH* branch3 = new iBRANCH(3,bus3->Uid(),bus5->Uid(),this);									//branch3: bus3->bus5
	branch3->frombus=bus3;
	branch3->tobus=bus5;
	branch3->ParallelCode=1;
	bus3->addLink(branch3);
	bus5->addLink(branch3);

	listBUS.insert(1,bus1);
	listBUS.insert(2,bus2);
	listBUS.insert(3,bus3);
	listBUS.insert(4,bus4);
	listBUS.insert(5,bus5);

	listAREA.insert(1,area1);
	listAREA.insert(2,area2);
	//iSTAT* stat1 = STAT_new("STAT1");
	//iSTAT* stat2 = STAT_new("STAT2");

	//QList<iNodeData*> listNodes1;
	//listNodes1.append(bus1);
	//stat1->setNodes(listNodes1);

	//QList<iNodeData*> listNodes2;
	//listNodes2.append(bus2);
	//listNodes2.append(bus3);
	//stat2->setNodes(listNodes2);

}

void	iDoc::close()
{
	qDeleteAll(listSTAT);
	listSTAT.clear();
	qDeleteAll(listBUS);
	listBUS.clear();
	qDeleteAll(listBRANCH);
	listBRANCH.clear();
	qDeleteAll(listTRANSFORMER);
	listTRANSFORMER.clear();
	qDeleteAll(listAREA);
	listAREA.clear();
	qDeleteAll(listNotes);
	listNotes.clear();
	qDeleteAll(listFACTSDEVICE);
	listFACTSDEVICE.clear();
}

bool iDoc::readDataFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

	//deal with data file
	GetBaseParameter(file);
	GetDataModel(file,T_BUS);	
	GetDataModel(file,T_LOAD);	
	GetDataModel(file,T_COMPENSATION);
	GetDataModel(file,T_GENERATOR);
	GetDataModel(file,T_BRANCH);
	GetDataModel(file,T_TRANSFORMER);
	GetDataModel(file,T_AREA);
	GetDataModel(file,T_FACTSDEVICE);
	file.close();
	
	setDataFile(fileName);																			//set current opened data file
    return true;
}

bool iDoc::readPfFile()
{
	if(m_szDataFile.isNull()) return false;
	QFileInfo fi(m_szDataFile);
	//to do ; find the Power flow file . must the same directory and same name, but different extension name (*.txt)
	QString extension=fi.suffix();
	QString pffile=fi.absoluteFilePath().replace(extension,"txt");

	if(QFile::exists(pffile))
	{
		ParserPowerFlow(pffile);
		setPfFile(pffile);
		return true;
	}
	else	
	{
		m_wanningmessage.append(tr("Can't found power flow file,must have the same directory of the data file,and has <*.txt> extension!"));		
		//QMessageBox::warning(NULL,tr("Message Log"),tr("Can't found power flow file,must have the same directory of the data file,and has <*.txt> extension"), QMessageBox::Yes,QMessageBox::No);
		return false;
	}	
}

void iDoc::GetBaseParameter(QFile& file)
{
	QTextStream stream(&file);
	stream.seek(0);
	while(!stream.atEnd())
	{
		QString readline=stream.readLine();
		if(readline.contains(ColumnName_keyword)&&(readline.contains("SBASE")))
		{
			readline=stream.readLine();
			QStringList datalist=readline.split(",",QString::SkipEmptyParts);
			SBase=datalist[1].toDouble();
			break;
		}
	}
}
void iDoc::ParserPowerFlow(QString & filename)
{
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		std::cerr << "Error: Cannot read file " << qPrintable(filename)
			<< ": " << qPrintable(file.errorString())
			<< std::endl;
		return ;
	}
	int sts=0;
	int linenumber=0;
	QString stringline="";
	//QString nameccolumn="X-- NAME --X";
	//int namelength=nameccolumn.length();
	QString namestart="X-";
	QString nameend="-X";
	QString tranformercolumn="RATIO";
	int fromnamelength=0;
	int tonamelength=0;
	int tranformerlength=tranformercolumn.length();
	int fromnameindex=0;
	int tonameindex=0;
	int t_index=0;
	bool nameindexok=false;
	QString cutline="--------------------------------------------------------------";
	iBUS *frombus=NULL;
	QTextStream stream(&file);
	stream.seek(0);
	while(!stream.atEnd())
	{
		stringline=stream.readLine();
		linenumber++;
		if(stringline.contains(cutline))  sts=1;
		else if(stringline=="") sts=0;
		//else if(stringline.contains(nameccolumn)) sts=4;			
		else if(stringline.contains(nameend) && stringline.contains(namestart)) sts=4;			
		switch(sts)
		{
		case 4:
			{
				/*fromnameindex=stringline.indexOf(nameccolumn);
				tonameindex=stringline.lastIndexOf(nameccolumn);*/
				fromnameindex=stringline.indexOf(namestart);
				int fromnameindex1=stringline.indexOf(nameend);

				tonameindex=stringline.lastIndexOf(namestart);
				int tonameindex1=stringline.lastIndexOf(nameend);

				if(stringline.mid(fromnameindex,fromnameindex1-fromnameindex).contains("NAME"))
					fromnamelength=fromnameindex1-fromnameindex+nameend.length();

				if(stringline.mid(tonameindex,tonameindex1-tonameindex).contains("NAME"))
					tonamelength=tonameindex1-tonameindex+nameend.length();

				t_index=stringline.indexOf(tranformercolumn);
				if((fromnameindex!=0)&&(tonameindex!=0)&&(fromnamelength!=0)&&(tonamelength!=0))
					nameindexok=true;
			}
			break;

		case 0:		
			break;

		case 1:
			{
				//To do :update bus node
				//first get the name
				if(!nameindexok) continue;						
				QByteArray bytes=stringline.toLocal8Bit();				
				QByteArray bytsexcludename=bytes.remove(fromnameindex,fromnamelength);				
				stringline=QString(bytsexcludename);
				//QStringList datalist=stringline.replace(fromname,"").split(" ",QString::SkipEmptyParts);
				QStringList datalist=stringline.split(" ",QString::SkipEmptyParts);
				if(datalist.count()==9)
				{
					int fromid=datalist[0].toInt();		
					if(fromid==0) 
					{
						m_wanningmessage.append(QString(tr("Power Flow file->Line %1, invalid BUS ID number!")).arg(linenumber));
						continue;
					}
					float refvoltage=datalist[1].toFloat();
					float voltage=datalist[3].toFloat();
					float voltageangle=datalist[4].toFloat();
					float PowerActive=datalist[5].toFloat();
					float LoadActive=datalist[6].toFloat();
					float ShuntActive=datalist[7].toFloat();
					
					frombus=this->getBUS(fromid);
					if(frombus)
					{
						//if the PowerFlow file has the bus data ,update it from this file
						frombus->m_refvoltage=refvoltage;
						frombus->m_voltage=voltage;
						frombus->m_angle=voltageangle;
						frombus->power_PG=PowerActive/SBase;
						frombus->Load_PL=LoadActive/SBase;
						frombus->Compensation_GL=ShuntActive/SBase;
					}
				}else
				{
					m_wanningmessage.append(QString(tr("Power Flow file->Line %1, the line of contain cutoff line format error!")).arg(linenumber));
				}
				sts=2;
			}
			break;

		case 2:
			{
				//to do : update link data
				if(!nameindexok) continue;
				sts=3;

				QByteArray bytes=stringline.toLocal8Bit();
				QByteArray toname=bytes.mid(tonameindex,tonamelength).trimmed();
				QByteArray bytsexcludename=bytes.remove(tonameindex,tonamelength);
				stringline=QString(bytsexcludename);
				//QStringList tobusdata=stringline.replace(toname,"").split(" ",QString::SkipEmptyParts);
				QStringList tobusdata=stringline.split(" ",QString::SkipEmptyParts);
				if(tobusdata.count()<11) 
				{					
					m_wanningmessage.append(QString(tr("Power Flow file->Line %1, Link data line format error!")).arg(linenumber));
					continue;
				}
				QString PowerReActiveStr=tobusdata[2];								//it maybe has suffix, one letter
				if(PowerReActiveStr.at(PowerReActiveStr.length()-1).isLetter())
				{
					PowerReActiveStr.chop(1);
				}
				float PowerReActive=PowerReActiveStr.toFloat();
				float LoadReActive=tobusdata[3].toFloat();
				float ShuntReActive=tobusdata[4].toFloat();
				if(frombus)
				{
					//if the PowerFlow file has the bus data ,update it from this file
					frombus->Compensation_BL=ShuntReActive/SBase;
					frombus->Load_QL=LoadReActive/SBase;
					frombus->power_QG=PowerReActive/SBase;
				}
				//to do :update Bus data		

				int toid=tobusdata[5].toInt();	
				if(toid==0) 
				{
					m_wanningmessage.append(QString(tr("Power Flow file->Line %1, invalid BUS ID number!")).arg(linenumber));
					continue;
				}
				float refvoltage=tobusdata[6].toFloat();
				int ckt=tobusdata[8].toInt();				
				float P=tobusdata[9].toFloat();
				float Q=tobusdata[10].toFloat();
				

				iBUS *tobus=this->getBUS(toid);
				if(tobus) tobus->m_refvoltage=refvoltage;
				if(frombus && tobus)
				{					
					QString t_vaue=stringline.mid(t_index-toname.length(),tranformerlength).trimmed();
					if(t_vaue!="")
					{
						// search transformer
						iTRANSFORMER *transformer=getTRANSFORMER(frombus->Id(),toid);
						if(transformer)
						{
							if(transformer->getToBus()->Id()==toid)
							{
								transformer->m_P1=P/SBase;
								transformer->m_Q1=Q/SBase;
							}else
							{
								transformer->m_P2=P/SBase;
								transformer->m_Q2=Q/SBase;
							}
						}
					}else
					{
						//search branch
						iBRANCH *branch=getBRANCH(frombus->Id(),toid,ckt);
						if(branch)
						{
							if(branch->tobus->Id()==toid)
							{
								branch->m_P1=P/SBase;
								branch->m_Q1=Q/SBase;
							}else
							{
								branch->m_P2=P/SBase;
								branch->m_Q2=Q/SBase;
							}
						}
					}
				}
			}
			break;

		case 3:
			{
				if(!nameindexok) continue;
				QByteArray bytes=stringline.toLocal8Bit();
				QByteArray toname=bytes.mid(tonameindex,tonamelength).trimmed();
				QByteArray bytsexcludename=bytes.remove(tonameindex,tonamelength);
				stringline=QString(bytsexcludename);
				//todo : update link data, from second line
				//QStringList tobusdata=stringline.replace(toname,"").split(" ",QString::SkipEmptyParts);
				QStringList tobusdata=stringline.split(" ",QString::SkipEmptyParts);
				if(tobusdata.count()<6) 
				{					
					m_wanningmessage.append(QString(tr("Power Flow file->Line %1, Link data line format error!")).arg(linenumber));
					continue;
				}
				//to do :update to Bus data and link data							
				int toid=tobusdata[0].toInt();		
				if(toid==0) 
				{
					m_wanningmessage.append(QString(tr("Power Flow file->Line %1, invalid BUS ID number!")).arg(linenumber));
					continue;
				}
				float refvoltage=tobusdata[1].toFloat();
				int ckt=tobusdata[3].toInt();
				float P=tobusdata[4].toFloat();
				float Q=tobusdata[5].toFloat();
				if(!frombus) continue;
				QString t_vaue=stringline.mid(t_index-toname.length(),tranformerlength).trimmed();
				if(t_vaue!="")
				{
					// transformer
					iTRANSFORMER *transformer=getTRANSFORMER(frombus->Id(),toid);
					if(transformer)
					{
						if(transformer->getToBus()->Id()==toid)
						{
							transformer->m_P1=P/SBase;
							transformer->m_Q1=Q/SBase;
						}else
						{
							transformer->m_P2=P/SBase;
							transformer->m_Q2=Q/SBase;
						}
					}

				}else
				{
					//branch
					iBRANCH *branch=getBRANCH(frombus->Id(),toid,ckt);
					if(branch)
					{
						if(branch->tobus->Id()==toid)
						{
							branch->m_P1=P/SBase;
							branch->m_Q1=Q/SBase;
						}else
						{
							branch->m_P2=P/SBase;
							branch->m_Q2=Q/SBase;
						}
					}
				}

			}
			break;
		}	
	
	}

	file.close();
}
void iDoc::GetDataModel(QFile& file,T_DATA datatype)
{
	int datarows=0;		
	int linenumber=0;
	int TranformerDummylines=0;
	QString dataname;
	if(datatype==T_BUS)
		dataname="BUS";
	else if(datatype==T_GENERATOR)
		dataname="GENERATOR";
	else if(datatype==T_LOAD)
		dataname="LOAD";
	else if(datatype==T_COMPENSATION)
		dataname="FIXED SHUNT";
	else if(datatype==T_BRANCH)
		dataname="BRANCH";
	else if(datatype==T_TRANSFORMER)
		dataname="TRANSFORMER";
	else if(datatype==T_AREA)
		dataname="AREA";
	else if(datatype==T_FACTSDEVICE)
		dataname="FACTS DEVICE";
	else return;
	QTextStream stream(&file);
	stream.seek(0);
	while(!stream.atEnd())
	{
		QString Beginingline=stream.readLine();
		linenumber++;
		if(Beginingline.contains(Prefix_keyword+dataname))
		{
				QString readline=stream.readLine();
				linenumber++;
				QStringList columnames;
				int columnCnt=0;
				int RowsCnt=0;
				while(readline.startsWith(ColumnName_keyword))									//Get columns 
				{				
					columnames.append(readline.remove(ColumnName_keyword).split(",",QString::SkipEmptyParts));
					columnCnt+=columnames.count();
					readline=stream.readLine();
					linenumber++;
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
						{
							int areaid=datalist[4].toInt();
							double angel=datalist[8].toDouble();
							iBUS *bus=new iBUS(datalist[0].toInt(),areaid,datalist[1].replace(QString("'"),QString("")).trimmed(),this);
							bus->m_refvoltage=datalist[2].toDouble();
							bus->m_voltage=datalist[7].toDouble();
							bus->m_angle=angel;
							bus->sbase=SBase;
							addBUS(bus);
						}
					break;

					case T_LOAD:
					{
						int busid;
						double pl,ql,ip,iq;
						busid=datalist[0].toInt();
						pl=datalist[5].toDouble();
						ql=datalist[6].toDouble();
						ip=datalist[7].toDouble();
						iq=datalist[8].toDouble();
						iBUS* bus = getBUS(busid);
						if(bus)
						{
							bus->Load_PL=(pl+ip)/SBase;
							bus->Load_QL=(ql+iq)/SBase;
						}
					}
					break;

					case T_COMPENSATION:
					{
						int busid;
						double Gl,Bl;
						busid=datalist[0].toInt();
						Gl=datalist[3].toDouble();
						Bl=datalist[4].toDouble();
						iBUS* bus = getBUS(busid);
						if(bus)
						{
							bus->Compensation_GL=Gl/SBase;
							bus->Compensation_BL=Bl/SBase;
						}
					}
					break;

					case T_GENERATOR:
					{
						int busid;
						double pg,qg;
						busid=datalist[0].toInt();
						pg=datalist[2].toDouble();
						qg=datalist[3].toDouble();
						iBUS* bus = getBUS(busid);
						if(bus)
						{
							bus->power_PG=pg/SBase;
							bus->power_QG=qg/SBase;
						}
					}
					break;

					case T_BRANCH:
						{
							int from,to;
							double r,x;
							int onservice;
							QString CKT= datalist[2].replace(QString("'"),QString("")).trimmed();
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							r=datalist[3].toDouble();
							x=datalist[4].toDouble();
							onservice=datalist[23].toInt();
							iBUS* frombus = getBUS(from);
							iBUS* tobus = getBUS(to);
							if((frombus!=NULL)&&(tobus!=NULL))
							{
								iBRANCH* branch = new iBRANCH(datarows,frombus->Uid(),tobus->Uid(),this);					//row index is the branch id
								branch->frombus=frombus;
								branch->tobus=tobus;
								branch->ParallelCode=CKT.toInt();
								branch->branch_R=r;
								branch->branch_X=x;
									//add branch into bus link list	
								frombus->addLink(branch);	
								tobus->addLink(branch);
								if(onservice==1) branch->m_onservice=true;
								else if(onservice==0) branch->m_onservice=false;
								else m_wanningmessage.append(QString(tr("Raw File,Line %1 Unknown Branch stat,STAT=%2")).arg(linenumber).arg(datalist[23]));
								addBRANCH(branch);														//add branch into branch list
							}
						}
					break;

					case T_TRANSFORMER:																//Transformer Data Area 
						{
							int from,to,k,onservice;
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							k=datalist[2].toInt();
							onservice=datalist[11].toInt();
							iBUS* frombus = getBUS(from);
							iBUS* tobus = getBUS(to);
							if((frombus!=NULL)&&(tobus!=NULL))
							{
								iTRANSFORMER* transformer = new iTRANSFORMER(datarows,frombus->Uid(),tobus->Uid(),this);	//row index is the transformer id
								// transformer  add into linkdatas
								transformer->frombus=frombus;
								transformer->tobus=tobus;
								frombus->addLink(transformer);
								tobus->addLink(transformer);
								/*if(onservice==1) transformer->m_onservice=true;
								else if(onservice==0) transformer->m_onservice=false;
								else m_wanningmessage.append(QString(tr("Raw File,Line %1 Unknown Transformer STAT!")).arg(linenumber));*/
								addTRANSFORMER(transformer);											//add transformer into transformer list

								//Next read the transformer data line
								QStringList t_data=stream.readLine().split(",",QString::SkipEmptyParts);
								linenumber++;
								//if(t_data.count()==3)	//valid data
								{
									double t_sbase=t_data[2].toDouble();
									transformer->transformer_R=t_data[0].toDouble()*t_sbase/SBase;
									transformer->transformer_X=t_data[1].toDouble()*t_sbase/SBase;
								}
							}
							if(k==0)
							{
								TranformerDummylines=2;
							}else TranformerDummylines=3;

							for(int i=0;i<TranformerDummylines;i++)
								stream.readLine();
							linenumber+=TranformerDummylines;
						}
						break;

						case T_AREA:
						{
							int areaid=datalist[0].toInt();
							QString areaname=datalist[4].replace(QString("'"),QString("")).trimmed();
							iAREA *area=new iAREA(areaid,areaname,this);
							addAREA(area);
						}
						break;

						case T_FACTSDEVICE:
						{
							int from,to;
							QString devicename=datalist[0].remove(0,1);
							devicename.chop(1);
							from = datalist[1].toInt();
							to   = datalist[2].toInt();
							iBUS* frombus = getBUS(from);
							iBUS* tobus = getBUS(to);
							if((frombus!=NULL)&&(tobus!=NULL))
							{
								iFACTSDEVICE* factsdevice = new iFACTSDEVICE(datarows,frombus->Uid(),tobus->Uid(),this);
								factsdevice->devicename=devicename;
								factsdevice->frombus=frombus;
								factsdevice->tobus=tobus;
								frombus->addLink(factsdevice);
								tobus->addLink(factsdevice);
								addFACTSDEVICE(factsdevice);	
							}
						}
						break;
					}
					readline=stream.readLine();
					linenumber++;
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
void	iDoc::getAvailableNode(QList<iNodeData *>& list)
{
	foreach(iBUS *bus,listBUS)
	{
		if(bus->statId() == 0)
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

int	iDoc::STAT_getId()
{
	int id = 1;
	if(listSTAT.isEmpty())
		return id;
	foreach(iSTAT* stat, listSTAT)
	{
		if(stat->Id() > id)
			id = stat->Id();
	}
	return id+1;
}
iSTAT* iDoc::STAT_new(int id,const QString& name)
{
	iSTAT* stat = new iSTAT(id,name,this);
	connect(this,SIGNAL(controlpanelChanged(ControlPanel &,uint)),stat,SLOT(OncontrolpanelChanged(ControlPanel &,uint)));
	listSTAT.insert(stat->Id(),stat);
	return stat;
}
void iDoc::STAT_delete(int id)
{
	iSTAT* stat = STAT_get(id);
	if(stat)
		delete stat;
	listSTAT.remove(id);
}

int	iDoc::SLINK_getId()
{
	int id = 1;
	if(listSLINK.isEmpty())
		return id;
	foreach(iSLINK* statLINK, listSLINK)
	{
		if(statLINK->Id() > id)
			id = statLINK->Id();
	}
	return id+1;
}
iSLINK* iDoc::SLINK_new(iSTAT* startSTAT,iSTAT* endSTAT)
{
	iSLINK* statLINK = new iSLINK(SLINK_getId(),startSTAT,endSTAT,this);
	listSLINK.insert(statLINK->Id(),statLINK);
	connect(this,SIGNAL(controlpanelChanged(ControlPanel &,uint)),statLINK,SLOT(OncontrolpanelChanged(ControlPanel &,uint)));
	return statLINK;
}
void iDoc::SLINK_delete(int id)
{
	iSLINK* statLINK = SLINK_get(id);
	if(statLINK)
		delete statLINK;
	listSLINK.remove(id);
}
iSLINK* iDoc::SLINK_get(iSTAT* startSTAT,iSTAT* endSTAT)
{
	foreach(iSLINK* statLINK, listSLINK)
	{
		if((statLINK->m_startSTAT == startSTAT && statLINK->m_endSTAT == endSTAT)
		 ||(statLINK->m_startSTAT == endSTAT && statLINK->m_endSTAT == startSTAT))
			return statLINK;
	}
	return NULL;
}
iNodeData* iDoc::getNode(int uid)
{
	int type = Uid2Type(uid);
	if(type == T_NONE)
		return NULL;

	int id = Uid2Id(uid);
	return getNode(type,id);
}
iNodeData* iDoc::getNode(int type,int id)
{
	if(type == T_BUS)
		return getBUS(id);
	else
		return NULL;
}

iTRANSFORMER*	iDoc::getTRANSFORMER(int node1ID,int node2ID)
{
	QMapIterator<int, iTRANSFORMER *> it(listTRANSFORMER);
   while (it.hasNext()) 
   {
	   it.next();
	   int fromid=it.value()->frombus->Id();
	   int toid=it.value()->tobus->Id();

	   if( (fromid== node1ID) &&(toid==node2ID))
		   return it.value();
	    if( (toid== node1ID) &&(fromid==node2ID))
		   return it.value();
   }
	return NULL;
}
iBRANCH*	iDoc::getBRANCH(int node1ID,int node2ID,int ckt)
{
   QMapIterator<int, iBRANCH *> it(listBRANCH);
   while (it.hasNext()) 
   {
	   it.next();
	   int fromid=it.value()->frombus->Id();
	   int toid=it.value()->tobus->Id();
	   int p_code=it.value()->ParallelCode;

	   if( (fromid== node1ID) &&(toid==node2ID) && (p_code==ckt))
		   return it.value();
	    if( (toid== node1ID) &&(fromid==node2ID)&& (p_code==ckt))
		   return it.value();
   }
	return NULL;
}
iAREA*	iDoc::getAREA(const QString& name)
{
   QMapIterator<int, iAREA *> it(listAREA);
   while (it.hasNext()) 
   {
	   if(it.next().value()->name() == name)
		   return it.value();
   }
	return NULL;
}
void iDoc::setControlPanel(ControlPanel &value,uint changes)
{
	if(listSTAT.count()>0)
	{
		m_controlpanel=value;
		emit controlpanelChanged(m_controlpanel,changes);
	}
}

void iDoc::setAreaSize(const QSize &size)
{
	m_AreaSize=size; 
	emit areaSizeChanged(m_AreaSize);
}

int	iDoc::Note_getId()
{
	int id = 1;
	if(listNotes.isEmpty())
		return id;
	foreach(iNote* note, listNotes)
	{
		if(note->Id() > id)
			id = note->Id();
	}
	return id+1;
}

void iDoc::Note_delete(int id)
{
	iNote* note = Note_get(id);
	if(note)
		delete note;
	listNotes.remove(id);
}

iNote*	iDoc::Note_new(const QString& text)
{
	iNote* note = new iNote(Note_getId(),text,this);	
	listNotes.insert(note->Id(),note);
	return note;
}

bool iDoc::readMapFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }
    xmlReader.setDevice(&file);

    xmlReader.readNext();
    while (!xmlReader.atEnd()) 
	{
		if (xmlReader.isStartElement()) 
		{
			if (xmlReader.name() == "map") 
			{
				//get data file name
				QString version = xmlReader.attributes().value("version").toString();
				QString dataFile = xmlReader.attributes().value("data").toString();
				setDataFile(dataFile);

				readMapElement();																	//read map element from map file
			}
			else 
			{
				xmlReader.raiseError(QObject::tr("Not a map file"));
			}
		} 
		else 
		{
			xmlReader.readNext();
		}
	}

	file.close();
    if (xmlReader.hasError()) {
        std::cerr << "Error: Failed to parse file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(xmlReader.errorString()) << std::endl;
        return false;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }
    return true;


}
void iDoc::readMapElement()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {

            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
			if(xmlReader.name() == "stats")  
			{
				readStations();
			}
			else if(xmlReader.name() == "notes")  
			{
				readNotes();
			}
			//else if(xmlReader.name() == "legends")  
			//{
			//	//readLegends();
			//}
			else 
			{
                skipUnknownElement();
            }
        } else {
            xmlReader.readNext();
        }
    }
}


void iDoc::readStations()
{
	xmlReader.readNext();
	while (!xmlReader.atEnd()) {
		if (xmlReader.isEndElement()) {
			xmlReader.readNext();
			break;
		}

		if (xmlReader.isStartElement()) {
			if (xmlReader.name() == "stat") {
				readStatElement();																	//read one station element
			}
			else 
			{
				skipUnknownElement();
			}
		} else {
			xmlReader.readNext();
		}
	}

}
void iDoc::readStatElement()
{
	int		stat_id = xmlReader.attributes().value("id").toString().toInt();
	QString stat_name = xmlReader.attributes().value("name").toString();
	int		stat_type = xmlReader.attributes().value("type").toString().toInt();
	QString stat_pos = xmlReader.attributes().value("pos").toString();

	QPointF posStat;
	QStringList list = stat_pos.split(",");
	if(list.count()>=2)
		posStat = QPointF(list.at(0).toFloat(),list.at(1).toFloat());

	QPointF posName(10,10);
	QFont   fontName;
	QPointF posValue(10,-20);

	iSTAT* stat= STAT_new(stat_id,stat_name);
	if(stat)
	{
		stat->setsType((STAT_TYPE)stat_type);
	}

    xmlReader.readNext();
    while (!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) 
		{
            if (xmlReader.name() == "name_item") 
			{
				//read station name element
				QString name_pos = xmlReader.attributes().value("pos").toString();
				QStringList list = name_pos.split(",");
				if(list.count()>=2)
					posName = QPointF(list.at(0).toFloat(),list.at(1).toFloat());

				fontName.fromString(xmlReader.attributes().value("font").toString());    
				QString name_text = xmlReader.readElementText();


				if (xmlReader.isEndElement())
					xmlReader.readNext();
            } 
			else if (xmlReader.name() == "value_item") 
			{
				//read station value element
				QString value_pos = xmlReader.attributes().value("pos").toString();
				QStringList list = value_pos.split(",");
				if(list.count()>=2)
					posValue = QPointF(list.at(0).toFloat(),list.at(1).toFloat());

				PF_VTYPE vGen	= (PF_VTYPE)xmlReader.attributes().value("gen_view").toString().toInt();
				PF_VTYPE vLoad	=(PF_VTYPE)xmlReader.attributes().value("load_view").toString().toInt();
				bool bLoad		= QVariant(xmlReader.attributes().value("load_show").toString()).toBool();
				bool bComp		= QVariant(xmlReader.attributes().value("comp_show").toString()).toBool();
				QString value_text = xmlReader.readElementText();
				stat->setPowerType(vGen);
				stat->setloadshown(bLoad);
				stat->setLoadType(vLoad);
				stat->setcompensationshown(bComp);


				if (xmlReader.isEndElement())
					xmlReader.readNext();
            } 
			else if (xmlReader.name() == "nodes") 
			{
				QList<iNodeData *> listNodes;
                readNodes(listNodes);
				stat->setNodes(listNodes);
            }
			else 
			{
                skipUnknownElement();
            }
        } else {
            xmlReader.readNext();
        }
    }
	emit statAdded(stat,posStat,fontName,posName,posValue);											//tell scene to add stat item
}

void iDoc::readNodes(QList<iNodeData *> &listNodes)
{
   xmlReader.readNext();
    while (!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "node") 
			{
				//read node element
				int id = xmlReader.attributes().value("id").toString().toInt();
				QString v_show = xmlReader.attributes().value("v_show").toString();
				QString name = xmlReader.readElementText();
				iNodeData* node = getNode(T_BUS,id);
				if(node)
				{
					node->setShowVoltage(QVariant(v_show).toBool());
					listNodes.append(node);
				}

				if (xmlReader.isEndElement())
					xmlReader.readNext();            
            }
			else 
			{
                skipUnknownElement();
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void iDoc::readNotes()
{
	xmlReader.readNext();
	while (!xmlReader.atEnd()) {
		if (xmlReader.isEndElement()) {
			xmlReader.readNext();
			break;
		}

		if (xmlReader.isStartElement()) {
			if (xmlReader.name() == "note") {
				//read one note element
				int		id = xmlReader.attributes().value("id").toString().toInt();
				QString szPos = xmlReader.attributes().value("pos").toString();
				QPointF pos;
				QStringList list = szPos.split(",");
				if(list.count()>=2)
					pos = QPointF(list.at(0).toFloat(),list.at(1).toFloat());
				QFont font;
				font.fromString(xmlReader.attributes().value("font").toString()); 
				int     color = xmlReader.attributes().value("color").toString().toInt();
				int		align = xmlReader.attributes().value("align").toString().toInt();
				bool	border= QVariant(xmlReader.attributes().value("border").toString()).toBool();
				QString text  = xmlReader.readElementText();

				iNote* note= Note_new(text);
				if(note) 
				{
					note->setTextFont(font);
					note->setTextColor(QColor(color));
					note->setAlignmode((Qt::Alignment)align);
					note->setborder(border);
				}

				emit noteAdded(note,pos);															//tell scene to add note item
				if (xmlReader.isEndElement())
					xmlReader.readNext();
			}
			else 
			{
				skipUnknownElement();
			}
		} else {
			xmlReader.readNext();
		}
	}
}

void iDoc::skipUnknownElement()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement()) {
            skipUnknownElement();
        } else {
            xmlReader.readNext();
        }
    }
}
bool iDoc::writeMapFile(const QString& mapFile)
{
    QFile file(mapFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(mapFile) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("map");
	xmlWriter.writeAttribute("version", "1.0");
	xmlWriter.writeAttribute("data", dataFile());
    writeStats(&xmlWriter);
	writeNotes(&xmlWriter);
	writeLegends(&xmlWriter);
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(mapFile) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
}
void iDoc::writeStats(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("stats");
	foreach (iSTAT* stat, listSTAT)
	{
		//<stat>
		xmlWriter->writeStartElement("stat");
		xmlWriter->writeAttribute("id", QString::number(stat->Id()));
		xmlWriter->writeAttribute("name", stat->name());
		xmlWriter->writeAttribute("type", QString::number(stat->sType()));
		DiagramItem* item = stat->myItem();
		xmlWriter->writeAttribute("pos", QString("%1,%2").arg(item->pos().x()).arg(item->pos().y()));

		//<name_item>
		xmlWriter->writeStartElement("name_item");
		DiagramTextItem* item_N = stat->itemName();
		xmlWriter->writeAttribute("pos", QString("%1,%2").arg(item_N->pos().x()).arg(item_N->pos().y()));
		xmlWriter->writeAttribute("font",item_N->font().toString());
		xmlWriter->writeEndElement();

		//<value_item>
		xmlWriter->writeStartElement("value_item");
		DiagramTextItem* item_V = stat->itemValue();
		xmlWriter->writeAttribute("pos", QString("%1,%2").arg(item_V->pos().x()).arg(item_V->pos().y()));
		xmlWriter->writeAttribute("gen_view",QString::number(stat->powerShowtype()));
		xmlWriter->writeAttribute("load_view",QString::number(stat->loadShowtype()));
		xmlWriter->writeAttribute("load_show",stat->isshowload()?"true":"false");
		xmlWriter->writeAttribute("comp_show",stat->isshowcompensation()?"true":"false");
		xmlWriter->writeEndElement();

		//<nodes>
		xmlWriter->writeStartElement("nodes");
		foreach(iNodeData* node,stat->nodeDatas())
		{
			xmlWriter->writeStartElement("node");
			xmlWriter->writeAttribute("id", QString::number(node->Id()));
			xmlWriter->writeAttribute("v_show",node->isShowVoltge()?"true":"false");
			QString name;
			if(node->type() == T_BUS)
				name = ((iBUS*)node)->name();
			xmlWriter->writeCharacters(name);
			xmlWriter->writeEndElement();
		}
		xmlWriter->writeEndElement();

		xmlWriter->writeEndElement();
	}
    xmlWriter->writeEndElement();

}
void iDoc::writeNotes(QXmlStreamWriter *xmlWriter)
{
	xmlWriter->writeStartElement("notes");
	foreach (iNote* note, listNotes)
	{
		//<note>
		xmlWriter->writeStartElement("note");
		xmlWriter->writeAttribute("id", QString::number(note->Id()));
		DiagramNoteItem* noteItem = note->noteitem();
		xmlWriter->writeAttribute("pos", QString("%1,%2").arg(noteItem->pos().x()).arg(noteItem->pos().y()));

		xmlWriter->writeAttribute("font",note->getTextFont().toString());
		xmlWriter->writeAttribute("color", QString::number(note->getTextColor().value()));
		xmlWriter->writeAttribute("align", QString::number(note->getAlignMode()));
		xmlWriter->writeAttribute("border",noteItem->hasborder()?"true":"false");
		xmlWriter->writeCharacters(note->text());
		xmlWriter->writeEndElement();
	}
	xmlWriter->writeEndElement();
}
void iDoc::writeLegends(QXmlStreamWriter *xmlWriter)
{
	xmlWriter->writeStartElement("legends");

	xmlWriter->writeEndElement();
}