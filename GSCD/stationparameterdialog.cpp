#include "stationparameterdialog.h"
#include "stationnamedialog.h"
#include "stationvaluedialog.h"
#include "diagramtextitem.h"

StationParameterDialog::StationParameterDialog(iSTAT *station,const ControlPanel &controlpanel,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	m_station=station;	
	m_panel=controlpanel;
	m_Sbase=m_station->nodeDatas().first()->getSBase();
	SetTableStyle(ui.tableWidget_parameter);

	ui.lineEdit_name->setText(m_station->name());

	foreach(iNodeData *node,m_station->nodeDatas())
	{
		AddNodeToTable(node);
	}			
	connect(ui.pushButton_OK,SIGNAL(clicked()),this,SLOT(OnOk()));	
	connect(ui.pushButton_tideway,SIGNAL(clicked()),this,SLOT(OnPowerFlow()));
	connect(ui.pushButton_voltage,SIGNAL(clicked()),this,SLOT(OnShowVoltage()));
}

StationParameterDialog::~StationParameterDialog()
{

}
void StationParameterDialog::OnShowVoltage()
{
	StationNameDialog dlg(m_station,this);
	if(dlg.exec()==QDialog::Accepted)
	{		
		//Update the voltage and name text		
		m_station->itemName()->setFont(dlg.GetFont());
		if(m_panel.isShowAllNodeVoltage)
		{
			m_station->itemName()->setPlainText(m_station->allNodeVoltage(m_panel.isShowVoltageAngle,m_panel.unittype) + m_station->name());
		}else
		m_station->itemName()->setPlainText(m_station->nodeVoltage(m_panel.isShowVoltageAngle,m_panel.unittype) + m_station->name());				
	}
}
void StationParameterDialog::OnPowerFlow()
{
	StationValueDialog dlg(m_station,this);
	if(dlg.exec()==QDialog::Accepted)
	{			
		m_station->itemValue()->setPlainText(m_station->value(m_Sbase,m_panel.unittype,m_panel.isShowReactivePowerValue));
	}
}
void StationParameterDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	tablewidget->horizontalHeader()->setStretchLastSection(true);	
	//tablewidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tablewidget->setColumnCount(ParameterColumnCnt);					//Set columns Count
	tablewidget->horizontalHeader()->setClickable(true);				//set header can be click and sorting
	tablewidget->setSortingEnabled(true);
	tablewidget->setAlternatingRowColors(true);
	tablewidget->verticalHeader()->hide();								//hide vertical header
	tablewidget->setShowGrid(true);										// show grid
	tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);	//select all row
	tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//can bot be edit
	//set column width	
	tablewidget->setColumnWidth(NAME,100);
	tablewidget->setColumnWidth(Ref_Volatge,80);
	tablewidget->setColumnWidth(Voltage,65);
	tablewidget->setColumnWidth(Angle,50);
	tablewidget->setColumnWidth(Energy,105);
	tablewidget->setColumnWidth(Load,105);
	tablewidget->setColumnWidth(Compensation,105);
	
	//set style	 
	tablewidget->setStyleSheet("selection-background-color:lightblue;"); 
	tablewidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}

void StationParameterDialog::AddNodeToTable(iNodeData *node)
{	
	int row = ui.tableWidget_parameter->rowCount();
	ui.tableWidget_parameter->insertRow(row);

	QTableWidgetItem *item = new QTableWidgetItem();
	QTableWidgetItem *refvoltageitem = new QTableWidgetItem();
	QTableWidgetItem *voltageitem = new QTableWidgetItem();
	QTableWidgetItem *angleitem = new QTableWidgetItem();
	QTableWidgetItem *poweritem = new QTableWidgetItem();
	QTableWidgetItem *loaditem = new QTableWidgetItem();
	QTableWidgetItem *compensationitem = new QTableWidgetItem();
	if(node->type()==T_BUS) 
	{
		iBUS *bus=(iBUS *)node;
		item->setText(bus->name());
		refvoltageitem->setText( QString::number(bus->GetRefVoltage(),10,1));
		voltageitem->setText( QString::number(bus->GetVoltage(),10,3));
		angleitem->setText(QString::number(bus->GetAngle(),10,1));

		if(bus->getPower()!=NULL)
			poweritem->setText(bus->getPower());
		if(bus->getLoad()!=NULL)
			loaditem->setText(bus->getLoad());
		if(bus->getCompensation()!=NULL)
			compensationitem->setText(bus->getCompensation());

		ui.tableWidget_parameter->setItem(row, NAME, item);	
		ui.tableWidget_parameter->setItem(row, Ref_Volatge, refvoltageitem);
		ui.tableWidget_parameter->setItem(row, Voltage, voltageitem);
		ui.tableWidget_parameter->setItem(row, Angle, angleitem);
		ui.tableWidget_parameter->setItem(row, Energy, poweritem);
		ui.tableWidget_parameter->setItem(row, Load, loaditem);
		ui.tableWidget_parameter->setItem(row, Compensation, compensationitem);
	}
}
void StationParameterDialog::OnOk()
{
	if(m_station)
	{
		m_station->setName(ui.lineEdit_name->text().trimmed());
	}
	accept();
}