#ifndef IDATA_H
#define IDATA_H

#include <QObject>
#include <QMap>
#include <QList>

//data type
typedef enum{
	T_NONE			= 0,
	T_AREA,
	T_STAT,
	T_SLINK,																						//station link data
	T_BUS,
	T_BRANCH,
	T_TRANSFORMER,

	T_NOTE,
	T_SYSINFO,
	T_LEGEND
}T_DATA;

//station type
typedef enum{
	STAT_HYDROPOWER		 = 0,
	STAT_THERMALPOWER,
	STAT_NUCLEARPOWER,
	STAT_550KV,
	STAT_330KV,		
	STAT_220KV,
	STAT_110KV
}STAT_TYPE;

//menu type
typedef enum{
	MENU_SCENE		= 0,
	MENU_STAT,
	MENU_STAT_NAME,
	MENU_STAT_VALUE,
	MENU_STAT_LINK,
	MENU_NOTE,
	MENU_SYSINFO,
	MENU_LEGEND

}MENU_TYPE;


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

	double GetRefVoltage(){return m_refvoltage;}
	double GetVoltage(){return m_voltage;}
	
	void setShowVoltage(bool value){m_isShowVoltage=value;}
	bool isShowVoltge(){return m_isShowVoltage;}

protected:
	int			m_statId;																			//station id(0: not assigned to a station, x: station id)
	
	QList<iLinkData *>	m_linkDatas;																//branchs/transformers connected to this node

	double		m_refvoltage;
	double      m_voltage;
	bool		m_isShowVoltage;
};

class iAREA : public iData
{
public:
	iAREA(int id,const QString& name,QObject *parent=0)
		:iData(id,parent)
	{
		m_Name = name;
	}
	~iAREA(){}
	T_DATA	type(){return T_AREA;}
	QString name(){return m_Name;}

private:
	friend class iDoc;
	QString			m_Name;
};

class DiagramItem;
class DiagramTextItem;
class iSLINK;
class iSTAT : public iData
{
public:
	iSTAT(int id,const QString& name,QObject *parent=0);
	~iSTAT();
	T_DATA	type(){return T_STAT;}
	QString name(){return m_Name;}
	void	setName(const QString& value){m_Name=value;}
	QString value(){return m_Value;}
	STAT_TYPE	sType(){return m_sType;}
	void	setsType(STAT_TYPE type){m_sType = type;}

	void	setNodes(const QList<iNodeData *>& listNodes);
	//void	addNode(iNodeData * node){node->statAdded(Id()),m_nodeDatas.append(node);}
	//void    removeNode(iNodeData *node){node->statRemoved(),m_nodeDatas.removeOne(node); }
	//void    removeNodes(const QList<iNodeData *>& listNodes);
	QList<iNodeData *>& nodeDatas(){return m_nodeDatas;}

	void	itemAdded(DiagramItem* item){m_itemStat = item;}
	void	itemRemoved(){m_itemStat=NULL;}
	DiagramItem* myItem(){return m_itemStat;}

	void	setItemName(DiagramTextItem* item){m_itemName = item;}
	DiagramTextItem* itemName(){return m_itemName;}
	void	setItemValue(DiagramTextItem* item){m_itemValue = item;}
	DiagramTextItem* itemValue(){return m_itemValue;}

	void	addSlink(iSLINK* slink);
	void	removeSlinks();
	void	removeSlink(iSLINK* slink);

private:
	friend class iDoc;
	QString			m_Name;
	QString			m_Value;
	STAT_TYPE		m_sType;																		//station type

	DiagramItem*	 m_itemStat;																	//pointer to station diagram item
	DiagramTextItem* m_itemName;																	//pointer to station name text item
	DiagramTextItem* m_itemValue;																	//pointer to station value text item

	QList<iNodeData *>	m_nodeDatas;
	QList<iSLINK *>		m_sLinks;
};

class iSLINK : public iData
{
public:
	iSLINK(int id,iSTAT* startSTAT,iSTAT* endSTAT,QObject *parent=0);
	~iSLINK(){}
	T_DATA	type(){return T_SLINK;}

	void			addLinkData(int groupId,iLinkData *linkData);
	QMap<int,QList<iLinkData *> > &linkGroups(){return m_linkGroups;}
	int				groupCount();
	int				groupLineCount(int groupId);													//get line count for one specific arrow group
	QList<iLinkData *> groupLinkDatas(int groupId);

	void			setStartItem(DiagramItem* item){m_startItem = item;}
	void			setEndItem(DiagramItem* item){m_endItem = item;}
	DiagramItem*	startItem(){return m_startItem;}
	DiagramItem*	endItem(){return m_endItem;}

	iSTAT*			startStat(){return m_startSTAT;}
	iSTAT*			endStat(){return m_endSTAT;}

private:
	friend class iDoc;

	iSTAT*			m_startSTAT;																	//start station
	iSTAT*			m_endSTAT;																		//end station
	DiagramItem*	m_startItem;
	DiagramItem*	m_endItem;
	QList<iLinkData *>					m_linkDatas;
	QMap<int,QList<iLinkData *> >		m_linkGroups;												//link groups, one group for one arrow(can have more than one arrow line)
};

class iBUS : public iNodeData
{
public:
	iBUS(int id,int areaid,const QString& name,QObject *parent=0);
	~iBUS(){}
	T_DATA type(){return T_BUS;}
	int    belongedArea(){return m_areaID;}
	
	QString name(){return m_Name;}
	

private:
	friend class iDoc;

	//properties
	QString		m_Name;
	int         m_areaID;
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

class iNote : public iData
{
public:
	iNote(int id,const QString& text,QObject *parent=0);
	~iNote(){}
	T_DATA type(){return T_NOTE;}

private:
	friend class iDoc;
};

class iSysInfo : public iData
{
public:
	iSysInfo(int id,const QString& name,QObject *parent=0);
	~iSysInfo(){}
	T_DATA type(){return T_SYSINFO;}

private:
	friend class iDoc;
};

class iLegend : public iData
{
public:
	iLegend(int id,QObject *parent=0);
	~iLegend(){}
	T_DATA type(){return T_LEGEND;}

private:
	friend class iDoc;
};
#endif // IDATA_H
