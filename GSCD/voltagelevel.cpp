#include "voltagelevel.h"
#include "editvoltagelevel.h"
#include <QMessageBox>
#include <QColorDialog>

VoltageLevel::VoltageLevel(QWidget *parent)
	: QDialog(parent)
	,m_Global(iGlobal::Instance())
	,m_changes(0)
	,m_iscolormap(false)
	,m_iswarning(false)
{
	ui.setupUi(this);	
	this->setFixedSize(this->size());

	m_Global.readGlobalXmlfile();
	m_levels=m_Global.getVoltagelevels();
	m_dccolor=m_Global.dccolor();
	m_defaultcolor=m_Global.defaultcolor();
	m_dcwidth=m_Global.dcwidth();
	
	connect(this,SIGNAL(globalChanged(ushort)),&m_Global,SLOT(Onchanges(ushort)));

	ui.checkBox_colormap->setChecked(m_Global.iscolormap());
	ui.checkBox_warning->setChecked(m_Global.iswarning());

	ui.pushButton_dccolor->setStyleSheet(QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_dccolor.red()).arg(m_dccolor.green()).arg(m_dccolor.blue()));
	ui.pushButton_defaultcolor->setStyleSheet(QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_defaultcolor.red()).arg(m_defaultcolor.green()).arg(m_defaultcolor.blue()));
	ui.comboBox_DClinewidth->setCurrentIndex(m_dcwidth);

	ui.tableWidget->setColumnWidth(RefVoltage,70);
	ui.tableWidget->setColumnWidth(Width,40);
	ui.tableWidget->setColumnWidth(Color,90);
	
	connect(ui.tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(Onvoltage_edit()));

	connect(ui.pushButton_edit,SIGNAL(clicked()),this,SLOT(Onvoltage_edit()));
	connect(ui.pushButton_add,SIGNAL(clicked()),this,SLOT(Onvoltage_new()));
	connect(ui.pushButton_remove,SIGNAL(clicked()),this,SLOT(Onvoltage_delete()));
	connect(ui.pushButton_dccolor,SIGNAL(clicked()),this,SLOT(OnDCColorChanged()));
	connect(ui.pushButton_defaultcolor,SIGNAL(clicked()),this,SLOT(OnDefaultColorChanged()));
	connect(ui.comboBox_DClinewidth,SIGNAL(currentIndexChanged(int)),this,SLOT(OnDcWidthChanged(int)));

	connect(ui.checkBox_colormap,SIGNAL(toggled(bool)),this,SLOT(Oncolormap(bool)));
	connect(ui.checkBox_warning,SIGNAL(toggled(bool)),this,SLOT(Onwarning(bool)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(OnSave()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	
	addLevels2Table();
	ui.tableWidget->sortByColumn(RefVoltage,Qt::DescendingOrder);			//default to voltage descending
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

VoltageLevel::~VoltageLevel()
{

}
void VoltageLevel::Oncolormap(bool check)
{
	m_iscolormap=check;
	m_Global.setcolormap(m_iscolormap);
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
	m_changes|=CHG_COLORMAP;
}
void VoltageLevel::Onwarning(bool check)
{
	m_iswarning=check;
	m_Global.setwarning(m_iswarning);
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
	m_changes|=CHG_WARNING;
}
void VoltageLevel::OnDCColorChanged()
{
	QColor color=QColorDialog::getColor(m_dccolor, this);
	if(!color.isValid()) return ;
	m_dccolor= color;
 
	QString strcolor=QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_dccolor.red()).arg(m_dccolor.green()).arg(m_dccolor.blue());
	ui.pushButton_dccolor->setStyleSheet(strcolor);	

	m_Global.setdccolor(m_dccolor);
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
	m_changes|=CHG_DCCOLOR;
}
void VoltageLevel::OnDefaultColorChanged()
{
	QColor color=QColorDialog::getColor(m_defaultcolor, this);
	if(!color.isValid()) return ;
	m_defaultcolor= color;
 
	QString strcolor=QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_defaultcolor.red()).arg(m_defaultcolor.green()).arg(m_defaultcolor.blue());
	ui.pushButton_defaultcolor->setStyleSheet(strcolor);	

	m_Global.setdefaultcolor(m_defaultcolor);
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
	m_changes|=CHG_DEFAULTCOLOR;
}
void VoltageLevel::OnDcWidthChanged(int index)
{
	m_dcwidth=index;
	m_Global.setdcwidth(m_dcwidth);
	ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
	m_changes|=CHG_DCWIDTH;
}
void VoltageLevel::Onvoltage_edit()
{		
	QList<QTableWidgetItem*> items=ui.tableWidget->selectedItems();
	if(items.count()==0) return ;

	QTableWidgetItem *voltageitem=items.at(RefVoltage);	
	QTableWidgetItem *widthitem=items.at(Width);
	QWidget *label= ui.tableWidget->cellWidget(ui.tableWidget->row(voltageitem),Color);

	CVoltageLevel *level=(CVoltageLevel *)voltageitem->data(Qt::UserRole).toUInt();
	if(!level) return ;
	EditVoltageLevel e_dlg(level);
	if(e_dlg.exec()==QDialog::Accepted)
	{
		float new_voltage=e_dlg.voltage();
		QColor new_color=e_dlg.color();
		int new_width=e_dlg.width();

		if(new_voltage==level->refVoltage() && new_width==level->getwidth()) {
				level->setcolor(new_color);
				label->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(new_color.red()).arg(new_color.green()).arg(new_color.blue()));
				ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
				m_changes|=CHG_LEVELS;
				return ;
		}
		if(m_Global.editvoltagelevel(level,new_voltage,new_width,new_color))
		{
			voltageitem->setData(Qt::DisplayRole,(new_voltage==0)?"Ground":QString("%1kV").arg(new_voltage));
			widthitem->setData(Qt::DisplayRole,level->getwidth());
			label->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(new_color.red()).arg(new_color.green()).arg(new_color.blue()));
			if(m_Global.errors()==WidthError)
			{
				QMessageBox::warning(this, tr("GSCD"),tr("The width of low voltage can't more than large one!\n Auto change it!"),QMessageBox::Ok);
			}
			ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
			m_changes|=CHG_LEVELS;
		}else
		{
			//edit errors
			if(m_Global.errors()==VoltageError)
			{
				QMessageBox::warning(this, tr("GSCD"),tr("This voltage is exist,please change a different!"),QMessageBox::Ok);
			} 
		}
	}
}
void VoltageLevel::Onvoltage_new()
{
	EditVoltageLevel e_dlg(NULL,this);
	if(e_dlg.exec()==QDialog::Accepted)
	{
		float new_voltage=e_dlg.voltage();
		QColor newcolor=e_dlg.color();		
		int width=e_dlg.width();

		CVoltageLevel *level=m_Global.newvoltagelevel(new_voltage,width,newcolor);
		if(!level)
		{
			QMessageBox::warning(this, tr("GSCD"),tr("This voltage is exist,please change a different!"),QMessageBox::Ok);
		}		
		else
		{
			if(m_Global.errors()==WidthError)
			{
				QMessageBox::warning(this, tr("GSCD"),tr("The width of low voltage can't more than large one!\n Auto change it!"),QMessageBox::Ok);
			}
			addLevel2Table(level);
			ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
			m_changes|=CHG_LEVELS;
		}
	}
}
void VoltageLevel::Onvoltage_delete()
{
	QList<QTableWidgetItem*> items=ui.tableWidget->selectedItems();
	if(items.count()==0) return;

	QTableWidgetItem *voltageitem=items.at(RefVoltage);	
	CVoltageLevel *level=(CVoltageLevel *)voltageitem->data(Qt::UserRole).toUInt();	
	if(level)
	{
		int selectrow=ui.tableWidget->row(voltageitem);
		ui.tableWidget->removeRow(selectrow);
		m_Global.deletevoltagelevel(level);
		ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
		m_changes|=CHG_LEVELS;
	}
	ui.tableWidget->clearSelection();
}
void VoltageLevel::OnSave()
{
	if(m_Global.saveGlobal2Xml())
	{
		ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);

		if(m_changes!=0) 
			emit globalChanged(m_changes);
	}
}

void VoltageLevel::addLevels2Table()
{
	int rows=m_levels.count();
	if(rows==0) return ;
	ui.tableWidget->setRowCount(rows);
	int index=0;
	foreach(CVoltageLevel *level,m_levels)	
	{		
		if(!level) continue;

		float voltage=level->refVoltage();
		QColor levelcolor= level->getcolor();

		//QTableWidgetItem *item0 = new QTableWidgetItem();	
		VoltageItem *item0 = new VoltageItem();	
		QTableWidgetItem *item1 = new QTableWidgetItem();		
		item0->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		
		QLabel *label=new QLabel();
		label->setAlignment(Qt::AlignCenter);
		label->setFixedSize(75,20);
		label->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(levelcolor.red()).arg(levelcolor.green()).arg(levelcolor.blue()));
		

		item0->setData(Qt::UserRole,(uint)level);		

		item0->setData(Qt::DisplayRole,(voltage==0)?"Ground":QString("%1kV").arg(voltage));	
		item1->setData(Qt::DisplayRole,level->getwidth());

		ui.tableWidget->setRowHeight(index,20);

		ui.tableWidget->setItem(index, RefVoltage, item0);		
		ui.tableWidget->setItem(index, Width, item1);			
		ui.tableWidget->setCellWidget(index,Color,label);
		index++;
	}
}

void VoltageLevel::addLevel2Table(CVoltageLevel * level)
{
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);

	ui.tableWidget->setRowHeight(row,20);
	float voltage=level->refVoltage();
	QColor levelcolor= level->getcolor();

	//QTableWidgetItem *item0 = new QTableWidgetItem();	
	VoltageItem *item0 = new VoltageItem();	
	QTableWidgetItem *item1 = new QTableWidgetItem();	
	item0->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
	item1->setTextAlignment(Qt::AlignCenter);

	QLabel *widget=new QLabel();
	widget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	widget->setFixedSize(75,20);
	widget->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(levelcolor.red()).arg(levelcolor.green()).arg(levelcolor.blue()));

	
	item0->setData(Qt::UserRole,(uint)level);
	item0->setData(Qt::DisplayRole,(voltage==0)?"Ground":QString("%1kV").arg(voltage));	
	item1->setData(Qt::DisplayRole,level->getwidth());

	ui.tableWidget->setItem(row, RefVoltage, item0);	
	int voltage_row=ui.tableWidget->row(item0);
	ui.tableWidget->setItem(voltage_row, Width, item1);	
	int width_row=ui.tableWidget->row(item1);	
	ui.tableWidget->setCellWidget(width_row,Color,widget);
}
