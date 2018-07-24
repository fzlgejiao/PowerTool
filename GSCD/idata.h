#ifndef IDATA_H
#define IDATA_H

#include <QObject>
#include <QMap>
#include <QList>

typedef enum{
	T_NONE			= 0,
	T_BUS			= 1,
	T_BRANCH,
	T_TRANSFORMER,
}T_DATA;

#define Uid2Type(uid)	((uid) >> 16)
#define	Uid2Id(uid)		((uid) & 0xFFFF)

class iData : public QObject
{
	Q_OBJECT

public:
	iData(int id,QObject *parent);
	~iData();
	virtual T_DATA type()		= 0;
	int		Id(){return m_ID;}
	int		Uid(){return (type()<<16) | Id();}
	
private:
	int			m_ID;	
};
class iLinkData : public iData
{
public:
	iLinkData(int id,int from,int to,QObject *parent);
	~iLinkData(){}
	virtual T_DATA type()		= 0;
	int fromId(){return m_fromId;}
	int toId(){return m_toId;}

protected:
	int			m_fromId;
	int			m_toId;
};

class DiagramItem;
class iBUS : public iData
{
public:
	iBUS(int id,const QString& name,QObject *parent);
	~iBUS(){}
	T_DATA type(){return T_BUS;}
	
	
	QString name(){return m_NAME;}
	void	addLink(iLinkData* data){m_linkDatas.append(data);}
	QList<iLinkData *>& linkDatas(){return m_linkDatas;}

	void	itemAdded(DiagramItem* item){m_bAdded = true;m_item = item;}
	void	itemRemoved(){m_bAdded = false;m_item=NULL;}
	bool	isAdded(){return m_bAdded;}
	DiagramItem* myItem(){return m_item;}

private:
	friend class iDoc;

	QString			m_NAME;

	QList<iLinkData *>	m_linkDatas;																//branchs/transformers connected to this bus
	bool			m_bAdded;																		//flag for added to scene or not
	DiagramItem*	m_item;																			//pointer to item on scene for bus	

};

class iBRANCH : public iLinkData
{
public:
	iBRANCH(int id,int from,int to,QObject *parent);
	~iBRANCH(){}
	T_DATA type(){return T_BRANCH;}
	

private:
	friend class iDoc;


};

class iTRANSFORMER : public iLinkData
{
public:
	iTRANSFORMER(int id,int from,int to,QObject *parent);
	~iTRANSFORMER(){}
	T_DATA type(){return T_TRANSFORMER;}
	

	

private:
	friend class iDoc;


};
#endif // IDATA_H
