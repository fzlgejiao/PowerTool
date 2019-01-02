#ifndef MESSAGEOUTPUT_H
#define MESSAGEOUTPUT_H

#include <QWidget>
#include "ui_messageoutput.h"

class MessageOutput : public QWidget
{
	Q_OBJECT

public:
	MessageOutput(QStringList message,QWidget *parent = 0);
	~MessageOutput();
	void update(QStringList message);

private slots:
		void OnClose();
		void OnClear();

private:
	Ui::MessageOutput ui;
};

#endif // MESSAGEOUTPUT_H
