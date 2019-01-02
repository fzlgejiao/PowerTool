#include "messageoutput.h"

MessageOutput::MessageOutput(QStringList message,QWidget *parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	ui.setupUi(this);
	this->setFixedSize(size());
	
	foreach(QString str,message)
		ui.plainTextEdit->appendPlainText(str);

	connect(ui.pushButton_clear,SIGNAL(clicked()),this,SLOT(OnClear()));
	connect(ui.pushButton_close,SIGNAL(clicked()),this,SLOT(OnClose()));
}

MessageOutput::~MessageOutput()
{
	
}

void MessageOutput::OnClose()
{		
	this->close();
	delete this;
}
void MessageOutput::OnClear()
{
	ui.plainTextEdit->clear();
}
void MessageOutput::update(QStringList message)
{
	ui.plainTextEdit->clear();
	foreach(QString str,message)
		ui.plainTextEdit->appendPlainText(str);
}
