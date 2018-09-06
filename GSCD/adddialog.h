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

enum Columns
{
	Type=0,
	ID,
	Name,
	StationColumnCnt
};

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


private slots:
		void OnHiddenTableActived(int row,int column);
		void OnAddedTableActived(int row,int column);
		void OnAdd();
		void OnAddAll();
		void OnRevoke();
		void OnRevokeAll();		
		void OnFontdialog();
		void OnComboAreaChanged(int index);
		void OnStatTypeChanged(int index);
		void OnBranchNodeAdd();
		
private:
	Ui::AddDialog ui;
	QList<iNodeData *> hiddennodelist;
	QList<iNodeData *> addednodelist;
	
	//QList<iNodeData *> Rawaddednodelist;
	QList<iNodeData *> Branchnodelist;

	iDoc *m_doc;
	iSTAT *m_editstation;
	bool is_edit;
	void addNode2Rows(QTableWidget *tablewidget, iNodeData *node);	
	iNodeData * GetNodefromID(int nodeid,QList<iNodeData *> nodelist);
	void SetTableStyle(QTableWidget *tablewidget);	
	void ClearTableContext(QTableWidget *tablewidget);
	//void addGeneratorRows(iGENERATOR *generator);
	
	QFont m_font;
	STAT_TYPE m_type;

};

#endif // ADDDIALOG_H
