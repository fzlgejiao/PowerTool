#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H

#include <QDialog>
#include "ui_parameterdialog.h"

class ParameterDialog : public QDialog
{
	Q_OBJECT

public:
	ParameterDialog(QWidget *parent = 0);
	~ParameterDialog();

private:
	Ui::ParameterDialog ui;
};

#endif // PARAMETERDIALOG_H
