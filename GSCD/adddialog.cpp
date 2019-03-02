#include "adddialog.h"
#include "idoc.h"
#include "idata.h"
#include "diagramtextitem.h"
#include <QTableWidget>
#include <QFontDialog>
#include <QScrollBar>


BusNodeModel::BusNodeModel(QObject *parent)
	: QAbstractTableModel(parent)
{	
	this->clear();

	listIcons.insert(NoIcon,new QIcon());

	listIcons.insert(BranchIcon,new QIcon(":/images/link.png"));
	listIcons.insert(TransformerIcon,new QIcon(":/images/transformer.png"));
	listIcons.insert(FactsDeviceIcon,new QIcon(":/images/facts.png"));

	listIcons.insert(BranchSelectedIcon,new QIcon(":/images/linkyes.png"));
	listIcons.insert(TransformerSelectedIcon,new QIcon(":/images/transformeryes.png"));
	listIcons.insert(FactsDeviceSelectedIcon,new QIcon(":/images/factsyes.png"));

	listIcons.insert(BranchUnSelectableIcon,new QIcon(":/images/linkgray.png"));
	listIcons.insert(TransformerUnSelectableIcon,new QIcon(":/images/transformergray.png"));
	listIcons.insert(FactsDeviceUnSelectableIcon,new QIcon(":/images/factsgray.png"));
}
int BusNodeModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
    return listnodes.size();
}
int BusNodeModel::columnCount(const QModelIndex &parent) const
{
	 Q_UNUSED(parent);
	 return StationColumnCnt;
}
QVariant BusNodeModel::data(const QModelIndex &index, int role) const
{
	 if (!index.isValid())
        return QVariant();
    if (index.row() >= listnodes.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        iNodeData *node = listnodes.at(index.row());
		
		switch (index.column()) {
        case ID:						
			return node->Id();
			
        case Name:
            return ((iBUS *)node)->name();
			
        case VB:
            return node->GetRefVoltage();

		case AreaName:
			return listareanames.at(index.row());
			
        default:
            return QVariant();
        }
    }
    else if (role == Qt::UserRole){
        if (index.column() == ID)
		{
			iNodeData *node=listnodes.at(index.row());  
            if(node) return (uint)node;
		}
    }else if (role == Qt::DecorationRole){
        if (index.column() == ID)
		{
			int type=listtypes.at(index.row());
			QIcon *icon=listIcons.value((IconType)type);
			if(icon ) return *icon;
		}      
    }

    return QVariant();
}
bool BusNodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;
	int row = index.row();

	if(role == Qt::DecorationRole)
	{
		if (index.column() == ID)
		{
			listtypes.replace(row,(IconType)value.toUInt());
			emit dataChanged(index, index);
			return true;
		}
	}
	return false;
}
QVariant BusNodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case ID:
            return tr("ID");
        case Name:
            return tr("Name");
        case VB:
            return tr("VB");
        default:
            return QVariant();
        }
    }
    return QVariant();
}
bool BusNodeModel::insertmyrow( int row,iNodeData * node,IconType type,QString areaname)
{
	beginInsertRows(QModelIndex(), row, row);
	
	listnodes.insert(row,node);
	listtypes.insert(row,type);
	listareanames.insert(row,areaname);
	endInsertRows();
	return true;
}
bool BusNodeModel::removeRows(int row, int count, const QModelIndex & parent)
{
	Q_UNUSED(parent);
	beginRemoveRows(QModelIndex(), row, row+count-1);

	for (int i=0; i < count; i++) 
	{
		listnodes.removeAt(row);
		listtypes.removeAt(row);
		listareanames.removeAt(row);
	}

	endRemoveRows();
	return true;
}
void BusNodeModel::toggleicontype(const QModelIndex &index)
{
	if (!index.isValid()) return ;

	IconType type=(IconType)listtypes.at(index.row());

	switch(type)
	{
	case BranchIcon:		
		listtypes.replace(index.row(),BranchSelectedIcon);		
		break;

	case TransformerIcon:
		listtypes.replace(index.row(),TransformerSelectedIcon);			
		break;

	case FactsDeviceIcon:
		listtypes.replace(index.row(),FactsDeviceSelectedIcon);	
		break;

	case BranchSelectedIcon:
		listtypes.replace(index.row(),BranchIcon);			
		break;

	case TransformerSelectedIcon:
		listtypes.replace(index.row(),TransformerIcon);		
		break;

	case FactsDeviceSelectedIcon:
		listtypes.replace(index.row(),FactsDeviceIcon);		
		break;

	default:
		break;

	}
	emit dataChanged(index, index);
}
bool BusNodeModel::isselected(const QModelIndex &index)
{
	if (!index.isValid()) return false;

	IconType type=(IconType)listtypes.at(index.row());

	switch(type)
	{	
	case BranchSelectedIcon:	
	case TransformerSelectedIcon:	
	case FactsDeviceSelectedIcon:		
		return true;

	default:
		return false;
	}
}
QModelIndex  BusNodeModel::getIndex(iNodeData *node)
{
	int nodeindex=listnodes.indexOf(node);

	if(nodeindex==-1) return QModelIndex();

	return index(nodeindex,ID);
}

SortFilterModel::SortFilterModel(QObject *parent)
	:QSortFilterProxyModel(parent)    
{
	m_conditionstring="";
	m_areanamefilter="";
	this->setFilterKeyColumn(ID);
}
bool SortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	if(m_conditionstring=="" && m_areanamefilter=="") return true;

	QRegExp filterReg(m_conditionstring, Qt::CaseInsensitive,QRegExp::Wildcard);
		
	QModelIndex index0 = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent);
	QModelIndex area_index = sourceModel()->index(source_row, AreaName, source_parent);

	QString displaycontent = sourceModel()->data(index0,Qt::DisplayRole).toString();
	QString AreaName = sourceModel()->data(area_index,Qt::DisplayRole).toString();
		
	if(m_areanamefilter=="") return displaycontent.contains(filterReg);
	else if(m_conditionstring=="") return AreaName==m_areanamefilter;
	else
		return (displaycontent.contains(filterReg) && AreaName==m_areanamefilter);	
}

AddDialog::AddDialog(iDoc *idoc,iSTAT * editstation,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);	
	m_changes = 0;
	m_doc=idoc;	
	m_editstation=editstation;
	m_doc->getAvailableNode(hiddennodelist);	

	foreach(iAREA *area,m_doc->getArealist())
		ui.comboBox_areas->addItem(area->name());
	this->setFixedSize(this->size());

	m_font=QFont("Times New Roman", 12, QFont::Bold);
	m_type=STAT_HYDROPLANT;

	hiddenmodel=new BusNodeModel(this);
	addedmodel=new BusNodeModel(this);
	branchnodemodel=new BusNodeModel(this);
	
	if(m_editstation)
	{
		is_edit=true;
		m_font=editstation->itemName()->font();
		ui.lineEdit_name->setText(m_editstation->name());

		m_type=m_editstation->sType();
		ui.comboBox_StatType->setCurrentIndex(m_type);

		//prepare  added model 
		foreach(iNodeData *node,m_editstation->nodeDatas())
		{
			QString name="";
			int areaid=((iBUS *)node)->belongedArea();
			name=m_doc->getAREA(areaid)->name();
			addedmodel->insertmyrow(0,node,NoIcon,name);
		}		
	}else
	{
		is_edit=false;			
		ui.lineEdit_name->setPlaceholderText("Sub-");
	}

	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	foreach(iNodeData *node ,hiddennodelist)
	{
		QString name="";
		int areaid=((iBUS *)node)->belongedArea();
		name=m_doc->getAREA(areaid)->name();
		hiddenmodel->insertmyrow(0,node,NoIcon,name);
	}

	connect(ui.pushButton_add,SIGNAL(clicked()),this,SLOT(OnClickAdd()));
	connect(ui.pushButton_addall,SIGNAL(clicked()),this,SLOT(OnAddAll()));
	connect(ui.pushButton_remove,SIGNAL(clicked()),this,SLOT(OnClickRevoke()));
	connect(ui.pushButton_removeall,SIGNAL(clicked()),this,SLOT(OnRevokeAll()));
	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontdialog()));
	connect(ui.pushButton_branchnodeadd,SIGNAL(clicked()),this,SLOT(OnBranchNodeAdd()));

	connect(ui.comboBox_areas,SIGNAL(currentIndexChanged(int)),this,SLOT(OnComboAreaChanged(int)));
	connect(ui.comboBox_StatType,SIGNAL(currentIndexChanged(int)),this,SLOT(OnStatTypeChanged(int)));
	connect(ui.comboBox_filter,SIGNAL(currentIndexChanged(int)),this,SLOT(OnComboFilterchanged(int)));

	connect(ui.lineEdit_filter,SIGNAL(textChanged(const QString &)),this,SLOT(Onfiltertextchanged(const QString &)));
	connect(ui.lineEdit_name,SIGNAL(textChanged(const QString &)),this,SLOT(OnnameChanged(const QString &)));

	connect(ui.tableView_hidden,SIGNAL(clicked(const QModelIndex &)),this,SLOT(OnHiddenTableActived(const QModelIndex &)));
	connect(ui.tableView_added,SIGNAL(clicked(const QModelIndex &)),this,SLOT(OnAddedTableActived(const QModelIndex &)));
	connect(ui.tableView_branch,SIGNAL(clicked(const QModelIndex &)),this,SLOT(OnBranchnodeActived(const QModelIndex &)));
	connect(ui.tableView_hidden,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(OnAdd(const QModelIndex &)));
	connect(ui.tableView_added,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(OnRevoke(const QModelIndex &)));

	connect(this,SIGNAL(hiddenmodelchanged( )),this,SLOT(Onhiddenmodelchanged( )));
	connect(this,SIGNAL(addedmodelchanged( )),this,SLOT(Onaddedmodelchanged( )));
	
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(OnOk()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	

	ui.lineEdit_hiddenCnt->setText(QString::number(hiddenmodel->rowCount()));
	ui.lineEdit_addedCnt->setText(QString::number(addedmodel->rowCount()));
			
	SetTableStyle(ui.tableView_hidden);
	SetTableStyle(ui.tableView_added);
	SetTableStyle(ui.tableView_branch);

	m_sortfilter=new SortFilterModel(this);
	m_sortfilter->setSourceModel(hiddenmodel);
	m_sortfilter->sort(ID, Qt::AscendingOrder);	
	m_sortfilter->setDynamicSortFilter(true);
	ui.tableView_hidden->setModel(m_sortfilter);
	
	addedmodelproxy=new QSortFilterProxyModel(this);
	addedmodelproxy->setSourceModel(addedmodel);
	addedmodelproxy->sort(ID, Qt::AscendingOrder);	
	addedmodelproxy->setDynamicSortFilter(true);
	ui.tableView_added->setModel(addedmodelproxy);

	branchmodelproxy=new QSortFilterProxyModel(this);
	branchmodelproxy->setSourceModel(branchnodemodel);
	branchmodelproxy->sort(ID, Qt::AscendingOrder);	
	branchmodelproxy->setDynamicSortFilter(true);
	ui.tableView_branch->setModel(branchmodelproxy);

	setColumnWidth(ui.tableView_hidden);
	setColumnWidth(ui.tableView_added);
	setColumnWidth(ui.tableView_branch);	
}
AddDialog::~AddDialog()
{

}
void AddDialog::OnnameChanged(const QString &name)
{
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	m_changes|=CHG_STAT_NAME;
}
void AddDialog::SetTableStyle(QTableView *tableview)
{
	//set table Parameter	
	tableview->setSortingEnabled(true);
	tableview->setAlternatingRowColors(true);
	tableview->verticalHeader()->hide();								//hide vertical header
	tableview->setShowGrid(true);										// show grid
	tableview->setSelectionMode(QAbstractItemView::SingleSelection);
	tableview->setSelectionBehavior(QAbstractItemView::SelectRows);	//select all row
	tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);	//can not be edit
	tableview->setTabKeyNavigation(false);
	tableview->setFocusPolicy(Qt::NoFocus); 
	tableview->setIconSize(QSize(24,24));
	
	//set sorting indicator,model is default ascendingOrder sort by ID .
	tableview->horizontalHeader()->setStretchLastSection(true);	
	tableview->horizontalHeader()->setResizeMode(QHeaderView::Interactive);		//user can resize the column width in UI
	tableview->horizontalHeader()->setClickable(true);							//set header can be click and sorting
	tableview->horizontalHeader()->setSortIndicator(ID, Qt::AscendingOrder);
	tableview->horizontalHeader()->setHighlightSections(false); 

	//tableview->verticalHeader()->setDefaultSectionSize(35);
		
	//set style	 
	tableview->setStyleSheet("selection-background-color:lightblue;"); 
	tableview->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}
void AddDialog::setColumnWidth(QTableView *tableview)
{
	tableview->setColumnHidden(AreaName,true);		
	tableview->setColumnWidth(ID,70);				
	tableview->setColumnWidth(Name,90);	
//	tableview->setColumnWidth(VB,80);	
}
void AddDialog::OnStatTypeChanged(int index)
{
	m_type=(STAT_TYPE)index;
	m_changes |=CHG_STAT_STYPE;
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
void AddDialog::OnComboAreaChanged(int index)
{	
	if(index==0) m_sortfilter->setAreafilter("");
	else m_sortfilter->setAreafilter(ui.comboBox_areas->currentText());

	emit hiddenmodelchanged();
}
void AddDialog::Onhiddenmodelchanged()
{
	ui.lineEdit_hiddenCnt->setText(QString::number(m_sortfilter->rowCount()));
}
void AddDialog::Onaddedmodelchanged()
{
	ui.lineEdit_addedCnt->setText(QString::number(addedmodelproxy->rowCount()));
}
void AddDialog::Onfiltertextchanged(const QString &text)
{
	m_sortfilter->setCondition(text);

	emit hiddenmodelchanged();
}
void AddDialog::OnComboFilterchanged(int index)
{
	m_sortfilter->setFilterKeyColumn(index);
	m_sortfilter->setCondition(ui.lineEdit_filter->text().trimmed());

	emit hiddenmodelchanged();
}

bool AddDialog::IsAddSite()
{
	if(addedmodelproxy->rowCount()>0) return true;
	else return false;
}

//bool AddDialog::IsRemovedSite()
//{
//	if(hiddennodelist.count()>0) return true;
//	else return false;
//}
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
	if(addedmodelproxy->rowCount()==0) 
		return name;
	QString nametxt=ui.lineEdit_name->text().trimmed();
	if(nametxt==NULL) 
	{
		QModelIndex firstindex=addedmodelproxy->index(0,ID);
		if(!firstindex.isValid()) return "";	
		iNodeData *node=(iNodeData *)addedmodelproxy->data(firstindex,Qt::UserRole).toUInt();
		if(node->type()==T_BUS)					//if not new the name ,use the first node's name
		{
			return ((iBUS *)node)->name(true);			
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
void AddDialog::showConnectionNode(iNodeData *selectnode)
{	
	if(!selectnode) return;
	if(selectnode->type()==T_BUS)
	{
		QString connection=QString(tr("Connected Buses: %1")).arg(((iBUS *)selectnode)->name());
		ui.label_branch->setText(connection);

		foreach(iLinkData *link,selectnode->linkDatas())
		{			
			iNodeData* fromnode = m_doc->getNode(link->fromUid());
			iNodeData* tonode = m_doc->getNode(link->toUid());	
			if(!fromnode || !tonode) continue;					
			if((!branchnodemodel->contains(fromnode)) && (selectnode!=fromnode) )
			{				
				switch(link->type())
				{
				case T_BRANCH:
					{				
						branchnodemodel->insertmyrow(0,fromnode,BranchIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(fromnode))					
							branchnodemodel->setData(index, BranchUnSelectableIcon, Qt::DecorationRole);
					}
					break;
				case T_TRANSFORMER:									
					{
						branchnodemodel->insertmyrow(0,fromnode,TransformerIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(fromnode))					
							branchnodemodel->setData(index, TransformerUnSelectableIcon, Qt::DecorationRole);
					}
					break;

				case T_FACTSDEVICE:
					{
						branchnodemodel->insertmyrow(0,fromnode,FactsDeviceIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(fromnode))					
							branchnodemodel->setData(index, FactsDeviceUnSelectableIcon, Qt::DecorationRole);
					}
					break;
				}
			}
			if((!branchnodemodel->contains(tonode)) && (selectnode!=tonode) )
			{				
				switch(link->type())
				{
				case T_BRANCH:
					{				
						branchnodemodel->insertmyrow(0,tonode,BranchIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(tonode))					
							branchnodemodel->setData(index, BranchUnSelectableIcon, Qt::DecorationRole);
					}
					break;
				case T_TRANSFORMER:									
					{
						branchnodemodel->insertmyrow(0,tonode,TransformerIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(tonode))					
							branchnodemodel->setData(index, TransformerUnSelectableIcon, Qt::DecorationRole);
					}
					break;

				case T_FACTSDEVICE:
					{
						branchnodemodel->insertmyrow(0,tonode,FactsDeviceIcon,"");
						QModelIndex index = branchnodemodel->index(0, ID);
						if(!index.isValid())  continue;
						if(!checknodecanbeselected(tonode))					
							branchnodemodel->setData(index, FactsDeviceUnSelectableIcon, Qt::DecorationRole);
					}
					break;
				}
			}
		}		
	}
}
void AddDialog::OnHiddenTableActived(const QModelIndex & index)
{
	ui.pushButton_add->setEnabled(true);
	QModelIndex idindex=m_sortfilter->index(index.row(),ID);
	if(!index.isValid()) return ;

	iNodeData *node=(iNodeData *)m_sortfilter->data(idindex,Qt::UserRole).toUInt();
	branchnodemodel->clear();
	branchmodelproxy->clear();
	showConnectionNode(node);
}
void AddDialog::OnAddedTableActived(const QModelIndex & index)
{	
	ui.pushButton_remove->setEnabled(true);

	QModelIndex idindex=addedmodelproxy->index(index.row(),ID);
	if(!index.isValid()) return ;	
	iNodeData *node=(iNodeData *)addedmodelproxy->data(idindex,Qt::UserRole).toUInt();
	branchnodemodel->clear();
	branchmodelproxy->clear();
	showConnectionNode(node);
}
void AddDialog::OnBranchNodeAdd()
{
	if(branchmodelproxy->rowCount()==0) return ;
	int activerows=0;
	for(int row=0;row<branchmodelproxy->rowCount();row++)
	{
		QModelIndex idindex=branchmodelproxy->index(row,ID);
		QModelIndex areanameindex=branchmodelproxy->index(row,AreaName);
		if(!idindex.isValid() || !areanameindex.isValid()) continue ;
		iNodeData *node=(iNodeData *)branchmodelproxy->data(idindex,Qt::UserRole).toUInt();
		QString areaname=branchmodelproxy->data(areanameindex,Qt::DisplayRole).toString();

		if(branchnodemodel->isselected(branchmodelproxy->mapToSource(idindex)))
		{		
			QModelIndex nodeindex=hiddenmodel->getIndex(node);
			if(!nodeindex.isValid()) continue;
			hiddenmodel->removeRows(nodeindex.row(),1);
			addedmodel->insertmyrow(0,node,NoIcon,areaname);	
			activerows++;
		}
	}

	if(activerows>0)
	{
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		branchnodemodel->clear();
		branchmodelproxy->clear();
		ui.label_branch->setText("Connected Buses ....");
		emit hiddenmodelchanged();
		emit addedmodelchanged();
		m_changes|=CHG_STAT_DATA;
	}


}
void AddDialog::OnAdd(const QModelIndex & index)
{		
	QModelIndex idindex=m_sortfilter->index(index.row(),ID);
	QModelIndex areanameindex=m_sortfilter->index(index.row(),AreaName);
	if(!idindex.isValid() || !areanameindex.isValid()) return ;

	iNodeData *node=(iNodeData *)m_sortfilter->data(idindex,Qt::UserRole).toUInt();
	QString areaname=m_sortfilter->data(areanameindex,Qt::DisplayRole).toString();
		
	if(m_sortfilter->removeRows(index.row(),1))
	{
		if(!addedmodel->insertmyrow(0,node,NoIcon,areaname)) return ;

		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui.tableView_hidden->clearSelection();
		branchnodemodel->clear();
		branchmodelproxy->clear();
		ui.label_branch->setText("Connected Buses ....");

		emit hiddenmodelchanged();
		emit addedmodelchanged();
		m_changes|=CHG_STAT_DATA;
	}
}
void AddDialog::OnClickAdd()
{
	QModelIndex c_index=ui.tableView_hidden->currentIndex();
	if(!c_index.isValid()) return ;

	OnAdd(c_index);
}
void AddDialog::OnAddAll()
{	
	if(m_sortfilter->rowCount()==0) return ;
	
	for(int row=0;row<m_sortfilter->rowCount();row++)
	{
		QModelIndex idindex=m_sortfilter->index(row,ID);
		QModelIndex areanameindex=m_sortfilter->index(row,AreaName);
		if(!idindex.isValid() || !areanameindex.isValid()) continue ;
		iNodeData *node=(iNodeData *)m_sortfilter->data(idindex,Qt::UserRole).toUInt();
		QString areaname=m_sortfilter->data(areanameindex,Qt::DisplayRole).toString();
		addedmodel->insertmyrow(0,node,NoIcon,areaname);	
	}

	m_sortfilter->removeRows(0,m_sortfilter->rowCount());

	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);	
	branchnodemodel->clear();
	branchmodelproxy->clear();
	ui.label_branch->setText("Connected Buses ....");

	emit hiddenmodelchanged();
	emit addedmodelchanged();
	m_changes|=CHG_STAT_DATA;
}
void AddDialog::OnRevoke(const QModelIndex & index)
{	
	QModelIndex idindex=addedmodelproxy->index(index.row(),ID);
	QModelIndex areanameindex=addedmodelproxy->index(index.row(),AreaName);
	if(!idindex.isValid() || !areanameindex.isValid()) return ;

	iNodeData *node=(iNodeData *)addedmodelproxy->data(idindex,Qt::UserRole).toUInt();
	QString areaname=addedmodelproxy->data(areanameindex,Qt::DisplayRole).toString();
	if(addedmodelproxy->removeRows(index.row(),1))
	{
		if(!hiddenmodel->insertmyrow(0,node,NoIcon,areaname)) return ;

		if(addedmodelproxy->rowCount()!=0)
			ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		else
			ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		ui.tableView_added->clearSelection();
		branchnodemodel->clear();
		branchmodelproxy->clear();
		ui.label_branch->setText("Connected Buses ....");

		emit hiddenmodelchanged();
		emit addedmodelchanged();
		m_changes|=CHG_STAT_DATA;	
	}
}
void AddDialog::OnClickRevoke()
{
	QModelIndex c_index=ui.tableView_added->currentIndex();
	if(!c_index.isValid()) return ;

	OnRevoke(c_index);
}
void AddDialog::OnRevokeAll()
{	
	if(addedmodelproxy->rowCount()==0) return ;
	
	for(int row=0;row<addedmodelproxy->rowCount();row++)
	{
		QModelIndex idindex=addedmodelproxy->index(row,ID);
		QModelIndex areanameindex=addedmodelproxy->index(row,AreaName);
		if(!idindex.isValid() || !areanameindex.isValid()) continue ;
		iNodeData *node=(iNodeData *)addedmodelproxy->data(idindex,Qt::UserRole).toUInt();
		QString areaname=addedmodelproxy->data(areanameindex,Qt::DisplayRole).toString();
		hiddenmodel->insertmyrow(0,node,NoIcon,areaname);	
	}

	addedmodelproxy->removeRows(0,addedmodelproxy->rowCount());

	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);	
	branchnodemodel->clear();
	branchmodelproxy->clear();
	ui.label_branch->setText("Connected Buses ....");

	emit hiddenmodelchanged();
	emit addedmodelchanged();
	m_changes|=CHG_STAT_DATA;
}
void AddDialog::OnOk()
{
	if(m_editstation)
	{
		m_editstation->setName(NewStationName());
		if(m_changes & CHG_STAT_DATA)
			m_editstation->setNodes(addedmodel->nodeslist());
	}

	accept();
}
bool AddDialog::checknodecanbeselected(iNodeData *node)
{	
	return (hiddenmodel->contains(node) && (!addedmodel->contains(node)));
}
void AddDialog::OnBranchnodeActived(const QModelIndex & index)
{		
	QModelIndex idindex=branchmodelproxy->index(index.row(),ID);
	branchnodemodel->toggleicontype(branchmodelproxy->mapToSource(idindex));	
}