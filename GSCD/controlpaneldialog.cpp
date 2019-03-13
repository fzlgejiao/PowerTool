#include "controlpaneldialog.h"
#include "diagramscene.h"

ControlPanelDialog::ControlPanelDialog(ControlPanel &controlpanel,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	m_panel=controlpanel;
	m_changes=0;

	switch(m_panel.showtype)
	{
	case SHOW_ONLYNAME:
		ui.radioButton_nonote->setChecked(true);
		break;

	case SHOW_POWERFLOW:
		ui.radioButton_powerflow->setChecked(true);
		break;

	case SHOW_RESISTANCE:
		ui.radioButton_resistance->setChecked(true);
		break;
	}

	switch(m_panel.unittype)
	{
	case UNIT_PERUNIT:
		ui.radioButton_unit2->setChecked(true);
		break;

	case UNIT_ACTUALVALUE:
		ui.radioButton_unit1->setChecked(true);
		break;
	}
	ui.checkBox_stationname->setChecked(m_panel.isShowStationName);
	ui.checkBox_stationvalue->setChecked(m_panel.isShowStationValue);
	ui.checkBox_branchline->setChecked(m_panel.isShowBranchLine);
	ui.checkBox_branchvalue->setChecked(m_panel.isShowBranchValue);
	ui.checkBox_reactivepowerflow->setChecked(m_panel.isShowReactivePowerValue);
	ui.checkBox_admittance->setChecked(m_panel.isShowAdmittance);
	ui.checkBox_angle->setChecked(m_panel.isShowVoltageAngle);
	ui.checkBox_custom->setChecked(m_panel.isShowAllNodeVoltage);

	connect(ui.radioButton_nonote,SIGNAL(clicked()),this,SLOT(Onshowtypechanged()));
	connect(ui.radioButton_powerflow,SIGNAL(clicked()),this,SLOT(Onshowtypechanged()));
	connect(ui.radioButton_resistance,SIGNAL(clicked()),this,SLOT(Onshowtypechanged()));

	connect(ui.radioButton_unit1,SIGNAL(clicked()),this,SLOT(Onunitchanged()));
	connect(ui.radioButton_unit2,SIGNAL(clicked()),this,SLOT(Onunitchanged()));

	connect(ui.checkBox_stationname,SIGNAL(toggled(bool)),this,SLOT(Onshowstationnametoggled(bool)));
	connect(ui.checkBox_stationvalue,SIGNAL(toggled(bool)),this,SLOT(Onshowstationvaluetoggled(bool)));
	connect(ui.checkBox_branchline,SIGNAL(toggled(bool)),this,SLOT(Onshowbranchlinetoggled(bool)));
	connect(ui.checkBox_branchvalue,SIGNAL(toggled(bool)),this,SLOT(Onshowbranchvaluetoggled(bool)));
	connect(ui.checkBox_reactivepowerflow,SIGNAL(toggled(bool)),this,SLOT(Onshowreactivepowertoggled(bool)));
	connect(ui.checkBox_admittance,SIGNAL(toggled(bool)),this,SLOT(Onshowadmittancetoggled(bool)));
	connect(ui.checkBox_angle,SIGNAL(toggled(bool)),this,SLOT(Onshowvoltageangletoggled(bool)));
	connect(ui.checkBox_custom,SIGNAL(toggled(bool)),this,SLOT(Onshowallvoltagetoggled(bool)));
	
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}

ControlPanelDialog::~ControlPanelDialog()
{

}
void ControlPanelDialog::Onshowtypechanged()
{
	if(ui.radioButton_nonote->isChecked()) 
		m_panel.showtype=SHOW_ONLYNAME;
	else if(ui.radioButton_powerflow->isChecked())
		m_panel.showtype=SHOW_POWERFLOW;
	else if(ui.radioButton_resistance->isChecked())
		m_panel.showtype=SHOW_RESISTANCE;
	else m_panel.showtype=SHOW_NONE;

	m_changes|=CHG_CONTROLPANEL_SHOWTYPE;
}
void ControlPanelDialog::Onunitchanged()
{
	if(ui.radioButton_unit1->isChecked()) 
		m_panel.unittype=UNIT_ACTUALVALUE;
	else if(ui.radioButton_unit2->isChecked())
		m_panel.unittype=UNIT_PERUNIT;
	else m_panel.unittype=UNIT_NONE;

	m_changes|=CHG_CONTROLPANEL_UNIT;
}
void ControlPanelDialog::Onshowstationnametoggled(bool toggle)
{
	m_panel.isShowStationName=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWSTATNAME;
}
void ControlPanelDialog::Onshowstationvaluetoggled(bool toggle)
{
	m_panel.isShowStationValue=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWSTATVALUE;
}
void ControlPanelDialog::Onshowbranchlinetoggled(bool toggle)
{
	m_panel.isShowBranchLine=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWBRNACHLINE;
}
void ControlPanelDialog::Onshowbranchvaluetoggled(bool toggle)
{
	m_panel.isShowBranchValue=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWBRNACHVALUE;
}
void ControlPanelDialog::Onshowreactivepowertoggled(bool toggle)
{
	m_panel.isShowReactivePowerValue=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWREACTIVEVALUE;
}
void ControlPanelDialog::Onshowadmittancetoggled(bool toggle)
{
	m_panel.isShowAdmittance=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWADMITTANCE;
}
void ControlPanelDialog::Onshowvoltageangletoggled(bool toggle)
{
	m_panel.isShowVoltageAngle=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWANGLE;
}
void ControlPanelDialog::Onshowallvoltagetoggled(bool toggle)
{
	m_panel.isShowAllNodeVoltage=toggle;
	m_changes|=CHG_CONTROLPANEL_SHOWALLVOLTAGE;
}
void ControlPanelDialog::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);	
	}
	else
	{
		QWidget::changeEvent(e);
	}
}

