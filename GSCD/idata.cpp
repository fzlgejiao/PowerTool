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

}
iNodeData::iNodeData(int id,QObject *parent)
	: iData(id,parent),m_statId(0)
{

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
				value += QString::number(voltage,10,1)+" < "+QString::number(angle,10,1) + "\n";
			else
				value += QString::number(voltage,10,1)+ "\n";
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
void iSTAT::OncontrolpanelChanged(ControlPanel &settings,uint changes)
{
	if(changes & CHG_CONTROLPANEL_SHOWTYPE)
	{
		switch(settings.showtype)
		{
		case SHOW_ONLYNAME:
			break;

		case SHOW_POWERFLOW:
			break;

		case SHOW_RESISTANCE:
			break;
		}
	}
	if(changes & CHG_CONTROLPANEL_SHOWSTATNAME)
	{
		this->itemName()->setVisible(settings.isShowStationName);
	}
	if(changes & CHG_CONTROLPANEL_SHOWSTATVALUE)
	{
		this->itemValue()->setVisible(settings.isShowStationValue);
	}
	if(changes & CHG_CONTROLPANEL_SHOWBRNACHLINE)
	{
		foreach(Arrow *arrow,this->myItem()->getArows())
		{
			arrow->setVisible(settings.isShowBranchLine);
		}
	}
	if(changes & CHG_CONTROLPANEL_SHOWBRNACHVALUE)
	{	
	}
	if(changes & CHG_CONTROLPANEL_SHOWREACTIVEVALUE)
	{	
	}
	if(changes & CHG_CONTROLPANEL_SHOWADMITTANCE)
	{	
	}
	if( (changes & CHG_CONTROLPANEL_SHOWANGLE)		||
		(changes & CHG_CONTROLPANEL_SHOWALLVOLTAGE) ||
		(changes & CHG_CONTROLPANEL_UNIT))
	{	
		if(settings.isShowAllNodeVoltage)
		{
			this->itemName()->setPlainText(this->allNodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
		}else
		{
			this->itemName()->setPlainText(this->nodeVoltage(settings.isShowVoltageAngle,settings.unittype)+this->name());
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
}
iBRANCH::iBRANCH(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}
iTRANSFORMER::iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}