#ifndef STATIONPARAMETERDIALOG_H
#define STATIONPARAMETERDIALOG_H

#include <QDialog>
#include "ui_stationparameterdialog.h"


class iSTAT;

enum StationParameterTableColumn{
	Name			=0,
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

private:
	Ui::StationParameterDialog ui;
	iSTAT *m_station;
	void SetTableStyle(QTableWidget *tablewidget);	
};

#endif // STATIONPARAMETERDIALOG_H
