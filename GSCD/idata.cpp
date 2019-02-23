#include "idata.h"
#include "diagramitem.h"
#include "idoc.h"
#include "controlpaneldialog.h"
#include "arrow.h"

iData::iData(int id,QObject *parent)
	: QObject(parent)
{
	m_ID = id;
}

iData::~iData()
{

}
iLinkData::iLinkData(int id,int fromUid,int toUid,QObject *parent)
	: iData(id,parent),m_fromUid(fromUid),m_toUid(toUid)
{
	m_P1=0;
	m_P2=0;
	m_Q1=0;
	m_Q2=0;
}
iNodeData::iNodeData(int id,QObject *parent)
	: iData(id,parent),m_statId(0)
{

}
QString iNodeData::getPower()
{
	if((power_QG!=0)||(power_PG!=0)) 
		return  QString("%1+i%2").arg(power_PG,0,'f',3,QLatin1Char('0')).arg(power_QG,0,'f',3,QLatin1Char('0'));
	else return NULL;
}
QString iNodeData::getLoad()
{
	if((Load_PL!=0)||(Load_QL!=0)) 
		return  QString("%1+i%2").arg(Load_PL,0,'f',3,QLatin1Char('0')).arg(Load_QL,0,'f',3,QLatin1Char('0'));
	else return NULL;
}
QString iNodeData::getCompensation()
{
	if((Compensation_GL!=0)||(Compensation_BL!=0)) 
		return  QString("%1+i%2").arg(Compensation_GL,0,'f',3,QLatin1Char('0')).arg(Compensation_BL,0,'f',3,QLatin1Char('0'));
	else return NULL;
}
//--------------------------------------------------------------------------------------------------
//	iSTAT funcs
//--------------------------------------------------------------------------------------------------
iSTAT::iSTAT(int id,const QString& name,QObject *parent)
	:iData(id,parent)
{
	m_Name = name;
	m_Value= "value";
	m_sType = STAT_220KV;
	m_itemStat = NULL;
	m_itemName = NULL;
	m_itemValue= NULL;

	m_power_showtype=VALUE_ADDUP;
	m_load_showtype=VALUE_ADDUP;
	is_loadshown=false;
	is_compensationshown=false;	
}
iSTAT::~iSTAT()
{
	foreach(iNodeData* node,m_nodeDatas)
	{
		node->statRemoved();
	}
}
void iSTAT::setNodes(const QList<iNodeData *>& listNodes)
{
	//update old nodes
	foreach(iNodeData* node,m_nodeDatas)
		node->statRemoved();
	//set new nodes to station
	foreach(iNodeData* node,listNodes)
		node->statAdded(Id());
	m_nodeDatas = listNodes;
}

QString iSTAT::value(double sbase,UNIT_TYPE type,bool showReactivePower)
{
	QString valueStr="";
	QString loadlistStr="";
	double factor=1.0;
	double totalPG=0;
	double totalQG=0;

	double totalPL=0;
	double totalQL=0;

	double totalGL=0;
	double totalBL=0;
	if(type==UNIT_ACTUALVALUE)
		factor=sbase;

	foreach(iNodeData* node,m_nodeDatas)
	{		
		double nodePG=node->getPowerPG()*factor;
		double nodeQG=node->getPowerQG()*factor;

		double nodePL=node->getLoadPL()*factor;
		double nodeQL=node->getLoadQL()*factor;

		double nodeGL=node->getGL()*factor;
		double nodeBL=node->getBL()*factor;

		if((nodePG!=0)||(nodeQG!=0)){
			if(m_power_showtype==VALUE_ADDUP)	
			{
				totalPG+=nodePG;
				totalQG+=nodeQG;
			}else
			{
				if(showReactivePower)
					valueStr+=QString("G:%1+j%2 \n").arg(nodePG,0,'f',3,QLatin1Char('0')).arg(nodeQG,0,'f',3,QLatin1Char('0'));
				else 
					valueStr+=QString("G:%1 \n").arg(nodePG,0,'f',3,QLatin1Char('0'));
			}
		}

		if((nodePL!=0)||(nodeQL!=0)){
			if(m_load_showtype==VALUE_ADDUP)
			{
				totalPL+=nodePL;
				totalQL+=nodeQL;
			}else
			{
				if(showReactivePower)
					loadlistStr+=QString("L:%1+j%2 \n").arg(nodePL,0,'f',3,QLatin1Char('0')).arg(nodeQL,0,'f',3,QLatin1Char('0'));
				else
					loadlistStr+=QString("L:%1 \n").arg(nodePL,0,'f',3,QLatin1Char('0'));
			}
		}

		if((nodeGL!=0)||(nodeBL!=0)){
			//Compensation is default is in add up mode
			totalGL+=nodeGL;
			totalBL+=nodeBL;
		}
	}
	if((valueStr=="")&&((totalPG+totalQG)!=0))
	{
		if(showReactivePower)
			valueStr+=QString("G:%1+j%2 \n").arg(totalPG,0,'f',3,QLatin1Char('0')).arg(totalQG,0,'f',3,QLatin1Char('0'));
		else 
			valueStr+=QString("G:%1 \n").arg(totalPG,0,'f',3,QLatin1Char('0'));
	}
	if(is_loadshown)
	{
		if((loadlistStr=="")&&((totalPL+totalQL)!=0))
		{
			if(showReactivePower)
				valueStr+=QString("L:%1+j%2 \n").arg(totalPL,0,'f',3,QLatin1Char('0')).arg(totalQL,0,'f',3,QLatin1Char('0'));
			else
				valueStr+=QString("L:%1 \n").arg(totalPL,0,'f',3,QLatin1Char('0'));
		}
		else 
			valueStr+=loadlistStr;
	}
	if((is_compensationshown)&&((totalGL+totalBL)!=0))
	{		
		if(showReactivePower)
			valueStr+=QString("S:%1+j%2 \n").arg(totalGL,0,'f',3,QLatin1Char('0')).arg(totalBL,0,'f',3,QLatin1Char('0'));
		else 
			valueStr+=QString("S:%1 \n").arg(totalGL,0,'f',3,QLatin1Char('0'));
	}
	return valueStr.trimmed();	
}
//void iSTAT::removeNodes(const QList<iNodeData *>& listNodes)
//{
//	foreach(iNodeData* node,listNodes)
//	{
//		node->statRemoved();
//		m_nodeDatas.removeOne(node);
//	}
//}
void iSTAT::addSlink(iSLINK* slink)
{
	m_sLinks.append(slink);
}
void iSTAT::removeSlinks()
{
   foreach (iSLINK *slink, m_sLinks) {
        slink->startStat()->removeSlink(slink);
        slink->endStat()->removeSlink(slink);
		iDoc* doc = (iDoc *)parent();
		if(doc)
			doc->SLINK_delete(slink->Id());
    }
}
void iSTAT::removeSlink(iSLINK* slink)
{
    int index = m_sLinks.indexOf(slink);

    if (index != -1)
        m_sLinks.removeAt(index);
}
QString iSTAT::nodeVoltage(bool withangle,UNIT_TYPE unit) const
{
	QString value;
	foreach(iNodeData* node,m_nodeDatas)
	{
		double voltage=0;
		switch(unit)
		{
		case UNIT_ACTUALVALUE:
		  voltage	= node->GetVoltage() * node->GetRefVoltage();
		  break;

		case UNIT_PERUNIT:
			voltage	= node->GetVoltage();
			break;
		}

		double angle	= node->GetAngle();

		if(node->isShowVoltge())
		{
			if(withangle)
				value += QString::number(voltage,10,2)+" < "+QString::number(angle,10,2) + "\n";
			else
				value += QString::number(voltage,10,2)+ "\n";
		}
	}
	return value;
}
QString iSTAT::allNodeVoltage(bool withangle,UNIT_TYPE unit) const
{
	QString value;
	foreach(iNodeData* node,m_nodeDatas)
	{
		double voltage=0;
		switch(unit)
		{
		case UNIT_ACTUALVALUE:
			voltage	= node->GetVoltage() * node->GetRefVoltage();
			break;

		case UNIT_PERUNIT:
			voltage	= node->GetVoltage();
			break;
		}
		double angle	= node->GetAngle();
		if(withangle)
			value += QString::number(voltage,10,1)+" < "+QString::number(angle,10,1) + "\n";
		else
			value += QString::number(voltage,10,1)+ "\n";
	}
	return value;
}

void iSTAT::OnapplyNameFont2all(QFont &font)
{
	this->itemName()->setFont(font);
}
void iSTAT::OncontrolpanelChanged(ControlPanel &settings,uint changes)
{
	if(changes & CHG_CONTROLPANEL_SHOWTYPE)
	{
		switch(settings.showtype)
		{
		case SHOW_ONLYNAME:
		case SHOW_RESISTANCE:
			{
				if(settings.isShowStationName)
					m_itemName->setPlainText(this->name());	
				else 
					m_itemName->setVisible(false);
				m_itemValue->setVisible(false);
			}
			break;

		case SHOW_POWERFLOW:
			{
				m_itemName->setVisible(settings.isShowStationName);
				m_itemValue->setVisible(settings.isShowStationValue);
				if(settings.isShowAllNodeVoltage)
				{
					m_itemName->setPlainText(this->allNodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
				}else
				{
					m_itemName->setPlainText(this->nodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
				}
				double base=this->nodeDatas().first()->getSBase();		
				m_itemValue->setPlainText(this->value(base,settings.unittype,settings.isShowReactivePowerValue));
			}
			break;			
		}
	}
	if(changes & CHG_CONTROLPANEL_SHOWSTATNAME)
	{
		m_itemName->setVisible(settings.isShowStationName);
	}
	if(changes & CHG_CONTROLPANEL_SHOWSTATVALUE)
	{
		if(settings.showtype==SHOW_POWERFLOW)
			m_itemValue->setVisible(settings.isShowStationValue);
	}
	/*if(changes & CHG_CONTROLPANEL_SHOWBRNACHLINE)
	{
		foreach(Arrow *arrow,this->myItem()->getArows())
		{
			arrow->setVisible(settings.isShowBranchLine);
		}
	}*/
	/*if(changes & CHG_CONTROLPANEL_SHOWBRNACHVALUE)
	{	
		foreach(Arrow *arrow,this->myItem()->getArows())
		{
			 QList<QGraphicsItem *> valueitems=arrow->childItems();
			 if(valueitems.length()==1)
			 {
				 DiagramTextItem* txtItem = qgraphicsitem_cast<DiagramTextItem *>(valueitems.first());
				 if(txtItem)
					 txtItem->setVisible(settings.isShowBranchValue);
			 }
		}
	}*/
	if(changes & CHG_CONTROLPANEL_SHOWREACTIVEVALUE)
	{			
		double base=this->nodeDatas().first()->getSBase();
		if(settings.showtype==SHOW_POWERFLOW)
			m_itemValue->setPlainText(this->value(base,settings.unittype,settings.isShowReactivePowerValue));
	}
	if(changes & CHG_CONTROLPANEL_SHOWADMITTANCE)
	{	
	}
	if( (changes & CHG_CONTROLPANEL_SHOWANGLE)		||
		(changes & CHG_CONTROLPANEL_SHOWALLVOLTAGE) ||
		(changes & CHG_CONTROLPANEL_UNIT))
	{	
		if(settings.showtype!=SHOW_POWERFLOW)	return ;
		if(changes & CHG_CONTROLPANEL_UNIT)	//Change the station value to current unit type			
		{
			double base=this->nodeDatas().first()->getSBase();			
			m_itemValue->setPlainText(this->value(base,settings.unittype,settings.isShowReactivePowerValue));			
		}
		if(settings.isShowAllNodeVoltage)
		{
			m_itemName->setPlainText(this->allNodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
		}else
		{
			m_itemName->setPlainText(this->nodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
		}
	}
}
//--------------------------------------------------------------------------------------------------
//	iSLINK funcs
//--------------------------------------------------------------------------------------------------
iSLINK::iSLINK(int id,iSTAT* startSTAT,iSTAT* endSTAT,QObject *parent)
	:iData(id,parent)
{
	m_startSTAT	= startSTAT;
	m_endSTAT	= endSTAT;
	m_startItem = NULL;
	m_endItem	= NULL;
	m_arrow=NULL;
}
void iSLINK::addLinkData(int groupId,iLinkData *linkData)
{
	m_linkDatas.append(linkData);
	m_linkGroups[groupId].append(linkData);

	QList<iLinkData *> linkDatas = m_linkGroups[groupId];
}
QList<iLinkData *>	iSLINK::groupLinkDatas(int groupId)
{
	return m_linkGroups[groupId];
}
int	iSLINK::groupCount()
{
	return m_linkGroups.keys().count();
}
int iSLINK::groupLineCount(int groupId)
{
	if(m_linkGroups.contains(groupId) == false)
		return 0;

	return m_linkGroups[groupId].count();
}
void iSLINK::OncontrolpanelChanged(ControlPanel &settings,uint changes)
{	
	if(!m_arrow) return ;
	double base=m_startSTAT->nodeDatas().first()->getSBase();
	if((changes & CHG_CONTROLPANEL_SHOWREACTIVEVALUE) || 
		(changes & CHG_CONTROLPANEL_UNIT)
	  )
	{	
		if(settings.showtype==SHOW_POWERFLOW)
			m_arrow->textItem()->setPlainText(this->linkvalue(base,settings.unittype,settings.isShowReactivePowerValue));		
	}
	if(changes & CHG_CONTROLPANEL_SHOWBRNACHLINE)
	{		
		m_arrow->setVisible(settings.isShowBranchLine);		
	}
	if(changes & CHG_CONTROLPANEL_SHOWBRNACHVALUE)
	{	
		if(settings.showtype!=SHOW_ONLYNAME)
			m_arrow->textItem()->setVisible(settings.isShowBranchValue);
	}
	if(changes & CHG_CONTROLPANEL_SHOWTYPE)
	{
		switch(settings.showtype)
		{
		case SHOW_ONLYNAME:
			{
				m_arrow->setshowArrow(false);
				m_arrow->textItem()->setVisible(false);
			}
			break;

		case SHOW_POWERFLOW:
			{
				m_arrow->setshowArrow(true);
				m_arrow->textItem()->setPlainText(this->linkvalue(base,settings.unittype,settings.isShowReactivePowerValue));
				m_arrow->textItem()->setVisible(settings.isShowBranchValue);
				m_arrow->setVisible(settings.isShowBranchLine);
			}
			break;

		case SHOW_RESISTANCE:
			{
				m_arrow->setshowArrow(false);
				m_arrow->textItem()->setPlainText(this->linkresistance());
				m_arrow->textItem()->setVisible(settings.isShowBranchValue);
			}
			break;
		}
	}
}
QString iSLINK::linkvalue(double sbase,UNIT_TYPE unit,bool showReactivePower)
{
	float link_P=0;
	float link_Q=0;
	QString linktext="";
	foreach(iLinkData *link,m_linkDatas)
	{
		if(link->P1_active()>=0)
		{
			link_P+=link->P1_active();
			link_Q+=link->Q1_reactive();
		}
		else 
		{
			link_P+=link->P2_active();
			link_Q+=link->Q2_reactive();
		}
	}
	if((link_P==0) && (link_Q==0)) return linktext; 

	if(unit==UNIT_ACTUALVALUE)
	{
		link_P*=sbase;
		link_Q*=sbase;
	}
	if(!showReactivePower)
	{
		linktext=QString("%1").arg(link_P,0,'f',1);
	}
	else
	{
		if(link_Q>0)			
			linktext=QString("%1+j%2").arg(link_P,0,'f',(unit==UNIT_ACTUALVALUE)?1:2).arg(link_Q,0,'f',(unit==UNIT_ACTUALVALUE)?1:2);
		else
			linktext=QString("%1-j%2").arg(link_P,0,'f',(unit==UNIT_ACTUALVALUE)?1:2).arg(qAbs(link_Q),0,'f',(unit==UNIT_ACTUALVALUE)?1:2);			
	}

	int count=m_linkDatas.count();
	if(count>1) 
		linktext.append(QString("(%1)").arg(count));

	return linktext;
}
QString iSLINK::linkresistance()
{	
	QString linktext="";
	if(m_linkDatas.count()==0) return linktext;
	iLinkData * link=m_linkDatas.last();						//show the last link data
	if(link->type()==T_BRANCH)	
		linktext=((iBRANCH *)link)->getRX();
	else if(link->type()==T_TRANSFORMER)
		linktext=((iTRANSFORMER *)link)->getRX();
	
	return linktext;
}
//--------------------------------------------------------------------------------------------------
//	iBUS funcs
//--------------------------------------------------------------------------------------------------
iBUS::iBUS(int id,int areaid,const QString& name,QObject *parent)
	: iNodeData(id,parent)
{
	m_Name = name;
	m_areaID=areaid;
	m_refvoltage=0;
	m_voltage=0;
	m_angle=0;
	m_isShowVoltage=false;

	power_PG=0;
	power_QG=0;

	Load_PL=0;
	Load_QL=0;

	Compensation_GL=0;
	Compensation_BL=0;
}
//QString iBUS::getPower()
//{
//	if((power_QG!=0)||(power_PG!=0)) 
//		return  QString("%1+i%2").arg(power_PG,0,'f',3,QLatin1Char('0')).arg(power_QG,0,'f',3,QLatin1Char('0'));
//	else return NULL;
//}
//QString iBUS::getLoad()
//{
//	if((Load_PL!=0)||(Load_QL!=0)) 
//		return  QString("%1+i%2").arg(Load_PL,0,'f',3,QLatin1Char('0')).arg(Load_QL,0,'f',3,QLatin1Char('0'));
//	else return NULL;
//}
//QString iBUS::getCompensation()
//{
//	if((Compensation_GL!=0)||(Compensation_BL!=0)) 
//		return  QString("%1+i%2").arg(Compensation_GL,0,'f',3,QLatin1Char('0')).arg(Compensation_BL,0,'f',3,QLatin1Char('0'));
//	else return NULL;
//}
iBRANCH::iBRANCH(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
	,m_onservice(false)
{
	
}
iTRANSFORMER::iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
	,m_onservice(false)
{
	
}
iFACTSDEVICE::iFACTSDEVICE(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}
iNote::iNote(int id,const QString& text,QObject *parent)
	  :iData(id,parent)
{
	m_text=text;
	m_noteitem=NULL;
}