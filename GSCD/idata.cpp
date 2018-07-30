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
	m_item = NULL;
}
iSTAT::~iSTAT()
{
	foreach(iNodeData* node,m_nodeDatas)
	{
		node->statRemoved();
	}
}
void iSTAT::addNodes(const QList<iNodeData *>& listNodes)
{
	foreach(iNodeData* node,listNodes)
		node->statAdded(Id());
	m_nodeDatas.append(listNodes);
}
//--------------------------------------------------------------------------------------------------
//	iBUS funcs
//--------------------------------------------------------------------------------------------------
iBUS::iBUS(int id,const QString& name,QObject *parent)
	: iNodeData(id,parent)
{
	m_NAME = name;
}
iBRANCH::iBRANCH(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}
iTRANSFORMER::iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent)
	: iLinkData(id,fromUid,toUid,parent)
{

}