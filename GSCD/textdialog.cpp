#include "textdialog.h"
#include <QFontDialog>
#include <QMenu>
#include <QColorDialog>

TextDialog::TextDialog(const QString& text,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());

	m_text=m_text;
	defaultfont=QFont("Arial",10);
	m_color=QColor("Black");
	m_font=defaultfont;
	ui.textEdit_inputtext->setText(m_text);
	//ui.pushButton_color->setMenu(new QMenu());
	
	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontdialog()));
	connect(ui.pushButton_color,SIGNAL(clicked()),this,SLOT(OnColorChanged()));
}
TextDialog::~TextDialog()
{

}
void TextDialog::OnFontdialog()
{
	bool ok;
	QFont font=QFontDialog::getFont(&ok,m_font);
	if(ok)
	{
			
	}
}
void TextDialog::OnColorChanged()
{
	QColor color=QColorDialog::getColor(m_color, this);
	m_color=color;
	ui.pushButton_color->setStyleSheet("color:white;background-color: rgb(255, 0, 0)");
	
}

