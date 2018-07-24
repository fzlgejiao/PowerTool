#include "adddialog.h"

AddDialog::AddDialog(iDoc *idoc ,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_doc=idoc;
}

AddDialog::~AddDialog()
{

}
