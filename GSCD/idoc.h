#ifndef IDOC_H
#define IDOC_H

#include <QObject>
#include <QMap>
#include <QFile>
#include "idata.h"
#include <QSize>
#include <QXmlStreamReader>

class iDoc : public QObject
{
	Q_OBJECT

public:
	iDoc(QObject *parent);
	~iDoc();
	void	close();
	bool	isModified(){return m_bModified;}
	void	setModified(bool m){m_bModified = m;}

	bool	readDataFile(const QString& fileName);
	bool	readMapFile(const QString& fileName);
	QString& dataFile(){return m_szDataFile;}
	QString& mapFile(){return m_szMapFile;}
	void	setDataFile(const QString& dataFile){m_szDataFile = dataFile;}
	bool	writeMapFile(const QString& mapFile);
	void	writeStats(QXmlStreamWriter *xmlWriter);
	void	writeNotes(QXmlStreamWriter *xmlWriter);
	void	writeLegends(QXmlStreamWriter *xmlWriter);

	int		STAT_getId();																			//get available station id for a new station
	iSTAT*	STAT_get(int id){return listSTAT.value(id,NULL);}
	void	STAT_delete(int id);
	iSTAT*	STAT_new(int id,const QString& name);

	int		SLINK_getId();																			//get available station link id for a new station link
	iSLINK*	SLINK_get(int id){return listSLINK.value(id,NULL);}
	void	SLINK_delete(int id);
	iSLINK*	SLINK_new(iSTAT* startSTAT,iSTAT* endSTAT);
	iSLINK* SLINK_get(iSTAT* startSTAT,iSTAT* endSTAT);												//get station link by start and end station
	
	int		Note_getId();
	iNote*	Note_get(int id){return listNotes.value(id,NULL);}
	void	Note_delete(int id);
	iNote*	Note_new(const QString& name);

	iNodeData* getNode(int uid);																	//get node by uid
	iNodeData* getNode(int type,int id);

	iBUS*	getBUS(int id){return listBUS.value(id,NULL);}
	iBRANCH*	getBRANCH(int id){return listBRANCH.value(id,NULL);}
	iTRANSFORMER*	getTRANSFORMER(int id){return listTRANSFORMER.value(id,NULL);}

	void	getAvailableNode(QList<iNodeData *>& list);												//to get buses which need to be show in scene
	QMap<int,iAREA *>&   getArealist(){return listAREA;}
	QMap<int,iSTAT *>&   getStatlist(){return listSTAT;}

	ControlPanel getControlPanel(){return m_controlpanel;}
	void	setControlPanel(ControlPanel &value,uint changes);

	iAREA*	getAREA(const QString& name);
	iAREA*	getAREA(int id){return listAREA.value(id,NULL);}

	QSize	getAreaSize(){return m_AreaSize; }
	void	setAreaSize(const QSize &size);

	iData*	Uid2Data(int uid);
	double	sBase(){return SBase;}

	//test func
	void	test();
	iSTAT*	getFreeSTAT()
	{
		foreach(iSTAT* stat,listSTAT)
		{
			if(!stat->myItem())
				return stat;
		}
		return NULL;
	}
protected:
	

	void	addBUS(iBUS* bus){listBUS.insert(bus->Id(),bus);}
	void	addBRANCH(iBRANCH* branch){listBRANCH.insert(branch->Id(),branch);}
	void	addTRANSFORMER(iTRANSFORMER* transformer){listTRANSFORMER.insert(transformer->Id(),transformer);}
	void	addAREA(iAREA* area){listAREA.insert(area->Id(),area);}

private:
	QMap<int,iSTAT *>			listSTAT;															//<id,iSTAT*>: id is the station id
	QMap<int,iSLINK *>			listSLINK;															//<id,iSLINK*>: id is the station link id
	QMap<int,iBUS *>			listBUS;															//<id,iBUS*>: id is the bus id
	QMap<int,iBRANCH *>			listBRANCH;															//<id,iBRANCH*>: id is the branch squence id
	QMap<int,iTRANSFORMER *>	listTRANSFORMER;													//<id,iTRANSFORMER*>: id is the transformer squence id
	QMap<int,iAREA *>			listAREA;
	QMap<int,iNote *>			listNotes;


	//for data file
	void GetBUSData(const QString& dataname="BUS");
	void GetBRANCHData(const QString& dataname="BRANCH");
	void GetTRANSMORMERData(const QString& dataname="TRANSFORMER");
	void GetBaseParameter(QFile& file);
	void GetDataModel(QFile& file,T_DATA datatype);

	//for data and map files
	void readMapElement();
	void readStations();
	void readStatElement();

	void readNodes(QList<iNodeData *> &listNodes);
	void skipUnknownElement();
	QXmlStreamReader xmlReader;

	QSize			m_AreaSize;
	QString			Prefix_keyword;
	QString			Suffix_keyword;
	QString			ColumnName_keyword;
	ControlPanel	m_controlpanel;
	double			SBase;
	QString			m_szDataFile;
	QString			m_szMapFile;
	bool			m_bModified;
signals:
	void controlpanelChanged(ControlPanel &settings,uint changes);
	void areaSizeChanged(QSize & size);
	void statAdded(iSTAT*,const QPointF&,const QFont&,QPointF&,QPointF&);

};

#endif // IDOC_H
