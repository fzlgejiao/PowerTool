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
enum Columns
{
	Type=0,
	ID,
	Name,
};

class AddDialog : public QDialog
{
	Q_OBJECT

public:
	AddDialog(iDoc * idoc ,iSTAT * editstation,QWidget *parent = 0);
	
	~AddDialog();
	bool IsAddSite();
	bool IsRemovedSite();
	QString NewSiteName();
	QList<iNodeData *>& GetAddedNode(){return addednodelist;}
	QList<iNodeData *>& GetRemovedNode(){return hiddennodelist;}

private slots:
		void OnHiddenTableActived(int row,int column);
		void OnAddedTableActived(int row,int column);
		void OnAdd();
		void OnAddAll();
		void OnRevoke();
		void OnRevokeAll();		
		
private:
	Ui::AddDialog ui;
	QList<iNodeData *> hiddennodelist;
	QList<iNodeData *> addednodelist;
	
	iDoc *m_doc;
	iSTAT *m_editstation;
	bool is_edit;
	void addNode2Rows(QTableWidget *tablewidget, iNodeData *node);	
	iNodeData * GetNodefromID(int nodeid,QList<iNodeData *> nodelist);
	void SetTableStyle(QTableWidget *tablewidget);	
	//void addGeneratorRows(iGENERATOR *generator);
	

};

#endif // ADDDIALOG_H
