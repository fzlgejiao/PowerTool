#include "idata.h"
#include "diagramitem.h"
#include "idoc.h"

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