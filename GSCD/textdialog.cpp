#include "textdialog.h"
#include <QFontDialog>
#include <QMenu>
#include <QColorDialog>

TextDialog::TextDialog(iNote *note,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
	if(!note)
	{			
		m_color=QColor("black");
		m_font=QFont("Arial",10);		
		m_alignmode=Qt::AlignCenter;
		hasborder=false;
		m_note=NULL;
	}else
	{				
		m_note=note;
		m_color=m_note->getTextColor();
		m_font=m_note->getTextFont();
		ui.textEdit_inputtext->setText(m_note->text());
		m_alignmode=m_note->getAlignMode();
		hasborder=m_note->hasborder();
	}

	ui.textEdit_inputtext->setFont(m_font);

	ui.checkBox_border->setChecked(hasborder);
	QString strcolor=QString("color:white;background-color: rgb(%1, %2, %3)").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
	ui.pushButton_color->setStyleSheet(strcolor);	
	switch(m_alignmode)
	{
	case Qt::AlignLeft:
		ui.radioButton_alignleft->setChecked(true);
		break;

	case Qt::AlignCenter:
		ui.radioButton_aligncenter->setChecked(true);
		break;

	case Qt::AlignRight:
		ui.radioButton_alignright->setChecked(true);
		break;
	}

	connect(ui.checkBox_border,SIGNAL(toggled(bool)),this,SLOT(OnBordertoggled(bool)));

	connect(ui.radioButton_alignleft,SIGNAL(clicked()),this,SLOT(OnAlignmodeChanged()));
	connect(ui.radioButton_aligncenter,SIGNAL(clicked()),this,SLOT(OnAlignmodeChanged()));
	connect(ui.radioButton_alignright,SIGNAL(clicked()),this,SLOT(OnAlignmodeChanged()));

	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontdialog()));
	connect(ui.pushButton_color,SIGNAL(clicked()),this,SLOT(OnColorChanged()));

	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(OnOK()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	

}
TextDialog::~TextDialog()
{

}
void TextDialog::OnBordertoggled(bool toggle)
{
	hasborder=toggle;
}
void TextDialog::OnFontdialog()
{
	bool ok;
	QFont font=QFontDialog::getFont(&ok,m_font,this);
	if(ok)
	{
		m_font=font;	
		ui.textEdit_inputtext->setFont(m_font);
	}
}
void TextDialog::OnColorChanged()
{
	QColor color=QColorDialog::getColor(m_color, this);
	if(!color.isValid()) return ;
	m_color=color;
	QString strcolor=QString("color:white;background-color: rgb(%1, %2, %3)").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
	ui.pushButton_color->setStyleSheet(strcolor);	
}
void TextDialog::OnAlignmodeChanged()
{
	if(ui.radioButton_alignleft->isChecked())
		m_alignmode=Qt::AlignLeft;
	else if(ui.radioButton_aligncenter->isChecked())
		m_alignmode=Qt::AlignCenter;
	else if(ui.radioButton_alignright->isChecked())
		m_alignmode=Qt::AlignRight;
	else 
		m_alignmode=Qt::AlignCenter;
}

void TextDialog::OnOK()
{
	if(m_note)
	{
		m_note->setTextFont(m_font);
		m_note->setTextColor(m_color);
		m_note->setAlignmode(m_alignmode);
		m_note->setborder(hasborder);
		m_note->setText(ui.textEdit_inputtext->toPlainText().trimmed());
	}
	this->accept();
}
void TextDialog::changeEvent(QEvent *e)
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

