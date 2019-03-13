#ifndef AREASETTING_H
#define AREASETTING_H

#include <QDialog>
#include "ui_areasetting.h"


class AreaSetting : public QDialog
{
	Q_OBJECT

public:
	AreaSetting(QSize &size,QWidget *parent = 0);
	~AreaSetting();
	QSize  getAreasize();

protected:
   virtual void changeEvent(QEvent *e);

private slots:
		
private:
	Ui::AreaSetting ui;
	QSize m_size;
};

#endif // AREASETTING_H
