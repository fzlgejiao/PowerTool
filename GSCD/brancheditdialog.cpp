#include "brancheditdialog.h"
#include "idata.h"

BranchEditDialog::BranchEditDialog(iBRANCH *branch,BranchAction action,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());

	m_branch=branch;	
	if(action==BranchEdit)
		ui.tabWidget->setCurrentWidget(ui.edittab);
	else
		ui.tabWidget->setCurrentWidget(ui.viewtab);



}

BranchEditDialog::~BranchEditDialog()
{

}
