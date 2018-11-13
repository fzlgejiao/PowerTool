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

	SetTableStyle(ui.tableWidget_branchs);
	iSTAT *fromstation=m_link->startStat();
	iSTAT *endstation=m_link->endStat();

	ui.lineEdit_from->setText(fromstation->name());
	ui.lineEdit_fromstation->setText(fromstation->name());

	ui.lineEdit_to->setText(endstation->name());
	ui.lineEdit_tostation->setText(endstation->name());

	//Set Tree 
	ui.treeWidget->setHeaderHidden(true);
	ui.treeWidget->setColumnCount(1);

	for(int i=0;i<m_link->groupCount();i++)
	{
		m_linklist.append(m_link->groupLinkDatas(i));
		addlink2tree(ui.treeWidget,m_link->groupLinkDatas(i));
	}

	foreach(iLinkData *link,m_linklist)
		addlink2Rows(ui.tableWidget_branchs,link);
	
	connect(ui.treeWidget,SIGNAL(itemPressed(QTreeWidgetItem *,int)),this,SLOT(OntreeitemPressed(QTreeWidgetItem *,int)));
	connect(ui.pushButton_addgroup,SIGNAL(clicked()),this,SLOT(OnaddGroup()));
	connect(ui.pushButton_parallel,SIGNAL(toggled(bool)),this,SLOT(Onparalleltoggled(bool)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	

	ui.buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	ui.buttonBox->button(QDialogButtonBox::Help)->setEnabled(false);
}
BranchEditDialog::~BranchEditDialog()
{

}
void BranchEditDialog::Onparalleltoggled(bool toggle)
{
	if(toggle) 
		m_enableparalleline=true;
	else 
		m_enableparalleline=false;
}
void BranchEditDialog::OntreeitemPressed(QTreeWidgetItem *item,int column)
{
	int index=ui.treeWidget->indexOfTopLevelItem(item);
	if(index>=0) //if this item is top level item ,return 
	{
		ui.pushButton_addgroup->setEnabled(false);
		return ;									
	}
	if(item->parent()->childCount()>1) 
		ui.pushButton_addgroup->setEnabled(true);
	else 
		ui.pushButton_addgroup->setEnabled(false);
}
void BranchEditDialog::OnaddGroup()
{
	QTreeWidgetItem *seleteditem=ui.treeWidget->currentItem();
	if(!seleteditem) return;

	QString txt=seleteditem->text(0);
	iLinkData *link;
	foreach(iLinkData *link,m_linklist)
	{
		
	}
	//addlink2tree(ui.treeWidget,newgroup);
}
void BranchEditDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	tablewidget->horizontalHeader()->setStretchLastSection(true);
	//tablewidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tablewidget->setColumnCount(BranchColumnCnt);										//Set  columns cnt
	tablewidget->horizontalHeader()->setClickable(true);				//set header can be click and sorting
	tablewidget->setSortingEnabled(true);
	tablewidget->setAlternatingRowColors(true);
	tablewidget->verticalHeader()->hide();								//hide vertical header
	tablewidget->setShowGrid(true);										// show grid
	tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);	//select all row
	tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//can bot be edit
	//set column width		
	tablewidget->setColumnWidth(Node1,80);
	tablewidget->setColumnWidth(Node2,80);
	tablewidget->setColumnWidth(ParallelCode,120);
	tablewidget->setColumnWidth(R_JX,100);
	tablewidget->setColumnWidth(P1_JQ1,100);
	tablewidget->setColumnWidth(P2_JQ2,100);
	//set style	 
	tablewidget->setStyleSheet("selection-background-color:lightblue;"); 
	tablewidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}
void BranchEditDialog::addlink2Rows(QTableWidget *tablewidget, iLinkData *link)
{
	int row = tablewidget->rowCount();
	tablewidget->insertRow(row);
	if(link->type()==T_BRANCH)
	{
		iBRANCH *branch=(iBRANCH *)link;
		QTableWidgetItem *fromitem = new QTableWidgetItem();		
		QTableWidgetItem *enditem = new QTableWidgetItem();
		QTableWidgetItem *codeitem = new QTableWidgetItem();
		QTableWidgetItem *rxitem = new QTableWidgetItem();

		fromitem->setText(branch->getFromBus()->name());
		enditem->setText(branch->getToBus()->name());
		codeitem->setText(QString::number(branch->getParallelCode()));
		rxitem->setText(branch->getRX());

		tablewidget->setItem(row, Node1, fromitem);
		tablewidget->setItem(row, Node2, enditem);
		tablewidget->setItem(row, ParallelCode, codeitem);
		tablewidget->setItem(row, R_JX, rxitem);
	}	
}
void BranchEditDialog::addlink2tree(QTreeWidget *treewidget,  QList<iLinkData * > linkgroup)
{
	if(linkgroup.count()==0) return;
	int index=treewidget->topLevelItemCount();

	QTreeWidgetItem * rootgroup=new  QTreeWidgetItem(treewidget,QStringList(QString("Group-[%1]").arg(index+1)));
	
	QList<QTreeWidgetItem *> items;
	for(int i=0;i<linkgroup.count();i++)
	{		
		if(linkgroup[i]->type()==T_BRANCH)
		{
			iBRANCH *branch=(iBRANCH *)linkgroup[i];
			items.append(new QTreeWidgetItem(rootgroup,QStringList(QString("%1 %2 ---- %3 %4 (%5)").arg(branch->getFromBus()->name()).arg(branch->getFromBus()->GetRefVoltage()).arg(branch->getToBus()->name()).arg(branch->getToBus()->GetRefVoltage()).arg(branch->getParallelCode()))));
		}
	}

	if(items.count()>0) treewidget->insertTopLevelItems(index, items);
}
