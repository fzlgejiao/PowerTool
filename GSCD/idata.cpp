#include "idata.h"
#include "diagramitem.h"

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
	m_name = name;
	m_itemStat = NULL;
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
//--------------------------------------------------------------------------------------------------
//	iBUS funcs
//--------------------------------------------------------------------------------------------------
iBUS::iBUS(int id,int areaid,const QString& name,QObject *parent)
	: iNodeData(id,parent)
{
	m_NAME = name;
	m_areaID=areaid;
}
iBRANCH::iBRANCH(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}
iTRANSFORMER::iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}