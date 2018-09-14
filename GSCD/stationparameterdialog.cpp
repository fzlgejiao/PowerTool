#include "stationparameterdialog.h"
#include "idata.h"
#include "stationnamedialog.h"

StationParameterDialog::StationParameterDialog(iSTAT *station,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	m_station=station;	
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
	
}
void StationParameterDialog::OnPowerFlow()
{

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
	tablewidget->setColumnWidth(Voltage,80);
	tablewidget->setColumnWidth(Angle,50);
	tablewidget->setColumnWidth(Energy,120);
	tablewidget->setColumnWidth(Load,120);
	tablewidget->setColumnWidth(Compensation,120);
	
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

	if(node->type()==T_BUS) 
	{
		iBUS *bus=(iBUS *)node;
		item->setText(bus->name());
		refvoltageitem->setText( QString::number(bus->GetRefVoltage(),10,1));
		voltageitem->setText( QString::number(bus->GetVoltage(),10,3));
		angleitem->setText(QString::number(bus->GetAngle(),10,1));
		ui.tableWidget_parameter->setItem(row, NAME, item);	
		ui.tableWidget_parameter->setItem(row, Ref_Volatge, refvoltageitem);
		ui.tableWidget_parameter->setItem(row, Voltage, voltageitem);
		ui.tableWidget_parameter->setItem(row, Angle, angleitem);
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