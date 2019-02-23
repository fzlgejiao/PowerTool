#ifndef EDITVOLTAGELEVEL_H
#define EDITVOLTAGELEVEL_H

#include <QDialog>
#include "ui_editvoltagelevel.h"
#include "idata.h"
#include "iglobal.h"

class EditVoltageLevel : public QDialog
{
	Q_OBJECT

public:
	EditVoltageLevel(CVoltageLevel *level,QWidget *parent = 0);
	~EditVoltageLevel();

	float voltage(){return ui.lineEdit_voltage->text().trimmed().toFloat();};
	int width(){return ui.comboBox_linewidth->currentIndex();}
	QColor color(){return m_color;}

private slots:
	void OnColorChanged();
	void OnOK();
	void OnWidthChanged(int index){m_width=index;}

private:
	Ui::EditVoltageLevel ui;
	CVoltageLevel *m_level;
	QColor m_color;
	int m_width;
	float m_voltage;

	QDoubleValidator *voltagevalidator;
};

#endif // EDITVOLTAGELEVEL_H
