#ifndef CONTROLPANELDIALOG_H
#define CONTROLPANELDIALOG_H

#include <QDialog>
#include "ui_controlpaneldialog.h"
#include "diagramscene.h"

class ControlPanelDialog : public QDialog
{
	Q_OBJECT

public:
	ControlPanelDialog(ControlPanel controlpanel,QWidget *parent = 0);
	~ControlPanelDialog();
	ControlPanel getControlPanel(){return m_panel;}

private slots:
		void Onshowtypechanged();
		void Onunitchanged();
		void Onshowstationnametoggled(bool toggle);
		void Onshowstationvaluetoggled(bool toggle);
		void Onshowbranchlinetoggled(bool toggle);
		void Onshowbranchvaluetoggled(bool toggle);
		void Onshowreactivepowertoggled(bool toggle);
		void Onshowadmittancetoggled(bool toggle);
		void Onshowvoltageangletoggled(bool toggle);
		void Onshowallvoltagetoggled(bool toggle);

private:
	Ui::ControlPanelDialog ui;
	ControlPanel m_panel;
};

#endif // CONTROLPANELDIALOG_H
