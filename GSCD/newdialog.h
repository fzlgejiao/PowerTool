#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include "ui_newdialog.h"

class NewDialog : public QDialog
{
	Q_OBJECT

public:
	NewDialog(QWidget *parent = 0,const QString& dataFile="");
	~NewDialog();
	QString FileName(){return m_szFile;}

private:
	Ui::NewDialog ui;
	QString	m_szFile;

private slots:
	void OnBrowse();
};

#endif // NEWDIALOG_H
