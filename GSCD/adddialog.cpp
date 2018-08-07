#include "adddialog.h"
#include "idoc.h"
#include "idata.h"
#include <QTableWidget>

AddDialog::AddDialog(iDoc *idoc,iSTAT * editstation,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	SetTableStyle(ui.tableWidget_hidden);
	SetTableStyle(ui.tableWidget_added);
	SetTableStyle(ui.tableWidget_branch);

	m_doc=idoc;
	m_editstation=editstation;
	m_doc->getAvailableNode(hiddennodelist);	
	
	if(m_editstation)
	{
		is_edit=true;
		ui.lineEdit_name->setText(m_editstation->name());

		foreach(iNodeData *node,m_editstation->nodeDatas())
		{
			addNode2Rows(ui.tableWidget_added,node);
			addednodelist.append(node);
		}
		Rawaddednodelist=addednodelist;
	}else
	{
		is_edit=false;
		ui.lineEdit_name->setPlaceholderText("Sub-");
	}
	foreach(iNodeData *bus ,hiddennodelist)
		addNode2Rows(ui.tableWidget_hidden,bus);

	connect(ui.tableWidget_hidden,SIGNAL(cellClicked (int,int)),this,SLOT(OnHiddenTableActived(int,int)));
	connect(ui.tableWidget_added,SIGNAL(cellClicked (int,int)),this,SLOT(OnAddedTableActived(int,int)));
	connect(ui.pushButton_add,SIGNAL(clicked()),this,SLOT(OnAdd()));
	connect(ui.pushButton_addall,SIGNAL(clicked()),this,SLOT(OnAddAll()));
	connect(ui.pushButton_remove,SIGNAL(clicked()),this,SLOT(OnRevoke()));
	connect(ui.pushButton_removeall,SIGNAL(clicked()),this,SLOT(OnRevokeAll()));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}
AddDialog::~AddDialog()
{

}
void AddDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	tablewidget->horizontalHeader()->setStretchLastSection(true);	
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
	tablewidget->setColumnWidth(Type,50);
	tablewidget->setColumnWidth(ID,50);
	tablewidget->setColumnWidth(Name,110);
	//set style	 
	tablewidget->setStyleSheet("selection-background-color:lightblue;"); 
	tablewidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}
void AddDialog::ClearTableContext(QTableWidget *tablewidget)
{
	int Cnt = tablewidget->rowCount();

	for(int i=0;i<Cnt;i++)   
	{        
		tablewidget->removeRow(0); 
	}
}

iNodeData * AddDialog::GetNodefromID(int nodeid,QList<iNodeData *> nodelist)
{
	foreach(iNodeData *node,nodelist)
	{
		if(node->Id()==nodeid)
			return node;
	}
	return NULL;
}
bool AddDialog::IsAddSite()
{
	if(addednodelist.count()>0) return true;
	else return false;
}

bool AddDialog::IsRemovedSite()
{
	if(hiddennodelist.count()>0) return true;
	else return false;
}
void AddDialog::GetNewAddedNodes(QList<iNodeData *>& nodes)
{
	if(Rawaddednodelist.count()==0)
		nodes=addednodelist;
	else{
		foreach(iNodeData *node,addednodelist)
		{
			if(!Rawaddednodelist.contains(node))
				nodes.append(node);
		}
	}
}
void AddDialog::GetNewRemovedNodes(QList<iNodeData *>& nodes)
{	
	foreach(iNodeData *node,Rawaddednodelist)
	{
		if(!addednodelist.contains(node))
			nodes.append(node);		
	}	
}
QString AddDialog::NewSiteName()
{
	//QString name;
	//if(addednodelist.count()==0) return NULL;
	QString nametxt=ui.lineEdit_name->text().trimmed();
	if(nametxt==NULL) 
	{
		if(addednodelist.first()->type()==T_BUS)					//if not new the name ,use the first node's name
		{
			return ((iBUS *)addednodelist.first())->name();			
		}
	}		
	else return nametxt;
}
void AddDialog::OnHiddenTableActived(int row,int column)
{
	ui.pushButton_add->setEnabled(true);

	Branchnodelist.clear();
	ClearTableContext(ui.tableWidget_branch);	
	QList<QTableWidgetItem*> items=ui.tableWidget_hidden->selectedItems();
	QTableWidgetItem *Iditem=items.at(ID);
	iNodeData *selectnode= GetNodefromID(Iditem->text().toInt(),hiddennodelist);
	if(selectnode->type()==T_BUS)
	{
		QString connection=QString(tr("connection with %1")).arg(((iBUS *)selectnode)->name());
		ui.label_branch->setText(connection);

		foreach(iLinkData *link,selectnode->linkDatas())
		{			
			iNodeData* fromnode = m_doc->getNode(link->fromUid());
			iNodeData* tonode = m_doc->getNode(link->toUid());	
			if(!fromnode || !tonode) continue;					
			if(selectnode!=fromnode)
				Branchnodelist.append(fromnode);
			if(selectnode!=tonode)
				Branchnodelist.append(tonode);
		}
		foreach(iNodeData *node,Branchnodelist)
			addNode2Rows(ui.tableWidget_branch,node);
	}
}
void AddDialog::OnAddedTableActived(int row,int column)
{
	ui.pushButton_remove->setEnabled(true);
	Branchnodelist.clear();
	ClearTableContext(ui.tableWidget_branch);	
	QList<QTableWidgetItem*> items=ui.tableWidget_added->selectedItems();
	QTableWidgetItem *Iditem=items.at(ID);
	iNodeData *selectnode= GetNodefromID(Iditem->text().toInt(),addednodelist);
	if(selectnode->type()==T_BUS)
	{
		QString connection=QString(tr("connection with %1")).arg(((iBUS *)selectnode)->name());
		ui.label_branch->setText(connection);

		foreach(iLinkData *link,selectnode->linkDatas())
		{			
			iNodeData* fromnode = m_doc->getNode(link->fromUid());
			iNodeData* tonode = m_doc->getNode(link->toUid());	
			if(!fromnode || !tonode) continue;					
			if(selectnode!=fromnode)
				Branchnodelist.append(fromnode);
			if(selectnode!=tonode)
				Branchnodelist.append(tonode);
		}
		foreach(iNodeData *node,Branchnodelist)
			addNode2Rows(ui.tableWidget_branch,node);
	}
}
void AddDialog::OnAdd()
{	
	if(hiddennodelist.count()==0) return;
	
	QList<QTableWidgetItem*> items=ui.tableWidget_hidden->selectedItems();
	int count=items.count();
	int columncnt=3;
	int rowscnt=count/columncnt;
	
	for(int row=0;row<rowscnt;row++)
	{
		QTableWidgetItem *item=items.at(columncnt*row+ID);							//get the bus ID cell				
		iNodeData *node= GetNodefromID(item->text().toInt(),hiddennodelist);
		if(node==NULL) return;
		addednodelist.append(node);
		
		addNode2Rows(ui.tableWidget_added,node);
		//remove the row		
		int selectrow=ui.tableWidget_hidden->row(items.at(row*columncnt));
		ui.tableWidget_hidden->removeRow(selectrow);
		hiddennodelist.removeOne(node);	
		
	}		
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	ui.tableWidget_hidden->clearSelection();
}
void AddDialog::OnAddAll()
{
	if(ui.tableWidget_hidden->rowCount()==0) return;
	int rowscnt=ui.tableWidget_hidden->rowCount();
	int columncnt=3;
	int firstrow=0;
	for(int i=0;i<rowscnt;i++)
	{
		ui.tableWidget_hidden->selectRow(firstrow);	
		QList<QTableWidgetItem*> items=ui.tableWidget_hidden->selectedItems();
		QTableWidgetItem* iditem=items.at(ID);
		iNodeData *node= GetNodefromID(iditem->text().toInt(),hiddennodelist);
		if(node==NULL) return;
		addednodelist.append(node);
		
		addNode2Rows(ui.tableWidget_added,node);
		//remove the row		
		hiddennodelist.removeOne(node);
		
		ui.tableWidget_hidden->removeRow(firstrow);
	}		
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	
}
void AddDialog::OnRevoke()
{
	if(ui.tableWidget_added->rowCount()==0) return;
	
	QList<QTableWidgetItem*> items=ui.tableWidget_added->selectedItems();
	int count=items.count();
	int columncnt=3;
	int rowscnt=count/columncnt;
		
	for(int row=0;row<rowscnt;row++)
	{
		QTableWidgetItem *item=items.at(columncnt*row+ID);		//get the bus ID cell
		iNodeData *node= GetNodefromID(item->text().toInt(),addednodelist);
		if(node==NULL) return;
		hiddennodelist.append(node);
		
		addNode2Rows(ui.tableWidget_hidden,node);
		//remove the row
		int selectrow=ui.tableWidget_added->row(items.at(row*columncnt));
		ui.tableWidget_added->removeRow(selectrow);
		addednodelist.removeOne(node);
		
	}
	if(is_edit) 
		{
			if(ui.tableWidget_added->rowCount()==0)
			ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	ui.tableWidget_added->clearSelection();
}
void AddDialog::OnRevokeAll()
{
	if(ui.tableWidget_added->rowCount()==0) return;

	int rowscnt=ui.tableWidget_added->rowCount();
	int columncnt=3;
	int firstrow=0;
	for(int i=0;i<rowscnt;i++)
	{
		ui.tableWidget_added->selectRow(firstrow);	
		QList<QTableWidgetItem*> items=ui.tableWidget_added->selectedItems();
		QTableWidgetItem* iditem=items.at(ID);
		iNodeData *node= GetNodefromID(iditem->text().toInt(),addednodelist);
		if(node==NULL) return;
		hiddennodelist.append(node);
		
		addNode2Rows(ui.tableWidget_hidden,node);
		//remove the row		
		addednodelist.removeOne(node);
		
		ui.tableWidget_added->removeRow(firstrow);
	}		
	if(is_edit) ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
void AddDialog::addNode2Rows(QTableWidget *tablewidget, iNodeData *node)
{
	int row = tablewidget->rowCount();
	tablewidget->insertRow(row);
	if(node->type()==T_BUS)
	{
	QTableWidgetItem *item0 = new QTableWidgetItem("BUS");	
	QTableWidgetItem *item1 = new QTableWidgetItem();
	QTableWidgetItem *item2 = new QTableWidgetItem();
				
	item1->setText(QString::number(((iBUS *)node)->Id()));
	item2->setText(((iBUS *)node)->name());
	
	tablewidget->setItem(row, Type, item0);
	tablewidget->setItem(row, ID, item1);
	tablewidget->setItem(row, Name, item2);
	}	
}
//void AddDialog::addGeneratorRows(iGENERATOR *generator);
//{
//	int row = ui.tableWidget_hidden->rowCount();
//	ui.tableWidget_hidden->insertRow(row);
//	
//
//	QTableWidgetItem *item0 = new QTableWidgetItem("GENERATOR");
//	QTableWidgetItem *item1 = new QTableWidgetItem();
//	QTableWidgetItem *item2 = new QTableWidgetItem();
//	
//	item1->setText(QString::number(generator->Id()));
//	item2->setText(generator->name());
//
//	ui.tableWidget_hidden->setItem(row, Type, item0);
//	ui.tableWidget_hidden->setItem(row, ID, item1);
//	ui.tableWidget_hidden->setItem(row, Name, item2);
//}
