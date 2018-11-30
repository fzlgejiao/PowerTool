#ifndef IDATA_H
#define IDATA_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QColor>
#include <QFont>
//data type
typedef enum{
	T_NONE			= 0,
	T_AREA,
	T_STAT,
	T_SLINK,																						//station link data
	T_BUS,
	T_BRANCH,
	T_TRANSFORMER,
	T_GENERATOR,
	T_LOAD,
	T_COMPENSATION,

	T_NOTE,
	T_SYSINFO,
	T_LEGEND
}T_DATA;

//station type
typedef enum{
	STAT_HYDROPLANT		 = 0,						//水电厂
	STAT_THERMALPLANT,								//火电厂
	STAT_NUCLEARPLANT,								//核电厂
	STAT_PUMPEDSTORAGEPLANT,						//抽水蓄能长
	STAT_SUBSYSTEM3,								//子系统
	STAT_SUBSYSTEM2,
	STAT_SUBSYSTEM1,
	STAT_550KV,
	STAT_330KV,		
	STAT_220KV,
	STAT_110KV,
	STAT_T_STATION,									//T接站
	STAT_SERIESCOMPENSATION,						//串补站
	STAT_CONVERTER,									//换流站
	STAT_SWITCHING									//开关站
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

typedef enum{
	SHOW_NONE		=0,	
	SHOW_ONLYNAME,
	SHOW_POWERFLOW,
	SHOW_RESISTANCE,
}SHOW_TYPE;

typedef enum{
	UNIT_NONE		=0,	
	UNIT_PERUNIT,
	UNIT_ACTUALVALUE,
}UNIT_TYPE;
//control panel settings
typedef struct{
	SHOW_TYPE showtype;
	bool isShowStationName;
	bool isShowStationValue;
	bool isShowBranchLine;
	bool isShowBranchValue;
	bool isShowReactivePowerValue;
	bool isShowAdmittance;
	bool isShowVoltageAngle;
	bool isShowAllNodeVoltage;
	UNIT_TYPE unittype;
}ControlPanel;

typedef enum{	
	VALUE_ADDUP			=0,
	VALUE_LIST
}STATION_VALUE_SHOW_TYPE;

//typedef enum 
//{
//	AlignLeft=0,
//	AlignCenter,
//	AlignRight,
//	AlignNONE
//}TextAlignMode;

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
	void	statRemoved()
	{
		m_statId=0;
		m_isShowVoltage=false;
	}
	
	void	addLink(iLinkData* data){m_linkDatas.append(data);}
	QList<iLinkData *>& linkDatas(){return m_linkDatas;}

	double GetRefVoltage(){return m_refvoltage;}
	double GetVoltage(){return m_voltage;}
	double GetAngle(){return m_angle;}

	void setShowVoltage(bool value){m_isShowVoltage=value;}
	bool isShowVoltge(){return m_isShowVoltage;}

	double getPowerPG(){return power_PG;}						//All bus values is in per unit ,default
	double getPowerQG(){return power_QG;}

	double getLoadPL(){return Load_PL;}						
	double getLoadQL(){return Load_QL;}

	double getGL(){return Compensation_GL;}						
	double getBL(){return Compensation_BL;}

	double getSBase(){return sbase;}
	QString getPower();
	QString getLoad();
	QString getCompensation();

protected:
	int			m_statId;																			//station id(0: not assigned to a station, x: station id)
	
	QList<iLinkData *>	m_linkDatas;																//branchs/transformers connected to this node

	double		m_refvoltage;
	double      m_voltage;
	double      m_angle;
	bool		m_isShowVoltage;

	double power_PG;
	double power_QG;

	double Load_PL;
	double Load_QL;

	double Compensation_GL;
	double Compensation_BL;

	double sbase;
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
class DiagramNoteItem;
class iSLINK;

class iSTAT : public iData
{
	Q_OBJECT

public:
	iSTAT(int id,const QString& name,QObject *parent=0);
	~iSTAT();
	T_DATA	type(){return T_STAT;}
	QString name(){return m_Name;}
	void	setName(const QString& name){m_Name=name;}
	QString value(){return m_Value;}
	QString value(double sbase,UNIT_TYPE type,bool showReactivePower);
	void	setValue(const QString& value){m_Value = value;}
	QString nodeVoltage(bool withangle,UNIT_TYPE unit) const;
	QString allNodeVoltage(bool withangle,UNIT_TYPE unit) const;

	STAT_TYPE	sType(){return m_sType;}
	void	setsType(STAT_TYPE type){m_sType = type;}

	void	setNodes(const QList<iNodeData *>& listNodes);
	//void	addNode(iNodeData * node){node->statAdded(Id()),m_nodeDatas.append(node);}
	//void    removeNode(iNodeData *node){node->statRemoved(),m_nodeDatas.removeOne(node); }
	//void    removeNodes(const QList<iNodeData *>& listNodes);
	QList<iNodeData *>& nodeDatas(){return m_nodeDatas;}

	void	setItem(DiagramItem* item){m_itemStat = item;}
	DiagramItem* myItem(){return m_itemStat;}

	void	setItemName(DiagramTextItem* item){m_itemName = item;}
	DiagramTextItem* itemName(){return m_itemName;}
	void	setItemValue(DiagramTextItem* item){m_itemValue = item;}
	DiagramTextItem* itemValue(){return m_itemValue;}

	void	addSlink(iSLINK* slink);
	void	removeSlinks();
	void	removeSlink(iSLINK* slink);	

	STATION_VALUE_SHOW_TYPE powerShowtype(){return m_power_showtype;}
	void setPowerType(STATION_VALUE_SHOW_TYPE type){m_power_showtype=type;}

	STATION_VALUE_SHOW_TYPE loadShowtype(){return m_load_showtype;}
	void setLoadType(STATION_VALUE_SHOW_TYPE type){m_load_showtype=type;}

	bool isshowload(){return is_loadshown;}
	void setloadshown(bool value){is_loadshown=value;}

	bool isshowcompensation(){return is_compensationshown;}
	void setcompensationshown(bool value){is_compensationshown=value;}
public slots:
		void OncontrolpanelChanged(ControlPanel &settings,uint changes);
		void OnapplyNameFont2all(QFont &font);

private:
	friend class iDoc;
	QString			m_Name;
	QString			m_Value;																		//nodes voltage value(can be more than one node,e.g "v1:v2")
	STAT_TYPE		m_sType;																		//station type

	STATION_VALUE_SHOW_TYPE m_power_showtype;
	STATION_VALUE_SHOW_TYPE m_load_showtype;

	DiagramItem*	 m_itemStat;																	//pointer to station diagram item
	DiagramTextItem* m_itemName;																	//pointer to station name text item
	DiagramTextItem* m_itemValue;																	//pointer to station value text item

	QList<iNodeData *>	m_nodeDatas;
	QList<iSLINK *>		m_sLinks;

	
	bool is_loadshown;
	bool is_compensationshown;
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
	
	QString name(bool withId = false)
	{
		if(withId)
			return QString("%1-%2").arg(Id()).arg(m_Name);
		else
			return m_Name;
	}
	


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
	iBUS * getFromBus(){return frombus;}
	iBUS * getToBus(){return tobus;}
	int    getParallelCode(){return ParallelCode;}
	QString getRX(){return QString("%1+j%2").arg(branch_R,0,'f',5).arg(branch_X,0,'f',5);}

private:
	friend class iDoc;
	iBUS *frombus;
	iBUS *tobus;
	int ParallelCode;
	double branch_R;
	double branch_X;
	//properties
};

class iTRANSFORMER : public iLinkData
{
public:
	iTRANSFORMER(int id,int fromUid,int toUid,QObject *parent=0);
	~iTRANSFORMER(){}
	T_DATA type(){return T_TRANSFORMER;}

	iBUS * getFromBus() {return frombus;}
	iBUS * getToBus() {return tobus;}

private:
	friend class iDoc;
	iBUS *frombus;
	iBUS *tobus;
	//properties
};
class iNote : public iData
{
	Q_OBJECT
public:
	iNote(int id,const QString& text,QObject *parent=0);
	~iNote(){}
	T_DATA type(){return T_NOTE;}

	QString  text(){return m_text;}
	void setText(QString& text){m_text=text;}

	Qt::Alignment getAlignMode(){return m_alignmode;}
	void setAlignmode(Qt::Alignment alignmode){ m_alignmode=alignmode;}

	QColor getTextColor(){return m_textcolor;}
	void setTextColor(QColor color){m_textcolor=color;}

	QFont getTextFont(){return m_font;}
	void setTextFont(QFont font){m_font=font;}

	bool hasborder(){return m_hasborder;}
	void setborder(bool border){m_hasborder=border;}

	DiagramNoteItem * noteitem() {return m_noteitem;}
	void setnoteitem(DiagramNoteItem *item){m_noteitem=item;}
private:
	friend class iDoc;
	QString m_text;
	Qt::Alignment m_alignmode;
	QColor m_textcolor;
	QFont m_font;
	bool m_hasborder;
	DiagramNoteItem * m_noteitem;
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
