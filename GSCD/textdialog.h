#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include "ui_textdialog.h"

class TextDialog : public QDialog
{
	Q_OBJECT

public:
	TextDialog(const QString& text ,QWidget *parent = 0);
	~TextDialog();

private:
	Ui::TextDialog ui;
};

#endif // TEXTDIALOG_H
