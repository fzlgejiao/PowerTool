#include "areasetting.h"

AreaSetting::AreaSetting(QSize &size,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//standard page Size , landscape mode
	/*
	A5: 210  * 148
	A4: 297  * 210
	A3: 420  * 297
	A2: 594  * 420
	A1: 841  * 594
	A0: 1189 * 841
	*/
	m_size=size;
	ui.spinBox_width->setValue(m_size.width());
	ui.spinBox_height->setValue(m_size.height());

	ui.spinBox_height->setToolTip(QString(tr("Min=%1  Max=%2")).arg(ui.spinBox_height->minimum()).arg(ui.spinBox_height->maximum()));
	ui.spinBox_width->setToolTip(QString(tr("Min=%1  Max=%2")).arg(ui.spinBox_width->minimum()).arg(ui.spinBox_width->maximum()));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

AreaSetting::~AreaSetting()
{

}

QSize  AreaSetting::getAreasize()
{
	QSize size(ui.spinBox_width->value(),ui.spinBox_height->value());

	return size;
}
void AreaSetting::changeEvent(QEvent *e)
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
