#ifndef AREASETTING_H
#define AREASETTING_H

#include <QDialog>
#include "ui_areasetting.h"


enum AreaSize
{
	A5		=0,
	A4,
	A3,
	A2,
	A1,
	A0,
	Custom
};

class AreaSetting : public QDialog
{
	Q_OBJECT

public:
	AreaSetting(QWidget *parent = 0);
	~AreaSetting();

private slots:
		void OnAreaSizeChanged(int index);
private:
	Ui::AreaSetting ui;
};

#endif // AREASETTING_H
