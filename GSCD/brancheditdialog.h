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
	P2_JQ2
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

private:
	void SetTableStyle(QTableWidget *tablewidget);
	void addlink2Rows(QTableWidget *tablewidget, iLinkData *link);

	Ui::BranchEditDialog ui;
	iSLINK *m_link;
	QList<iLinkData *> m_linklist;

};

#endif // BRANCHEDITDIALOG_H
