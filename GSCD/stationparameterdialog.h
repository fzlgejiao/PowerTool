#ifndef STATIONPARAMETERDIALOG_H
#define STATIONPARAMETERDIALOG_H

#include <QDialog>
#include "ui_stationparameterdialog.h"


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
	StationParameterDialog(iSTAT *station,QWidget *parent = 0);
	~StationParameterDialog();
	QString GetStationName(){return ui.lineEdit_name->text().trimmed();}

private  slots:
	void OnShowVoltage();
	void OnPowerFlow();

private:
	Ui::StationParameterDialog ui;
	iSTAT *m_station;
	void SetTableStyle(QTableWidget *tablewidget);	
	void AddNodeToTable(iNodeData *node);
};

#endif // STATIONPARAMETERDIALOG_H
