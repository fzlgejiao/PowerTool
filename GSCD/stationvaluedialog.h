#ifndef STATIONVALUEDIALOG_H
#define STATIONVALUEDIALOG_H

#include <QDialog>
#include "ui_stationvaluedialog.h"


class iSTAT;
class DiagramTextItem;


class StationValueDialog : public QDialog
{
	Q_OBJECT

public:
	StationValueDialog(iSTAT *stat,QWidget *parent = 0);
	~StationValueDialog();
	bool IsShowCompensation(){return m_showcompensation;}
	
protected:
   virtual void changeEvent(QEvent *e);

private slots:
		void OngroupLoadtoggled(bool toggle);
		void OnpowerTypeChanged();
		void OnloadTypeChanged();
		void OncompensationChanged(bool toggle);

private:
	Ui::StationValueDialog ui;
	iSTAT *m_stat;
	DiagramTextItem *m_textitem;
	bool m_showcompensation;
};

#endif // STATIONVALUEDIALOG_H
