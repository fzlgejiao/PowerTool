#include "brancheditdialog.h"
#include "idata.h"

BranchEditDialog::BranchEditDialog(iSLINK *link,BranchAction action,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());

	m_link=link;	
	if(action==BranchEdit)
		ui.tabWidget->setCurrentWidget(ui.edittab);
	else
		ui.tabWidget->setCurrentWidget(ui.viewtab);



}
BranchEditDialog::~BranchEditDialog()
{

}
void BranchEditDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	//tablewidget->horizontalHeader()->setStretchLastSection(true);
	tablewidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tablewidget->setColumnCount(3);										//Set 3 columns
	tablewidget->horizontalHeader()->setClickable(true);				//set header can be click and sorting
	tablewidget->setSortingEnabled(true);
	tablewidget->setAlternatingRowColors(true);
	tablewidget->verticalHeader()->hide();								//hide vertical header
	tablewidget->setShowGrid(true);										// show grid
	tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);	//select all row
	tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//can bot be edit
	//set column width		
	//set style	 
	tablewidget->setStyleSheet("selection-background-color:lightblue;"); 
	tablewidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}
void BranchEditDialog::addlink2Rows(QTableWidget *tablewidget, iLinkData *link)
{
	
}
