#ifndef BRANCHEDITDIALOG_H
#define BRANCHEDITDIALOG_H

#include <QDialog>
#include "ui_brancheditdialog.h"


class iBRANCH;


enum BranchAction
{
	BranchEdit=0,
	BranchView
};
class BranchEditDialog : public QDialog
{
	Q_OBJECT

public:
	BranchEditDialog(iBRANCH *branch,BranchAction action,QWidget *parent = 0);
	~BranchEditDialog();

private:
	Ui::BranchEditDialog ui;
	iBRANCH *m_branch;
};

#endif // BRANCHEDITDIALOG_H
