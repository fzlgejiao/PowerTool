#ifndef STATIONNAMEDIALOG_H
#define STATIONNAMEDIALOG_H

#include <QDialog>
#include "ui_stationnamedialog.h"
#include <QFont>

class iSTAT;
class DiagramTextItem;
class StationNameDialog : public QDialog
{
	Q_OBJECT

public:
	StationNameDialog(iSTAT *stat,QWidget *parent = 0);
	~StationNameDialog();
	QString StationName();
	QFont GetFont() {return m_font;}
	bool IsApplyAll(){return m_IsApplyAll;}

private slots:
		void OnclearshownNodes();
		void OnFontchanged();
		void OnCheckBoxChanged(int state);
		void OnshowNodeChanged(const QModelIndex& index);
		void OnOk();
private:
	Ui::StationNameDialog ui;
	iSTAT *m_stat;
	DiagramTextItem *m_textitem;
	QFont m_font;
	bool m_IsApplyAll;
};

#endif // STATIONNAMEDIALOG_H
