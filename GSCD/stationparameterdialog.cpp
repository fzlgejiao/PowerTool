#include "stationparameterdialog.h"

StationParameterDialog::StationParameterDialog(iSTAT *station,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_station=station;
}

StationParameterDialog::~StationParameterDialog()
{

}

void StationParameterDialog::SetTableStyle(QTableWidget *tablewidget)
{
	//set table Parameter
	tablewidget->horizontalHeader()->setStretchLastSection(true);	
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
	tablewidget->setColumnWidth(Name,50);
	tablewidget->setColumnWidth(Ref_Volatge,50);
	tablewidget->setColumnWidth(Voltage,50);
	tablewidget->setColumnWidth(Angle,50);
	tablewidget->setColumnWidth(Energy,50);
	tablewidget->setColumnWidth(Load,50);
	tablewidget->setColumnWidth(Compensation,50);
	
	//set style	 
	tablewidget->setStyleSheet("selection-background-color:lightblue;"); 
	tablewidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 
}
