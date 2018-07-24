#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "ui_adddialog.h"


class iDoc;

class AddDialog : public QDialog
{
	Q_OBJECT

public:
	AddDialog(iDoc * idoc ,QWidget *parent = 0);
	~AddDialog();

private:
	Ui::AddDialog ui;

	iDoc *m_doc;

};

#endif // ADDDIALOG_H
