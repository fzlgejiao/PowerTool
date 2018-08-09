#ifndef IDATA_H
#define IDATA_H

#include <QObject>
#include <QMap>
#include <QList>

//data type
typedef enum{
	T_NONE			= 0,
	T_STAT,
	T_BUS,
	T_BRANCH,
	T_TRANSFORMER,
}T_DATA;

#define Uid2Type(uid)	((uid) >> 16)
#define	Uid2Id(uid)		((uid) & 0xFFFF)
#define	UID(type,id)	(((type)<<16)|(id))

#define ITEM_DATA	0																				//item data type

class iData : public QObject
{
	Q_OBJECT

public:
	iData(int id,QObject *parent=0);
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
	iLinkData(int id,int fromUid,int toUid,QObject *parent=0);
	~iLinkData(){}
	virtual T_DATA type()		= 0;
	int fromUid(){return m_fromUid;}
	int toUid(){return m_toUid;}

protected:
	int			m_fromUid;																			//uid of from node
	int			m_toUid;																			//uid of to node
};

class iNodeData : public iData
{
public:
	iNodeData(int id,QObject *parent=0);
	~iNodeData(){}
	virtual T_DATA type()		= 0;
	int		statId(){return m_statId;}
	void	statAdded(int id){m_statId = id;}
	void	statRemoved(){m_statId=0;}

	void	addLink(iLinkData* data){m_linkDatas.append(data);}
	QList<iLinkData *>& linkDatas(){return m_linkDatas;}


protected:
	int			m_statId;																			//station id(0: not assigned to a station, x: station id)

	QList<iLinkData *>	m_linkDatas;																//branchs/transformers connected to this node
};

class DiagramItem;
class iSTAT : public iData
{
public:
	iSTAT(int id,const QString& name,QObject *parent=0);
	~iSTAT();
	T_DATA	type(){return T_STAT;}
	QString name(){return m_name;}
	void	setName(const QString& value){m_name=value;}

	void	setNodes(const QList<iNodeData *>& listNodes);
	//void	addNode(iNodeData * node){node->statAdded(Id()),m_nodeDatas.append(node);}
	//void    removeNode(iNodeData *node){node->statRemoved(),m_nodeDatas.removeOne(node); }
	//void    removeNodes(const QList<iNodeData *>& listNodes);
	QList<iNodeData *>& nodeDatas(){return m_nodeDatas;}

	void	itemAdded(DiagramItem* item){m_item = item;}
	void	itemRemoved(){m_item=NULL;}
	DiagramItem* myItem(){return m_item;}

private:
	QString			m_name;
	DiagramItem*	m_item;																			//pointer to item on scene for station	

	QList<iNodeData *>	m_nodeDatas;
};


class iBUS : public iNodeData
{
public:
	iBUS(int id,const QString& name,QObject *parent=0);
	~iBUS(){}
	T_DATA type(){return T_BUS;}
	
	
	QString name(){return m_NAME;}

private:
	friend class iDoc;

	//properties
	QString			m_NAME;
};

class iBRANCH : public iLinkData
{
public:
	iBRANCH(int id,int fromUid,int toUid,QObject *parent=0);
	~iBRANCH(){}
	T_DATA type(){return T_BRANCH;}
	

private:
	friend class iDoc;

	//properties
};

class iTRANSFORMER : public iLinkData
{
public:
	iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent=0);
	~iTRANSFORMER(){}
	T_DATA type(){return T_TRANSFORMER;}
	

	

private:
	friend class iDoc;

	//properties
};
#endif // IDATA_H
