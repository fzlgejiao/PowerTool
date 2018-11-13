#ifndef STATIONPARAMETERDIALOG_H
#define STATIONPARAMETERDIALOG_H

#include <QDialog>
#include "ui_stationparameterdialog.h"
#include "idata.h"

class iData;
class iNodeData;
class iSTAT;

enum StationParameterTableColumn{
	NAME			=0,
	Ref_Volatge,
	Voltage,
	Angle,
	Energy,
	Load,
	Compensation,
	ParameterColumnCnt
};

class StationParameterDialog : public QDialog
{
	Q_OBJECT

public:
	StationParameterDialog(iSTAT *station,const ControlPanel &controlpanel,QWidget *parent = 0);
	~StationParameterDialog();
	QString GetStationName(){return ui.lineEdit_name->text().trimmed();}

private  slots:
	void OnShowVoltage();
	void OnPowerFlow();
	void OnOk();

private:
	Ui::StationParameterDialog ui;	
	iSTAT *m_station;
	void SetTableStyle(QTableWidget *tablewidget);	
	void AddNodeToTable(iNodeData *node);
	
	ControlPanel m_panel;
	double m_Sbase;
};

#endif // STATIONPARAMETERDIALOG_H
