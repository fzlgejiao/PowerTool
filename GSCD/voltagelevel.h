#ifndef VOLTAGELEVEL_H
#define VOLTAGELEVEL_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "ui_voltagelevel.h"
#include "idata.h"
#include "iglobal.h"

class iGlobal;
enum LevelColumns
{
	RefVoltage	=0,
	Width,
	Color,	
	VoltageLevelColumnCnt
};

#define	CHG_COLORMAP		0x01
#define	CHG_WARNING 		0x02
#define	CHG_DCWIDTH		    0x04
#define	CHG_DCCOLOR 		0x08
#define	CHG_DEFAULTCOLOR 	0x10
#define	CHG_LEVELS 			0x20

class VoltageItem:public QTableWidgetItem
{
public:
	VoltageItem()
		:QTableWidgetItem()
	{

	}

	bool operator < (const QTableWidgetItem &other) const
	{
		QString value1 = text();
		QString value2 = other.text();
		
		if(value1=="Ground") return true;

		value1.remove("kV");
		value2.remove("kV");
		return value1.toFloat() < value2.toFloat();
	}
};

class VoltageLevel : public QDialog
{
	Q_OBJECT

public:
	VoltageLevel(QWidget *parent = 0);
	~VoltageLevel();

private slots:
		void Onvoltage_edit();
		void Onvoltage_new();
		void Onvoltage_delete();
		void OnSave();

		void OnDCColorChanged();
		void OnDefaultColorChanged();
		void OnDcWidthChanged(int index);

		void Oncolormap(bool check);
		void Onwarning(bool check);

private:
	Ui::VoltageLevel ui;
	QList<CVoltageLevel *> m_levels;
	void addLevels2Table();
	void addLevel2Table(CVoltageLevel * level);
	iGlobal &m_Global;

	QColor m_dccolor;
	QColor m_defaultcolor;
	int    m_dcwidth;
	bool   m_iscolormap;
	bool   m_iswarning;
	ushort m_changes;

signals:
	void colormapChanged(bool iscolor);
	void warningChanged(bool iswarning);
	void levelschanged();
};


#endif // VOLTAGELEVEL_H
