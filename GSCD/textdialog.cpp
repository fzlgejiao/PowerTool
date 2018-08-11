#include "textdialog.h"

TextDialog::TextDialog(const QString& text,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size());
}

TextDialog::~TextDialog()
{

}
