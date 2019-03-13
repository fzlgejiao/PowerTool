#ifndef CONTROLPANELDIALOG_H
#define CONTROLPANELDIALOG_H

#include <QDialog>
#include "ui_controlpaneldialog.h"
#include "diagramscene.h"

#define	CHG_CONTROLPANEL_SHOWTYPE			0x01
#define	CHG_CONTROLPANEL_SHOWSTATNAME		0x02
#define	CHG_CONTROLPANEL_SHOWSTATVALUE		0x04
#define	CHG_CONTROLPANEL_SHOWBRNACHLINE		0x08
#define	CHG_CONTROLPANEL_SHOWBRNACHVALUE	0x10
#define	CHG_CONTROLPANEL_SHOWREACTIVEVALUE	0x20
#define	CHG_CONTROLPANEL_SHOWADMITTANCE		0x40
#define	CHG_CONTROLPANEL_SHOWANGLE			0x80
#define	CHG_CONTROLPANEL_SHOWALLVOLTAGE		0x100
#define	CHG_CONTROLPANEL_UNIT				0x200

class ControlPanelDialog : public QDialog
{
	Q_OBJECT

public:
	ControlPanelDialog(ControlPanel &controlpanel,QWidget *parent = 0);
	~ControlPanelDialog();
	ControlPanel getControlPanel(){return m_panel;}
	uint getchanges(){return m_changes;}

protected:
   virtual void changeEvent(QEvent *e);

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
	uint  m_changes;
};

#endif // CONTROLPANELDIALOG_H
