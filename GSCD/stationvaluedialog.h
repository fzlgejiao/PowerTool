#ifndef STATIONVALUEDIALOG_H
#define STATIONVALUEDIALOG_H

#include <QDialog>
#include "ui_stationvaluedialog.h"


class iSTAT;
class DiagramTextItem;

enum ValueShowType
{
	NotShow=0,
	Total,
	List
};
class StationValueDialog : public QDialog
{
	Q_OBJECT

public:
	StationValueDialog(iSTAT *stat,QWidget *parent = 0);
	~StationValueDialog();
	bool IsShowCompensation(){return m_showcompensation;}
	ValueShowType GetPowerShowType(){return m_powershowtype;}
	ValueShowType GetLoaderShowType(){return m_loadershowtype;}

private:
	Ui::StationValueDialog ui;
	iSTAT *m_stat;
	DiagramTextItem *m_textitem;
	bool m_showcompensation;
	ValueShowType m_powershowtype;
	ValueShowType m_loadershowtype;

};

#endif // STATIONVALUEDIALOG_H
