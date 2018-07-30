#include "adddialog.h"
#include "idoc.h"
#include "idata.h"
#include <QTableWidget>

AddDialog::AddDialog(iDoc *idoc ,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.lineEdit_name->setPlaceholderText("Sub-");
	m_doc=idoc;
	m_doc->getAvailableBus(hiddenbuslist);	
	
	SetTableStyle(ui.tableWidget_hidden);
	SetTableStyle(ui.tableWidget_added);
		
	foreach(iBUS *bus ,hiddenbuslist)
		addBus2Rows(ui.tableWidget_hidden,bus);

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

iBUS * AddDialog::GetiBUSfromID(int busid,QList<iBUS *> buslist)
{
	foreach(iBUS *bus,buslist)
	{
		if(bus->Id()==busid)
			return bus;
	}
	return NULL;
}

bool AddDialog::IsAddSite()
{
	if(addedbuslist.count()>0) return true;
	else return false;
}

void AddDialog::OnHiddenTableActived(int row,int column)
{
	ui.pushButton_add->setEnabled(true);
}
void AddDialog::OnAddedTableActived(int row,int column)
{
	ui.pushButton_remove->setEnabled(true);
}
void AddDialog::OnAdd()
{	
	if(hiddenbuslist.count()==0) return;
	QList<QTableWidgetItem*> items=ui.tableWidget_hidden->selectedItems();
	int count=items.count();
	int columncnt=3;
	int rowscnt=count/columncnt;
	
	for(int row=0;row<rowscnt;row++)
	{
		QTableWidgetItem *item=items.at(columncnt*row+ID);							//get the bus ID cell				
		iBUS *bus= GetiBUSfromID(item->text().toInt(),hiddenbuslist);
		if(bus==NULL) return;
		addedbuslist.append(bus);

		addBus2Rows(ui.tableWidget_added,bus);
		//remove the row		
		int selectrow=ui.tableWidget_hidden->row(items.at(row*columncnt));
		ui.tableWidget_hidden->removeRow(selectrow);
		hiddenbuslist.removeOne(bus);	
	}		
	
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
		iBUS *bus= GetiBUSfromID(iditem->text().toInt(),hiddenbuslist);
		if(bus==NULL) return;
		addedbuslist.append(bus);

		addBus2Rows(ui.tableWidget_added,bus);
		//remove the row		
		hiddenbuslist.removeOne(bus);
		ui.tableWidget_hidden->removeRow(firstrow);
	}		
}
void AddDialog::OnRevoke()
{
	if(addedbuslist.count()==0) return;
	QList<QTableWidgetItem*> items=ui.tableWidget_added->selectedItems();
	int count=items.count();
	int columncnt=3;
	int rowscnt=count/columncnt;
		
	for(int row=0;row<rowscnt;row++)
	{
		QTableWidgetItem *item=items.at(columncnt*row+ID);		//get the bus ID cell
		iBUS *bus= GetiBUSfromID(item->text().toInt(),addedbuslist);
		if(bus==NULL) return;
		hiddenbuslist.append(bus);
		addBus2Rows(ui.tableWidget_hidden,bus);
		//remove the row
		int selectrow=ui.tableWidget_added->row(items.at(row*columncnt));
		ui.tableWidget_added->removeRow(selectrow);
		addedbuslist.removeOne(bus);
	}
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
		iBUS *bus= GetiBUSfromID(iditem->text().toInt(),addedbuslist);
		if(bus==NULL) return;
		hiddenbuslist.append(bus);

		addBus2Rows(ui.tableWidget_hidden,bus);
		//remove the row		
		addedbuslist.removeOne(bus);
		ui.tableWidget_added->removeRow(firstrow);
	}		
}
void AddDialog::addBus2Rows(QTableWidget *tablewidget, iBUS *bus)
{
	int row = tablewidget->rowCount();
	tablewidget->insertRow(row);

	QTableWidgetItem *item0 = new QTableWidgetItem("BUS");	
	QTableWidgetItem *item1 = new QTableWidgetItem();
	QTableWidgetItem *item2 = new QTableWidgetItem();
				
	item1->setText(QString::number(bus->Id()));
	item2->setText(bus->name());
	
	tablewidget->setItem(row, Type, item0);
	tablewidget->setItem(row, ID, item1);
	tablewidget->setItem(row, Name, item2);
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
