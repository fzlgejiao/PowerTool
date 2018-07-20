#ifndef IDATA_H
#define IDATA_H

#include <QObject>
#include <QMap>

enum T_DATA{
	T_BUS	=	0,
	T_BRANCH,
	T_TRANSFORMER,
};

class iData : public QObject
{
	Q_OBJECT

public:
	iData(int id,QObject *parent);
	~iData();
	int		id(){return m_ID;}
	virtual T_DATA type()		= 0;
	
private:
	int			m_ID;	
};


class iBRANCH;
class iTRANSFORMER;
class iBUS : public iData
{
public:
	iBUS(int id,const QString& name,QObject *parent);
	~iBUS(){}
	T_DATA type(){return T_BUS;}
	
	QString name(){return m_NAME;}
	void	attach(iData* data){linkDatas.append(data);}

	void	beAdded(){m_bAdded = true;}
	void	beRemoved(){m_bAdded = false;}
	bool	isAdded(){return m_bAdded;}

private:
	friend class iDoc;

	QString		m_NAME;

	QList<iData *>			linkDatas;																//branchs/transformers connected to this bus
	bool		m_bAdded;																			//flag for added to scene or not

};

class iBRANCH : public iData
{
public:
	iBRANCH(int id,QObject *parent);
	~iBRANCH(){}
	T_DATA type(){return T_BRANCH;}
	
	int		fromBUS(){return m_fromBUS;}
	int		toBUS(){return m_toBUS;}
	
	void set2FromBUS(int fromvalue) { m_fromBUS= fromvalue ;}
	void set2ToBUS(int tovalue) { m_toBUS= tovalue ;}

private:
	friend class iDoc;
	int			m_fromBUS;
	int			m_toBUS;

};

class iTRANSFORMER : public iData
{
public:
	iTRANSFORMER(int id,QObject *parent);
	~iTRANSFORMER(){}
	T_DATA type(){return T_TRANSFORMER;}
	
	int		fromBUS(){return m_fromBUS;}
	int		toBUS(){return m_toBUS;}
	
	void set2FromBUS(int fromvalue) { m_fromBUS= fromvalue ;}
	void set2ToBUS(int tovalue) { m_toBUS= tovalue ;}

private:
	friend class iDoc;
	int			m_fromBUS;
	int			m_toBUS;

};
#endif // IDATA_H
