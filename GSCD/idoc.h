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
	iBUS*	getBUS(int id){return listBUS.value(id,NULL);}
	iBRANCH*	getBRANCH(int id){return listBRANCH.value(id,NULL);}
	iTRANSFORMER*	getTRANSFORMER(int id){return listTRANSFORMER.value(id,NULL);}

protected:

	void	addBUS(iBUS* bus){listBUS.insert(bus->id(),bus);}
	void	addBRANCH(iBRANCH* branch){listBRANCH.insert(branch->id(),branch);}
	void	addTRANSFORMER(iTRANSFORMER* transformer){listTRANSFORMER.insert(transformer->id(),transformer);}


private:
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
