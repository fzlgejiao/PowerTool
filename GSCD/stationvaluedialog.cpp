#include "stationvaluedialog.h"
#include "diagramtextitem.h"
#include "idata.h"

StationValueDialog::StationValueDialog(iSTAT *stat,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	
	m_stat=stat;
	m_textitem=m_stat->itemValue();

	this->setWindowTitle(m_stat->name());

	if(m_stat->powerShowtype()==VALUE_ADDUP)
		ui.radioButton_powertotal->setChecked(true);
	else
		ui.radioButton_powerlist->setChecked(true);

	if(m_stat->loadShowtype()==VALUE_ADDUP)
		ui.radioButton_loadertotal->setChecked(true);
	else
		ui.radioButton_loaderlist->setChecked(true);

	
	ui.radioButton_loadertotal->setEnabled(m_stat->isshowload());
	ui.radioButton_loaderlist->setEnabled(m_stat->isshowload());
		
	ui.checkBox_compensation->setChecked(m_stat->isshowcompensation());
	ui.groupBox_load->setChecked(m_stat->isshowload());

	connect(ui.groupBox_load,SIGNAL(toggled(bool)),this,SLOT(OngroupLoadtoggled(bool)));
	connect(ui.radioButton_powertotal,SIGNAL(clicked()),this,SLOT(OnpowerTypeChanged()));
	connect(ui.radioButton_powerlist,SIGNAL(clicked()),this,SLOT(OnpowerTypeChanged()));

	connect(ui.radioButton_loadertotal,SIGNAL(clicked()),this,SLOT(OnloadTypeChanged()));
	connect(ui.radioButton_loaderlist,SIGNAL(clicked()),this,SLOT(OnloadTypeChanged()));

	connect(ui.checkBox_compensation,SIGNAL(toggled(bool)),SLOT(OncompensationChanged(bool)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}

StationValueDialog::~StationValueDialog()
{

}

void StationValueDialog::OngroupLoadtoggled(bool toggle)
{
	m_stat->setloadshown(toggle);
	ui.radioButton_loadertotal->setEnabled(toggle);
	ui.radioButton_loaderlist->setEnabled(toggle);
}

void StationValueDialog::OncompensationChanged(bool toggle)
{
	m_stat->setcompensationshown(toggle);
}

void StationValueDialog::OnpowerTypeChanged()
{
	if(ui.radioButton_powerlist->isChecked())
		m_stat->setPowerType(VALUE_LIST);
	else 
		m_stat->setPowerType(VALUE_ADDUP);
}
void StationValueDialog::OnloadTypeChanged()
{
	if(ui.radioButton_loaderlist->isChecked())
		m_stat->setLoadType(VALUE_LIST);
	else 
		m_stat->setLoadType(VALUE_ADDUP);
}
