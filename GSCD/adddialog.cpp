#include "adddialog.h"
#include "idoc.h"
#include "idata.h"
#include "diagramtextitem.h"
#include <QTableWidget>
#include <QFontDialog>

AddDialog::AddDialog(iDoc *idoc,iSTAT * editstation,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_changes = 0;

	SetTableStyle(ui.tableWidget_hidden);
	SetTableStyle(ui.tableWidget_added);
	SetTableStyle(ui.tableWidget_branch);
	
	m_doc=idoc;
	
	m_editstation=editstation;
	m_doc->getAvailableNode(hiddennodelist);	
	foreach(iAREA *area,m_doc->getArealist())
		ui.comboBox_areas->addItem(area->name());
	this->setFixedSize(this->size());

	m_font=QFont("Times New Roman", 12, QFont::Bold);
	m_type=STAT_HYDROPLANT;

	if(m_editstation)
	{
		is_edit=true;
		m_font=editstation->itemName()->font();
		ui.lineEdit_name->setText(m_editstation->name());

		m_type=m_editstation->sType();
		ui.comboBox_StatType->setCurrentIndex(m_type);

		foreach(iNodeData *node,m_editstation->nodeDatas())
		{
			addNode2Rows(ui.tableWidget_added,node);
			addednodelist.append(node);
		}
		//Rawaddednodelist=addednodelist;
	}else
	{
		is_edit=false;			
		ui.lineEdit_name->setPlaceholderText("Sub-");
	}

	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	foreach(iNodeData *bus ,hiddennodelist)
		addNode2Rows(ui.tableWidget_hidden,bus);

	connect(ui.tableWidget_hidden,SIGNAL(cellClicked (int,int)),this,SLOT(OnHiddenTableActived(int,int)));
	connect(ui.tableWidget_added,SIGNAL(cellClicked (int,int)),this,SLOT(OnAddedTableActived(int,int)));
	connect(ui.tableWidget_hidden,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(OnAdd()));
	connect(ui.tableWidget_added,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(OnRevoke()));
	connect(ui.pushButton_add,SIGNAL(clicked()),this,SLOT(OnAdd()));
	connect(ui.pushButton_addall,SIGNAL(clicked()),this,SLOT(OnAddAll()));
	connect(ui.pushButton_remove,SIGNAL(clicked()),this,SLOT(OnRevoke()));
	connect(ui.pushButton_removeall,SIGNAL(clicked()),this,SLOT(OnRevokeAll()));
	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontdialog()));
	connect(ui.comboBox_areas,SIGNAL(currentIndexChanged(int)),this,SLOT(OnComboAreaChanged(int)));
	connect(ui.pushButton_branchnodeadd,SIGNAL(clicked()),this,SLOT(OnBranchNodeAdd()));
	connect(ui.comboBox_StatType,SIGNAL(currentIndexChanged(int)),this,SLOT(OnStatTypeChanged(int)));
	connect(ui.lineEdit_name,SIGNAL(textChanged(const QString &)),this,SLOT(OnnameChanged(const QString &)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(OnOk()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	

	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(ui.tableWidget_added->rowCount()));

	
}
AddDialog::~AddDialog()
{

}
void AddDialog::OnnameChanged(const QString &name)
{
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	m_changes|=CHG_STAT_NAME;
}
void AddDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	tablewidget->horizontalHeader()->setStretchLastSection(true);
	//tablewidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tablewidget->setColumnCount(StationColumnCnt);										//Set 3 columns
	tablewidget->horizontalHeader()->setClickable(true);				//set header can be click and sorting
	tablewidget->setSortingEnabled(true);
	tablewidget->setAlternatingRowColors(true);
	tablewidget->verticalHeader()->hide();								//hide vertical header
	tablewidget->setShowGrid(true);										// show grid
	tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);	//select all row
	tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//can bot be edit
	//set column width	
	tablewidget->setColumnWidth(Name,80);
	tablewidget->setColumnWidth(VB,80);
	tablewidget->setColumnWidth(BelongedAreaName,100);
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
void AddDialog::OnStatTypeChanged(int index)
{
	m_type=(STAT_TYPE)index;
	m_changes |=CHG_STAT_STYPE;
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
void AddDialog::OnComboAreaChanged(int index)
{
	QList<iNodeData *> list;
	if(index==0)
	{		
		list=hiddennodelist;
	}else
	{
		QString SelectAreaName=ui.comboBox_areas->currentText();
		iAREA *area=m_doc->getAREA(SelectAreaName);
		if(!area) return;
		foreach(iNodeData *node,hiddennodelist)
		{
			if(node->type()==T_BUS)
			{
				if(((iBUS *)node)->belongedArea()==area->Id())
				{
					list.append(node);
				}
			}
		}
	}
	//Clear current table
	ClearTableContext(ui.tableWidget_hidden);
	foreach(iNodeData *bus ,list)
		addNode2Rows(ui.tableWidget_hidden,bus);
	//Update counter
	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
}
//iNodeData * AddDialog::GetNodefromItem(QString name,double refvlotage,QString areaname,QList<iNodeData *> nodelist)
//{
//	foreach(iNodeData *node,nodelist)
//	{
//		double voltage=node->GetRefVoltage();
//		
//		if(node->type()==T_BUS)
//		{
//			iBUS *bus=(iBUS *)node;
//			iAREA *area=m_doc->getAREA(bus->belongedArea());
//			if( (bus->name()==name)        &&
//			    (voltage==refvlotage)      &&
//			    (area->name()==areaname)	)
//				return node;
//		}
//			
//	}
//	return NULL;
//}
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
//void AddDialog::GetRemovedNodes(QList<iNodeData *>& nodes)
//{	
//	foreach(iNodeData *node,Rawaddednodelist)
//	{
//		if(!addednodelist.contains(node))
//			nodes.append(node);		
//	}	
//}
QString AddDialog::NewStationName()
{
	QString name;
	if(addednodelist.count()==0) 
		return name;
	QString nametxt=ui.lineEdit_name->text().trimmed();
	if(nametxt==NULL) 
	{
		if(addednodelist.first()->type()==T_BUS)					//if not new the name ,use the first node's name
		{
			return ((iBUS *)addednodelist.first())->name(true);			
		}
	}		
	return nametxt;
}
void AddDialog::OnFontdialog()
{	
	bool isok;
	QFont font=QFontDialog::getFont(&isok,m_font,this);
	if(isok)
	{
		m_font=	font;
		m_changes|=CHG_STAT_NAME;
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}
void AddDialog::OnHiddenTableActived(int row,int column)
{
	ui.pushButton_add->setEnabled(true);

	Branchnodelist.clear();
	ClearTableContext(ui.tableWidget_branch);	
	QList<QTableWidgetItem*> items=ui.tableWidget_hidden->selectedItems();
	if(items.count()==0) return;

	QTableWidgetItem *Nameitem=items.at(Name);	
	iNodeData *selectnode=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();
	if(!selectnode) return;
	if(selectnode->type()==T_BUS)
	{
		QString connection=QString(tr("connection with %1")).arg(((iBUS *)selectnode)->name());
		ui.label_branch->setText(connection);

		foreach(iLinkData *link,selectnode->linkDatas())
		{			
			iNodeData* fromnode = m_doc->getNode(link->fromUid());
			iNodeData* tonode = m_doc->getNode(link->toUid());	
			if(!fromnode || !tonode) continue;					
			if((!Branchnodelist.contains(fromnode)) && (selectnode!=fromnode) )
			{
				Branchnodelist.append(fromnode);
				if(link->type()==T_BRANCH)
				{
					addNode2Rows(ui.tableWidget_branch,fromnode,BranchIcon);
				}else if(link->type()==T_TRANSFORMER)
				{
					addNode2Rows(ui.tableWidget_branch,fromnode,TransformerIcon);
				}
			}
			if((!Branchnodelist.contains(tonode)) && (selectnode!=tonode) )
			{
				Branchnodelist.append(tonode);
				if(link->type()==T_BRANCH)
				{
					addNode2Rows(ui.tableWidget_branch,tonode,BranchIcon);
				}else if(link->type()==T_TRANSFORMER)
				{
					addNode2Rows(ui.tableWidget_branch,tonode,TransformerIcon);
				}
			}
		}		
	}
}
void AddDialog::OnAddedTableActived(int row,int column)
{
	ui.pushButton_remove->setEnabled(true);
	Branchnodelist.clear();
	ClearTableContext(ui.tableWidget_branch);	
	QList<QTableWidgetItem*> items=ui.tableWidget_added->selectedItems();
	if(items.count()==0) return;

	QTableWidgetItem *Nameitem=items.at(Name);	
	iNodeData *selectnode=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();
	if(!selectnode)  return ;
	if(selectnode->type()==T_BUS)
	{
		QString connection=QString(tr("connection with %1")).arg(((iBUS *)selectnode)->name());
		ui.label_branch->setText(connection);

		foreach(iLinkData *link,selectnode->linkDatas())
		{			
			iNodeData* fromnode = m_doc->getNode(link->fromUid());
			iNodeData* tonode = m_doc->getNode(link->toUid());	
			if(!fromnode || !tonode) continue;				
			if((!Branchnodelist.contains(fromnode)) && (selectnode!=fromnode) )
			{
				Branchnodelist.append(fromnode);
				if(link->type()==T_BRANCH)
				{
					addNode2Rows(ui.tableWidget_branch,fromnode,BranchIcon);
				}else if(link->type()==T_TRANSFORMER)
				{
					addNode2Rows(ui.tableWidget_branch,fromnode,TransformerIcon);
				}
			}
			if((!Branchnodelist.contains(tonode)) && (selectnode!=tonode) )
			{
				Branchnodelist.append(tonode);
				if(link->type()==T_BRANCH)
				{
					addNode2Rows(ui.tableWidget_branch,tonode,BranchIcon);
				}else if(link->type()==T_TRANSFORMER)
				{
					addNode2Rows(ui.tableWidget_branch,tonode,TransformerIcon);
				}
			}
		}		
	}
}
void AddDialog::OnBranchNodeAdd()
{
	if(Branchnodelist.count()==0) return;

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
		QTableWidgetItem *Nameitem=items.at(Name);	
		iNodeData *node=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();		
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
	ui.label_branch->setText("connection with ....");
	ClearTableContext(ui.tableWidget_branch);

	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(ui.tableWidget_added->rowCount()));
	m_changes|=CHG_STAT_DATA;
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

		QTableWidgetItem *Nameitem=items.at(Name);	 
		iNodeData *node=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();	
		if(node==NULL) return;
		addednodelist.append(node);
		
		addNode2Rows(ui.tableWidget_added,node);
		//remove the row		
		hiddennodelist.removeOne(node);
		
		ui.tableWidget_hidden->removeRow(firstrow);
	}		

	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	ui.label_branch->setText("connection with ....");
	ClearTableContext(ui.tableWidget_branch);

	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(ui.tableWidget_added->rowCount()));
	m_changes|=CHG_STAT_DATA;
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
		QTableWidgetItem *Nameitem=items.at(Name);	
		iNodeData *node=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();		
		if(node==NULL) return;
		hiddennodelist.append(node);
		
		addNode2Rows(ui.tableWidget_hidden,node);
		//remove the row
		int selectrow=ui.tableWidget_added->row(items.at(row*columncnt));
		ui.tableWidget_added->removeRow(selectrow);
		addednodelist.removeOne(node);
		
	}
	
	if(ui.tableWidget_added->rowCount()==0)
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

	ui.tableWidget_added->clearSelection();
	ui.label_branch->setText("connection with ....");
	ClearTableContext(ui.tableWidget_branch);

	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(ui.tableWidget_added->rowCount()));
	m_changes|=CHG_STAT_DATA;
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

		QTableWidgetItem *Nameitem=items.at(Name); 
		iNodeData *node=(iNodeData *)Nameitem->data(Qt::UserRole).toUInt();

		if(node==NULL) return;

		hiddennodelist.append(node);
		
		addNode2Rows(ui.tableWidget_hidden,node);
		//remove the row		
		addednodelist.removeOne(node);
		
		ui.tableWidget_added->removeRow(firstrow);
	}		
	ui.label_branch->setText("connection with ....");
	ClearTableContext(ui.tableWidget_branch);
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	ui.lineEdit_hiddenCnt->setText(QString::number(ui.tableWidget_hidden->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(ui.tableWidget_added->rowCount()));
	m_changes|=CHG_STAT_DATA;
}
void AddDialog::addNode2Rows(QTableWidget *tablewidget, iNodeData *node,IconType icon)
{
	int row = tablewidget->rowCount();
	tablewidget->insertRow(row);
	if(node->type()==T_BUS)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem();	
		QTableWidgetItem *item1 = new QTableWidgetItem();
		QTableWidgetItem *item2 = new QTableWidgetItem();
		//binding data object to name item(itme0)
		item0->setData(Qt::UserRole,(uint)node);
		switch(icon)
		{	
		case BranchIcon:
			{
			item0->setIcon(QIcon(":/images/link.png"));			
			break;
			}
		case TransformerIcon:
			item0->setIcon(QIcon(":/images/transformer.png"));
			break;
		}

		item0->setText(((iBUS *)node)->name());			
		item1->setText(QString::number(node->GetRefVoltage()));
		item2->setText(m_doc->getAREA(((iBUS *)node)->belongedArea())->name());

		tablewidget->setItem(row, Name, item0);
		tablewidget->setItem(row, VB, item1);
		tablewidget->setItem(row, BelongedAreaName, item2);
	}	
}
void AddDialog::OnOk()
{
	if(m_editstation)
	{
		m_editstation->setName(NewStationName());
		if(m_changes & CHG_STAT_DATA)
			m_editstation->setNodes(addednodelist);
	}

	accept();
}