#include "editvoltagelevel.h"
#include <QColorDialog>

EditVoltageLevel::EditVoltageLevel(CVoltageLevel *level,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_level=level;

	voltagevalidator=new QDoubleValidator();
	voltagevalidator->setBottom(0);
	ui.lineEdit_voltage->setValidator(voltagevalidator);

	m_width=1;
	m_voltage=0;
	m_color=QColor(Qt::black);

	if(m_level)
	{		
		m_width=m_level->getwidth();
		m_voltage=m_level->refVoltage();
		m_color=m_level->getcolor();
	}	

	ui.lineEdit_voltage->setText(QString::number(m_voltage));
	ui.comboBox_linewidth->setCurrentIndex(m_width);
	ui.pushButton_color->setStyleSheet(QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()));
	
	connect(ui.pushButton_color,SIGNAL(clicked()),this,SLOT(OnColorChanged()));
	connect(ui.comboBox_linewidth,SIGNAL(currentIndexChanged(int)),this,SLOT(OnWidthChanged(int)));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(OnOK()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

EditVoltageLevel::~EditVoltageLevel()
{

}

void EditVoltageLevel::OnColorChanged()
{
	QColor color=QColorDialog::getColor(m_color, this);
	if(!color.isValid()) return ;
	m_color= color;
 
	QString strcolor=QString("color:white;background-color:rgb(%1,%2,%3)").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
	ui.pushButton_color->setStyleSheet(strcolor);	
}
void EditVoltageLevel::OnOK()
{
	/*if(m_level) 
	{
		m_level->setvoltage(m_voltage);
		m_level->setwidth(m_width);
		m_level->setcolor(QString("rgb(%1,%2,%3)").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()));
	}*/
	this->accept();
}
void EditVoltageLevel::changeEvent(QEvent *e)
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
