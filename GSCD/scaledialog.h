#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include "ui_scaledialog.h"

class ScaleDialog : public QDialog
{
	Q_OBJECT

public:
	ScaleDialog(int scale ,QWidget *parent = 0);
	~ScaleDialog();
	int GetScale() { return m_scale;}

private slots:
	void OnScaleSliderChanged(int value);

private:
	Ui::ScaleDialog ui;
	int m_scale;
	int scalemin;
	int scalemax;
};

#endif // SCALEDIALOG_H
