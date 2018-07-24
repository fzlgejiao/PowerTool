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
iLinkData::iLinkData(int id,int from,int to,QObject *parent)
	: iData(id,parent),m_fromId(from),m_toId(to)
{

}
iBUS::iBUS(int id,const QString& name,QObject *parent)
	: iData(id,parent)
{
	m_NAME = name;
	m_bAdded = false;
}
iBRANCH::iBRANCH(int id,int from,int to,QObject *parent)
	: iLinkData(id,from,to,parent)
{

}
iTRANSFORMER::iTRANSFORMER(int id,int from,int to,QObject *parent)
	: iLinkData(id,from,to,parent)
{

}