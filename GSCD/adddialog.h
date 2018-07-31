#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "ui_adddialog.h"
#include <QList>
#include "idata.h"
#include <QTableWidget>

class iDoc;
class  iNode;

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
	AddDialog(iDoc * idoc ,QWidget *parent = 0);
	~AddDialog();
	bool IsAddSite();
	QString NewSiteName();
	QList<iBUS *>& GetAddedbus(){return addedbuslist;}

private slots:
		void OnHiddenTableActived(int row,int column);
		void OnAddedTableActived(int row,int column);
		void OnAdd();
		void OnAddAll();
		void OnRevoke();
		void OnRevokeAll();		
		
private:
	Ui::AddDialog ui;
	QList<iBUS *> hiddenbuslist;
	QList<iBUS *> addedbuslist;
	
	iDoc *m_doc;
	void addBus2Rows(QTableWidget *tablewidget, iBUS *bus);
	iBUS * GetiBUSfromID(int busid,QList<iBUS *> buslist);
	void SetTableStyle(QTableWidget *tablewidget);	
	//void addGeneratorRows(iGENERATOR *generator);
	

};

#endif // ADDDIALOG_H
