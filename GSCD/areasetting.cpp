#include "areasetting.h"

AreaSetting::AreaSetting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//standard page Size , landscape mode
	/*
	A5: 210  * 148
	A4: 297  * 210
	A3: 420  * 297
	A2: 594  * 420
	A1: 841  * 594
	A0: 1189 * 841
	*/
	ui.comboBox_size->setCurrentIndex(1);
	OnAreaSizeChanged(ui.comboBox_size->currentIndex());
	connect(ui.comboBox_size,SIGNAL(currentIndexChanged(int)),this,SLOT(OnAreaSizeChanged(int)));
}

AreaSetting::~AreaSetting()
{

}
void AreaSetting::OnAreaSizeChanged(int index)
{
	AreaSize size=(AreaSize)index;
	switch(size)
	{
	case A5:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(210);
			ui.spinBox_height->setValue(148);
		}
		break;

	case A4:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(297);
			ui.spinBox_height->setValue(210);
		}
		break;

	case A3:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(420);
			ui.spinBox_height->setValue(297);
		}
		break;

	case A2:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(594);
			ui.spinBox_height->setValue(420);
		}
		break;

	case A1:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(841);
			ui.spinBox_height->setValue(594);
		}
		break;

	case A0:
		{		
			ui.spinBox_width->setEnabled(false);
			ui.spinBox_height->setEnabled(false);
			ui.spinBox_width->setValue(1189);
			ui.spinBox_height->setValue(841);
		}
		break;

	case Custom:
		{		
			ui.spinBox_width->setEnabled(true);
			ui.spinBox_height->setEnabled(true);			
		}
		break;
	}
}

