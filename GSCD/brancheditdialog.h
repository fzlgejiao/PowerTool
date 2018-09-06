#ifndef BRANCHEDITDIALOG_H
#define BRANCHEDITDIALOG_H

#include <QDialog>
#include "ui_brancheditdialog.h"

class iSLINK;
class iLinkData;


enum BranchColumns
{
	Node1	=0,
	Node2,
	ParallelCode,
	R_JX,
	P1_JQ1,
	P2_JQ2,
	BranchColumnCnt
};
enum BranchAction
{
	BranchEdit=0,
	BranchView
};
class BranchEditDialog : public QDialog
{
	Q_OBJECT

public:
	BranchEditDialog(iSLINK *link,BranchAction action,QWidget *parent = 0);
	~BranchEditDialog();
	bool enbaleParalleLine(){return m_enableparalleline;}

private slots:
	void OntreeitemPressed(QTreeWidgetItem *item,int column);
	void OnaddGroup();
	void Onparalleltoggled(bool toggle);

private:
	void SetTableStyle(QTableWidget *tablewidget);
	void addlink2Rows(QTableWidget *tablewidget, iLinkData *link);
	void addlink2tree(QTreeWidget *treewidget, QList<iLinkData * > linkgroup);
	
	Ui::BranchEditDialog ui;
	iSLINK *m_link;
	QList<iLinkData *> m_linklist;
	bool			   m_enableparalleline;

};

#endif // BRANCHEDITDIALOG_H
