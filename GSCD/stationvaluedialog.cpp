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

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}

StationValueDialog::~StationValueDialog()
{

}
