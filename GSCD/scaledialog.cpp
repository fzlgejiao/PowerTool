#include "scaledialog.h"

ScaleDialog::ScaleDialog(int scale,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	m_scale=scale;
	scalemin=10;
	scalemax=500;
	ui.ScaleSlider->setMinimum(scalemin);
	ui.ScaleSlider->setMaximum(scalemax);
	ui.ScaleSlider->setValue(m_scale);
	
	ui.label_min->setText(QString("%1%").arg(scalemin));
	ui.label_max->setText(QString("%1%").arg(scalemax));
	ui.lineEdit_scale->setText(QString("%1").arg(m_scale));

	connect(ui.ScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(OnScaleSliderChanged(int)));
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

ScaleDialog::~ScaleDialog()
{

}

void ScaleDialog::OnScaleSliderChanged(int value)
{	
	m_scale=value;
	ui.lineEdit_scale->setText(QString("%1").arg(value));
}
