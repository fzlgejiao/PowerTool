#include "stationnamedialog.h"
#include "idata.h"
#include "diagramtextitem.h"
#include <QFontDialog>
#include <QFont>
#include <QStringListModel>

StationNameDialog::StationNameDialog(iSTAT *stat,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_stat=stat;
	m_textitem=stat->itemName();
	this->setFixedSize(this->size());

	m_IsApplyAll=false;
	m_font=m_textitem->font();
	ui.lineEdit_name->setText(m_stat->name());
	
	ui.lineEdit_namepreview->setText(m_stat->name());
	ui.lineEdit_namepreview->setFont(m_font);

	QStringList list;
	foreach(iNodeData *node,m_stat->nodeDatas())
	{
		if(node->type()==T_BUS)
		{
			iBUS *bus=(iBUS *)node;			
			QString str=QString("%1 --- %2kV").arg(bus->name()).arg(node->GetRefVoltage());
			list.append(str);
		}
	}
	
	QStringListModel *model= new QStringListModel(this);
    model->setStringList(list);
	
	ui.listView_shownnodes->setModel(model);
	
	for(int row=0;row<list.count();row++)
	{
		QModelIndex index = model->index(row);
		if(m_stat->nodeDatas()[row]->isShowVoltge()) 
			ui.listView_shownnodes->setCurrentIndex(index);
	}

	connect(ui.pushButton_clear,SIGNAL(clicked()),this,SLOT(OnclearshownNodes()));
	connect(ui.pushButton_font,SIGNAL(clicked()),this,SLOT(OnFontchanged()));
	connect(ui.checkBox_applyall,SIGNAL(stateChanged (int)),this,SLOT(OnCheckBoxChanged(int)));

	connect(ui.listView_shownnodes,SIGNAL(clicked(const QModelIndex&)),this,SLOT(OnshowNodeChanged(const QModelIndex&)));
	
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(ui.buttonBox,SIGNAL(rejected()),this,SLOT(reject()));	
}
StationNameDialog::~StationNameDialog()
{

}
void StationNameDialog::OnshowNodeChanged(const QModelIndex& index)
{	
}
QString StationNameDialog::StationName()
{
	return (ui.lineEdit_name->text().trimmed());
}
void StationNameDialog::OnCheckBoxChanged(int state)
{
	if(ui.checkBox_applyall->isChecked()) m_IsApplyAll=true;
	else m_IsApplyAll=false;
}
void StationNameDialog::OnclearshownNodes()
{
	ui.listView_shownnodes->clearSelection();
}
void StationNameDialog::OnFontchanged()
{
	bool ok;
	QFont font=QFontDialog::getFont(&ok,m_font);
	if(ok)
	{
		m_font=font;
		ui.lineEdit_namepreview->setFont(m_font);
	}
}
