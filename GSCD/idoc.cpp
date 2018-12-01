#include "idoc.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QXmlStreamWriter>
#include <iostream>


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
}

iDoc::~iDoc()
{
	qDeleteAll(listSLINK);																			//already deleted in iSTAT::removeSlinks
	qDeleteAll(listSTAT);
	qDeleteAll(listBUS);
	qDeleteAll(listBRANCH);
	qDeleteAll(listTRANSFORMER);
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

	file.close();
	if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }
    return true;
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
void iDoc::GetDataModel(QFile& file,T_DATA datatype)
{
	int datarows=0;		
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
	else return;
	QTextStream stream(&file);
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
							QString CKT= datalist[2].replace(QString("'"),QString("")).trimmed();
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							r=datalist[3].toDouble();
							x=datalist[4].toDouble();
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
								addBRANCH(branch);														//add branch into branch list
							}
						}
					break;

					case T_TRANSFORMER:																//Transformer Data Area 
						{
							int from,to,k;
							from = datalist[0].toInt();
							to   = datalist[1].toInt();
							k=datalist[2].toInt();

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
								addTRANSFORMER(transformer);											//add transformer into transformer list
							}

							if(k==0)
							{
								TranformerDummylines=3;
							}else TranformerDummylines=4;

							for(int i=0;i<TranformerDummylines;i++)
								stream.readLine();
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
    while (!xmlReader.atEnd()) {
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "map") {
				readMapElement();
            } else {
                xmlReader.raiseError(QObject::tr("Not a map file"));
            }
        } else {
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
            if (xmlReader.name() == "datafile") 
			{
				//read data file element
				m_szDataFile = xmlReader.readElementText();
				readDataFile(m_szDataFile);
				if (xmlReader.isEndElement())
					xmlReader.readNext();
            }
			else if(xmlReader.name() == "stats")  
			{
				readStations();
			}
			//else if(xmlReader.name() == "notes")  
			//{
			//	//readNotes();
			//}
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
	emit statAdded(stat,posStat,fontName,posName,posValue);
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
