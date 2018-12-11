#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "ui_adddialog.h"
#include <QList>
#include "idata.h"
#include <QTableWidget>

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
	StationColumnCnt
};

typedef enum
{
	NoIcon   =0,
	BranchIcon,
	TransformerIcon
}IconType;

typedef enum  
{		
	UnSelectable   =100,
	LinkSelected,
	LinkUnSelected,
	TransformerSelected,
	TransformerUnSelected
}IconItemType;

class AddDialog : public QDialog
{
	Q_OBJECT

public:
	AddDialog(iDoc * idoc ,iSTAT * editstation,QWidget *parent = 0);
	
	~AddDialog();
	bool IsAddSite();
	bool IsRemovedSite();
	QString NewStationName();
	QList<iNodeData *>&  GetAddedNodes(){return addednodelist;}
	QFont GetFont() {return m_font;}
	//void GetRemovedNodes(QList<iNodeData *>& nodes);
	STAT_TYPE getstationtype(){return m_type;}
	ushort	changes(){return m_changes;}


private slots:
		void OnHiddenTableActived(int row,int column);
		void OnAddedTableActived(int row,int column);
		void OnBranchnodeActived(int row,int column);
		void OnAdd();
		void OnAddAll();
		void OnRevoke();
		void OnRevokeAll();		
		void OnFontdialog();
		void OnComboAreaChanged(int index);
		void OnStatTypeChanged(int index);
		void OnBranchNodeAdd();
		void OnnameChanged(const QString &name);
		void OnOk();
		void acceptVScrollValueChanged(int);
		
private:
	Ui::AddDialog ui;
	QList<iNodeData *> hiddennodelist;
	QList<iNodeData *> addednodelist;
	
	//QList<iNodeData *> Rawaddednodelist;
	QList<iNodeData *> Branchnodelist;

	iDoc *m_doc;
	iSTAT *m_editstation;
	bool is_edit;
	void addNode2Rows(QTableWidget *tablewidget, iNodeData *node,IconType icon= NoIcon,bool isselectable=true);		
	//iNodeData * GetNodefromItem(QString name,double refvlotage,QString areaname,QList<iNodeData *> nodelist);
	int finditemrow(iNodeData *node);
	void showConnectionNode(QTableWidget *tablewidget);
	void SetTableStyle(QTableWidget *tablewidget);	
	void ClearTableContext(QTableWidget *tablewidget);
	bool checknodecanbeselected(iNodeData *node);
	//void addGeneratorRows(iGENERATOR *generator);
	
	QFont		m_font;
	STAT_TYPE	m_type;
	ushort		m_changes;
};

#endif // ADDDIALOG_H
