#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "ui_adddialog.h"
#include <QList>
#include "idata.h"
#include <QTableWidget>
#include <QSortFilterProxyModel>

class iDoc;
class iNodeData;
class iSTAT;
class QFont;

#define	CHG_STAT_STYPE		0x01
#define	CHG_STAT_NAME		0x02
#define	CHG_STAT_DATA		0x04

enum Columns
{
	//Type=0,
	ID		=0,
	Name,
	VB,	
	AreaName,
	StationColumnCnt
};

typedef enum
{
	NoIcon   =0,
	BranchIcon,
	TransformerIcon,
	FactsDeviceIcon,

	BranchSelectedIcon,
	TransformerSelectedIcon,
	FactsDeviceSelectedIcon,

	BranchUnSelectableIcon,
	TransformerUnSelectableIcon,
	FactsDeviceUnSelectableIcon
}IconType;

class BusNodeModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    BusNodeModel(QObject *parent=0);

    int rowCount(const QModelIndex &parent= QModelIndex()) const;
    int columnCount(const QModelIndex &parent= QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
   // Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);  
	
	bool insertmyrow( int row,iNodeData * node,IconType type,QString areaname);
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool contains(iNodeData *node){ return listnodes.contains(node);}
	inline void clear(){listnodes.clear();listtypes.clear();listareanames.clear();}
	QList<iNodeData * >	& nodeslist(){return listnodes;}
	void toggleicontype(const QModelIndex &index);
	bool isselected(const QModelIndex &index);
	QModelIndex  getIndex(iNodeData *node);

private:
	QList<iNodeData * >			  listnodes;													//bus nodes
	QList<QString >				  listareanames;												//bus nodes belonged area name
	QMap<IconType,QIcon*>		  listIcons;													//types
	QList<IconType >			  listtypes;
};


class SortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SortFilterModel(QObject *parent = 0);    
	void setCondition(const QString& condition){ m_conditionstring=condition; invalidateFilter();}	
	void setAreafilter(const QString& name){m_areanamefilter=name;invalidateFilter();}

protected:   
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
 	  
private:
    QString m_conditionstring;
	QString m_areanamefilter;
	int     m_column;
};

class AddDialog : public QDialog
{
	Q_OBJECT

public:
	AddDialog(iDoc * idoc ,iSTAT * editstation,QWidget *parent = 0);
	
	~AddDialog();
	bool IsAddSite();
	//bool IsRemovedSite();
	QString NewStationName();
	QList<iNodeData *>&  GetAddedNodes(){return addedmodel->nodeslist();}
	QFont GetFont() {return m_font;}	
	STAT_TYPE getstationtype(){return m_type;}
	ushort	changes(){return m_changes;}


private slots:
		void OnHiddenTableActived(const QModelIndex & index);
		void OnAddedTableActived(const QModelIndex & index);
		void OnBranchnodeActived(const QModelIndex & index);
		void OnAdd(const QModelIndex &index);
		void OnClickAdd();
		void OnAddAll();
		void OnRevoke(const QModelIndex &index);
		void OnClickRevoke();
		void OnRevokeAll();		
		void OnFontdialog();
		void OnComboAreaChanged(int index);
		void OnStatTypeChanged(int index);
		void OnBranchNodeAdd();
		void OnnameChanged(const QString &name);
		void OnOk();		
		void Onhiddenmodelchanged();
		void Onaddedmodelchanged();
		void Onfiltertextchanged(const QString &text);
		void OnComboFilterchanged(int index);
		
private:
	Ui::AddDialog ui;
	QList<iNodeData *> hiddennodelist;
	
	BusNodeModel * hiddenmodel;
	SortFilterModel *m_sortfilter;
	
	BusNodeModel * addedmodel;
	QSortFilterProxyModel *addedmodelproxy;

	BusNodeModel * branchnodemodel;
	QSortFilterProxyModel *branchmodelproxy;
	

	iDoc *m_doc;
	iSTAT *m_editstation;
	bool is_edit;	
	
	void showConnectionNode(iNodeData *selectnode);
	void SetTableStyle(QTableView *tableview);	
	bool checknodecanbeselected(iNodeData *node);
	void setColumnWidth(QTableView *tableview);
		
	QFont		m_font;
	STAT_TYPE	m_type;
	ushort		m_changes;
signals:
	void hiddenmodelchanged();
	void addedmodelchanged();
};

#endif // ADDDIALOG_H
