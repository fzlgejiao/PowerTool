#ifndef IDOC_H
#define IDOC_H

#include <QObject>
#include <QMap>
#include <QFile>
#include "idata.h"


class iDoc : public QObject
{
	Q_OBJECT

public:
	iDoc(QObject *parent);
	~iDoc();

	bool	openDataFile(const QString& file);
	bool	openMapFile(const QString& file);
	void	close();

	int		STAT_getId();																			//get available station id
	iSTAT*	STAT_get(int id){return listSTAT.value(id,NULL);}
	void	STAT_delete(int id)
	{
		iSTAT* stat = STAT_get(id);
		if(stat)
			delete stat;
		listSTAT.remove(id);
	}
	void	STAT_add(iSTAT* stat){listSTAT.insert(stat->Id(),stat);}
	
	iNodeData* getNode(int uid);
	iNodeData* getNode(int type,int id);

	iBUS*	getBUS(int id){return listBUS.value(id,NULL);}
	iBRANCH*	getBRANCH(int id){return listBRANCH.value(id,NULL);}
	iTRANSFORMER*	getTRANSFORMER(int id){return listTRANSFORMER.value(id,NULL);}

	void	getAvailableNode(QList<iNodeData *>& list);												//to get buses which need to be show in scene
	
	iData*	Uid2Data(int uid);

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


private:
	QMap<int,iSTAT *>			listSTAT;															//<id,iSTAT*>: id is the station id
	QMap<int,iBUS *>			listBUS;															//<id,iBUS*>: id is the bus id
	QMap<int,iBRANCH *>			listBRANCH;															//<id,iBRANCH*>: id is the branch squence id
	QMap<int,iTRANSFORMER *>	listTRANSFORMER;													//<id,iTRANSFORMER*>: id is the transformer squence id

	void GetBUSData(const QString& dataname="BUS");
	void GetBRANCHData(const QString& dataname="BRANCH");
	void GetTRANSMORMERData(const QString& dataname="TRANSFORMER");

	void GetDataModel(T_DATA datatype);

	bool OpenDataFile(const QString& file );
	void CloseDataFile();

	QFile *m_file;

	QString Prefix_keyword;
	QString Suffix_keyword;
	QString	ColumnName_keyword;
};

#endif // IDOC_H
