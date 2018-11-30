#include <QtGui>
#include "newdialog.h"

NewDialog::NewDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

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
	ui.leFile->setText(m_szFile);
}
