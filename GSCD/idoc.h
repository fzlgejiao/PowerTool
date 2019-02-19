#ifndef IDOC_H
#define IDOC_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <QFile>
#include "idata.h"
#include <QSize>
#include <QXmlStreamReader>

//#define Link2ID(from,to,ckt)	((((quint64)from)<<32) | (to<<4) | (ckt))		//use the unsigned int 64bit data,ckt is very small, maybe just less than 10
	

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
	bool    readPfFile(const QString& fileName=QString());
	QString dataFile(){return m_szDataFile;}
	QString mapFile(){return m_szMapFile;}
	QString pfFile(){return m_PfFile;}
	void	setDataFile(const QString& dataFile){m_szDataFile = dataFile;}
	void	setPfFile(const QString& file){m_PfFile = file;}
	bool	writeMapFile(const QString& mapFile);

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

	iBRANCH*	getBRANCHfromHash(QString & linkid){return BRANCH_hash.value(linkid,NULL);}
	iTRANSFORMER*	getTRANSFORMERfromHash(QString & linkid){return TRANSFORMER_hash.value(linkid,NULL);}

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

	QStringList wanningmessage(){return m_wanningmessage;}

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
	void	addFACTSDEVICE(iFACTSDEVICE* device){listFACTSDEVICE.insert(device->Id(),device);}

private:
	QMap<int,iSTAT *>			listSTAT;															//<id,iSTAT*>: id is the station id
	QMap<int,iSLINK *>			listSLINK;															//<id,iSLINK*>: id is the station link id
	QMap<int,iBUS *>			listBUS;															//<id,iBUS*>: id is the bus id
	QMap<int,iBRANCH *>			listBRANCH;															//<id,iBRANCH*>: id is the branch squence id
	QMap<int,iTRANSFORMER *>	listTRANSFORMER;													//<id,iTRANSFORMER*>: id is the transformer squence id
	QHash<QString,iBRANCH *>		BRANCH_hash;															//<id,iBRANCH*>: id use the link message
	QHash<QString,iTRANSFORMER *>	TRANSFORMER_hash;													//<id,iTRANSFORMER*>: id use the link message
	QMap<int,iAREA *>			listAREA;
	QMap<int,iNote *>			listNotes;
	QMap<int,iFACTSDEVICE *>	listFACTSDEVICE;
		
	inline QString  Link2ID(int from,int to,int ckt) { return QString("%1%2%3").arg(from).arg(to).arg(ckt);}

	//for data file
	void GetBUSData(const QString& dataname="BUS");
	void GetBRANCHData(const QString& dataname="BRANCH");
	void GetTRANSMORMERData(const QString& dataname="TRANSFORMER");
	void GetBaseParameter(QFile& file);
	void GetDataModel(QFile& file);
	void ParserPowerFlow(QString & filename);
	T_DATA getdatatype(QString stringline);
	//for map file
	void readSettings();
	void readMapElement();
	void readStations();
	void readStatElement();
	void readNodes(QList<iNodeData *> &listNodes);

	void writeSettings(QXmlStreamWriter *xmlWriter);
	void writeStats(QXmlStreamWriter *xmlWriter);
	void writeNotes(QXmlStreamWriter *xmlWriter);
	void writeLegends(QXmlStreamWriter *xmlWriter);

	void readNotes();
	void skipUnknownElement();
	QXmlStreamReader xmlReader;
	
	QSize			m_AreaSize;
	QString			Prefix_keyword;
	QString			Suffix_keyword;
	QString			ColumnName_keyword;
	ControlPanel	m_controlpanel;
	double			SBase;
	QString			m_szDataFile;
	QString			m_PfFile;
	QString			m_szMapFile;
	bool			m_bModified;
	QStringList     m_wanningmessage;
	QMap<T_DATA,QString> m_parserdatas;

signals:
	void controlpanelChanged(ControlPanel &settings,uint changes);
	void areaSizeChanged(QSize & size);
	void statAdded(iSTAT*,const QPointF&,const QFont&,QPointF&,QPointF&);
	void noteAdded(iNote*,const QPointF& pos,const QSizeF& size);

};

#endif // IDOC_H
