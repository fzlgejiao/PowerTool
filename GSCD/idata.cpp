#include "idata.h"

iData::iData(int id,QObject *parent)
	: QObject(parent)
{
	m_ID = id;
}

iData::~iData()
{

}
iBUS::iBUS(int id,const QString& name,QObject *parent)
	: iData(id,parent)
{
	m_NAME = name;
}
iBRANCH::iBRANCH(int id,QObject *parent)
	: iData(id,parent)
{

}
iTRANSFORMER::iTRANSFORMER(int id,QObject *parent)
	: iData(id,parent)
{

}