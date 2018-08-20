#include "stationnamedialog.h"
#include "idata.h"
#include "diagramtextitem.h"
#include <QFontDialog>
#include <QFont>

StationNameDialog::StationNameDialog(DiagramTextItem *item,iSTAT *stat,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_stat=stat;
	m_textitem=item;

	m_IsApplyAll=false;
	m_font=m_textitem->font();
	ui.lineEdit_name->setText(m_stat->name());
	
	ui.lineEdit_namepreview->setText(m_stat->name());
	ui.lineEdit_namepreview->setFont(m_font);

	connect(ui.pushButton_clear,SIGNAL(clicked()),this,SLOT(OnclearshownNodes()));
	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontchanged()));
	connect(ui.checkBox_applyall,SIGNAL(stateChanged (int)),this,SLOT(OnCheckBoxChanged(int)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}

StationNameDialog::~StationNameDialog()
{

}
void StationNameDialog::OnCheckBoxChanged(int state)
{
	if(ui.checkBox_applyall->isChecked()) m_IsApplyAll=true;
	else m_IsApplyAll=false;
}
void StationNameDialog::OnclearshownNodes()
{

}
void StationNameDialog::OnFontchanged()
{
	bool ok;
	QFont font=QFontDialog::getFont(&ok,m_font);
	if(ok)
	{
		m_font=font;
		ui.lineEdit_namepreview->setFont(m_font);
	}
}
