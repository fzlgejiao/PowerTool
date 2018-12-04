#include <QtGui>
#include "newdialog.h"

NewDialog::NewDialog(QWidget *parent,const QString& dataFile)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_szFile = dataFile;
	ui.leFile->setText(dataFile);

	connect(ui.btnBrowse,SIGNAL(clicked()),this,SLOT(OnBrowse()));
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

NewDialog::~NewDialog()
{

}
void NewDialog::OnBrowse()
{
	m_szFile = QFileDialog::getOpenFileName(this,tr("Open Raw file"),".",tr("Raw File (*.raw)"));
	if(m_szFile.isEmpty() == false)
		ui.leFile->setText(m_szFile);
}
