
#include <QtGui>

#include "diagramscene.h"
#include "arrow.h"
#include "idoc.h"
#include "adddialog.h"
#include "mainwindow.h"
#include "stationparameterdialog.h"
#include "textdialog.h"
#include "stationnamedialog.h"
#include "stationvaluedialog.h"
#include "brancheditdialog.h"
//! [0]
DiagramScene::DiagramScene(iDoc* doc,QObject *parent)
    : QGraphicsScene(parent)
{
	pMain		= (MainWindow *)parent;
	myDoc		= doc;

    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

	myFont.setWeight(QFont::Bold);

	//context menus
	propertyAction = new QAction(tr("&Properties..."), this);
	propertyAction->setStatusTip(tr("Show object property"));

	editStationAction = new QAction(tr("&Edit Station..."), this);
	editStationAction->setStatusTip(tr("Edit station"));

	editBranchAction = new QAction(tr("&Edit Branch..."), this);
	editBranchAction->setStatusTip(tr("Edit branch"));

	deleteAction = new QAction(QIcon(":/images/delete.png"),tr("&Delete"), this);
	deleteAction->setStatusTip(tr("Delete item from diagram"));

	defPositionAction = new QAction(tr("&Return to default position"), this);
	defPositionAction->setStatusTip(tr("Return to default position"));

	sceneMenu=new QMenu();
	sceneMenu->addAction("none-select1");
	sceneMenu->addAction("none-select2");

	multiMenu=new QMenu();
	multiMenu->addAction("multi-select1");
	multiMenu->addAction("multi-select2");


	statMenu=new QMenu();
	statMenu->addAction(propertyAction);
	statMenu->addAction(editStationAction);
	statMenu->addSeparator();
	statMenu->addAction(deleteAction);

	statValueMenu=new QMenu();
	statValueMenu->addAction(propertyAction);
	statValueMenu->addAction(defPositionAction);

	statNameMenu=new QMenu();
	statNameMenu->addAction(propertyAction);
	statNameMenu->addAction(defPositionAction);

	branchMenu=new QMenu();
	branchMenu->addAction(propertyAction);
	branchMenu->addAction(editBranchAction);

	noteMenu=new QMenu();
	noteMenu->addAction(propertyAction);
	noteMenu->addAction(deleteAction);

	//add menus to scene and items
	addMenu(MENU_SCENE,sceneMenu);
	addMenu(MENU_STAT,statMenu);
	addMenu(MENU_STAT_NAME,statNameMenu);
	addMenu(MENU_STAT_VALUE,statValueMenu);
	addMenu(MENU_BRANCH,branchMenu);
	
	m_controlpanel.showtype=SHOW_POWERFLOW;
	m_controlpanel.isShowStationName=true;
	m_controlpanel.isShowStationValue=true;
	m_controlpanel.isShowBranchLine=true;
	m_controlpanel.isShowBranchValue=true;
	m_controlpanel.isShowReactivePowerValue=true;
	m_controlpanel.isShowAdmittance=false;
	m_controlpanel.isShowVoltageAngle=false;
	m_controlpanel.isShowAllNodeVoltage=false;
	m_controlpanel.unittype=UNIT_ACTUALVALUE;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item =
            qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item =
            qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]


//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
	qDebug("PressEvent");
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (pMain->mode() == M_MoveItem) 
	{
    }   
	mouseEvent->accept();
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	qDebug("MoveEvent");
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
   if (mouseEvent->button() == Qt::LeftButton)
	{
		switch (pMain->mode()) 
		{
		case M_AddStation:
			{
				QList<iNodeData *> Nodelist;
				myDoc->getAvailableNode(Nodelist);

				if(Nodelist.count())
				{
					addStation(mouseEvent->scenePos());
				}

				emit modeDone();
			}
			break;

		case M_AddNote:
			{
				TextDialog textdlg(NULL);
				if(textdlg.exec()==QDialog::Accepted)
				{
					addNote(mouseEvent->scenePos());
				}
				emit modeDone();
			}
			break;
		}
	}
	/*
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem =
                qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem =
                qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
//! [12] //! [13]
    line = 0;
	*/
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
	qDebug("ReleaseEvent");
}
void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QList<QGraphicsItem *> &list = selectedItems();
	if(list.count() <= 1)																			//none-selected
	{	
		viewItem();	
	}
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);								
	qDebug("DoubleClickEvent");
}
void DiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	qreal     x = event->scenePos().x();
	qreal     y = event->scenePos().y();

	QList<QGraphicsItem *> &list = selectedItems();
	if(list.count() == 0)																			//none-selected
	{	
		if(sceneMenu)
			sceneMenu->exec(event->screenPos());
	}
	else if(list.count() > 1)																		//multi-selected
	{
		if(multiMenu)
			multiMenu->exec(event->screenPos());

	}
	else																							//one-selected
	{
		QGraphicsItem* item = itemAt( x, y );
		if(!item)
			return;
		iData* data = (iData *)item->data(ITEM_DATA).toUInt();
		if(!data)
			return;
		switch(data->type())																		
		{
		case T_STAT:																				//station 
			{
				iSTAT* stat = (iSTAT *)data;
				if(item->type() == DiagramItem::Type)												//station item									
				{
					DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
					QMenu* menu = getMenu(MENU_STAT);
					if(!menu)
						return;
					QAction* action = menu->exec(event->screenPos());
					if(action == propertyAction)
						viewStation(statItem,stat);
					else if(action == editStationAction)
						editStation(statItem,stat);
					else if(action == deleteAction)
						deleteStation(statItem,stat);

				}
				else if(item->type() == DiagramTextItem::Type)										//station value/name item
				{
					DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);
					if(item == stat->itemName())
					{
						QMenu* menu = getMenu(MENU_STAT_NAME);
						if(!menu)
							return;
						QAction* action = menu->exec(event->screenPos());
						if(action == propertyAction)
							viewStationName(textItem,stat);
						else if(action == defPositionAction)
							textItem->backToDefaultPos();
					}
					else if(item == stat->itemValue())
					{
						QMenu* menu = getMenu(MENU_STAT_VALUE);
						if(!menu)
							return;
						QAction* action = menu->exec(event->screenPos());
						if(action == propertyAction)
							viewStationValue(textItem,stat);
						else if(action == defPositionAction)
							textItem->backToDefaultPos();
					}
				}
			}
			break;
		case T_BRANCH:
			{
				iBRANCH* branch = (iBRANCH *)data;
				if(item->type() == Arrow::Type)
				{
						QMenu* menu = getMenu(MENU_BRANCH);
						if(!menu)
							return;
						QAction* action = menu->exec(event->screenPos());
						if(action == propertyAction)
							viewBranch(branch);
						else if(action == editBranchAction)
							editBranch(branch);
				}
			}
			break;
		case T_NOTE:
			break;
		}
	}
	//QGraphicsScene::contextMenuEvent(event);
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
void DiagramScene::viewItem()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;

	procItem(ACT_VIEW,item);

}
void DiagramScene::editItem()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;

	procItem(ACT_EDIT,item);
}
void DiagramScene::deleteItems()
{
    foreach (QGraphicsItem *item, selectedItems()) 
	{
		iData* data = (iData *)item->data(ITEM_DATA).toUInt();
		if(!data)
			continue;
         if (data->type() == T_STAT && item->type() == DiagramItem::Type) 
		 {
			 DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
             deleteStation(statItem,(iSTAT *)data);
         }
     }
}
void DiagramScene::procItem(ACT_TYPE act,QGraphicsItem* item)
{
	iData* data = (iData *)item->data(ITEM_DATA).toUInt();
	if(!data)
		return;
	switch(data->type())																		
	{
	case T_STAT:																				//station 
		{
			iSTAT* stat = (iSTAT *)data;
			if(item->type() == DiagramItem::Type)												//station item									
			{
				DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
				if(act == ACT_VIEW)
					viewStation(statItem,stat);
				else if(act == ACT_EDIT)
					editStation(statItem,stat);
				else if(act == ACT_DELETE)
					deleteStation(statItem,stat);
			}
			else if(item->type() == DiagramTextItem::Type)										//station value/name item
			{
				DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);
				if(act == ACT_VIEW)
				{
					if(textItem == stat->itemName())
						viewStationName(textItem,stat);
					else
						viewStationValue(textItem,stat);
				}
			}
		}
		break;
	case T_BRANCH:
		{
			viewBranch((iBRANCH *)data);
		}
		break;
	case T_NOTE:
		break;
	}
}
void DiagramScene::addStation(const QPointF& pos)
{
	AddDialog dlg(myDoc,NULL,pMain);
	if(dlg.exec()!=QDialog::Accepted)
		return;

	if(!dlg.IsAddSite()) 
		return ;
	//To do : add new station data object			
	iSTAT* stat= myDoc->STAT_new(dlg.NewStationName());												//create a new station object
	QList<iNodeData *> addednodes=dlg.GetAddedNodes();		;
	stat->setNodes(addednodes);

	//create station diagram item
	DiagramItem *item = new DiagramItem(stat, 0,this);												//create diagram item for station
	item->setBrush(myItemColor);
	addItem(item);
	item->setPos(pos);
	stat->itemAdded(item);																			//set item to station

	//create station name text item
	//DiagramTextItem* itemName = new DiagramTextItem(item,this);
	//itemName->setFont(myFont);
	//itemName->setPlainText(stat->name());
	////itemName->setTextInteractionFlags(Qt::TextEditorInteraction);
	////itemName->setZValue(1000.0);
	////connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),   this, SLOT(editorLostFocus(DiagramTextItem*)));
	////connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),this, SIGNAL(itemSelected(QGraphicsItem*)));
	//addItem(itemName);
	//itemName->setDefaultTextColor(myTextColor);
	//itemName->setPos(QPointF(20,20));
	//////emit textInserted(itemName);

	DiagramTextItem* itemName = new DiagramTextItem(item,this);
	//itemName->setFont(myFont);
	itemName->setFont(dlg.GetFont());
	itemName->setPlainText(stat->name());
	itemName->setDefaultTextColor(Qt::red);
	itemName->setPos(QPointF(20,20));
	itemName->setDefaultPos(QPointF(20,20));
	itemName->setData(ITEM_DATA,(uint)stat);
	addItem(itemName);
	stat->setItemName(itemName);

	//create station value text item
	DiagramTextItem* itemValue = new DiagramTextItem(item,this);
	itemValue->setFont(myFont);	
	itemValue->setPlainText(stat->value());
	itemValue->setDefaultTextColor(Qt::red);
	itemValue->setPos(QPointF(20,-20));
	itemValue->setDefaultPos(QPointF(20,-20));
	itemValue->setData(ITEM_DATA,(uint)stat);
	addItem(itemValue);
	stat->setItemValue(itemValue);

	//create station to staiton arrow item
	foreach(iNodeData* node,stat->nodeDatas())
	{
		if(!node)
			break;

		//to check if both fromBus and toBus of the attached data to this bus are already added
		foreach(iLinkData* data,node->linkDatas())
		{
			if(!data)
				break;

			int from = data->fromUid();
			int to	 = data->toUid();
			iNodeData* node1 = myDoc->getNode(from);
			iNodeData* node2 = myDoc->getNode(to);
			if(!node1 || !node2)
				break;
			if(node1->statId() == 0 || node2->statId() == 0 || node1->statId() == node2->statId())
				break;

			iSTAT* stat1 = myDoc->STAT_get(node1->statId());
			iSTAT* stat2 = myDoc->STAT_get(node2->statId());
			if(!stat1 || !stat2)
				break;

			DiagramItem *startItem	= stat1->myItem();
			DiagramItem *endItem	= stat2->myItem();
			if(!startItem || !endItem)
				break;

			if(data->type() == T_BRANCH)
			{
				Arrow *arrow = new Arrow(startItem, endItem,data);									//create arrow item for brunch
				arrow->setColor(myLineColor);
				startItem->addArrow(arrow);
				endItem->addArrow(arrow);
				arrow->setZValue(-1000.0);
				addItem(arrow);
				arrow->updatePosition();
			}
		}

	}
	item->setSelected(true);

}

void DiagramScene::editStation(DiagramItem *item,iSTAT* stat)
{
	AddDialog dlg(myDoc,stat,pMain);
	if(dlg.exec()!=QDialog::Accepted)
		return;		

	//update the name
	stat->setName(dlg.NewStationName());
	stat->itemName()->setPlainText(dlg.NewStationName());
	//update the name font
	stat->itemName()->setFont(dlg.GetFont());

	//change nodes of station
	QList<iNodeData *> addednodes=dlg.GetAddedNodes();
	stat->setNodes(addednodes);

	item->removeArrows();

	foreach(iNodeData* node,stat->nodeDatas())
	{
		if(!node)
			break;

		//to check if both fromBus and toBus of the attached data to this bus are already added
		foreach(iLinkData* data,node->linkDatas())
		{
			if(!data)
				break;

			int from = data->fromUid();
			int to	 = data->toUid();
			iNodeData* node1 = myDoc->getNode(from);
			iNodeData* node2 = myDoc->getNode(to);
			if(!node1 || !node2)
				break;
			if(node1->statId() == 0 || node2->statId() == 0 || node1->statId() == node2->statId())
				break;

			iSTAT* stat1 = myDoc->STAT_get(node1->statId());
			iSTAT* stat2 = myDoc->STAT_get(node2->statId());
			if(!stat1 || !stat2)
				break;

			DiagramItem *startItem	= stat1->myItem();
			DiagramItem *endItem	= stat2->myItem();
			if(!startItem || !endItem)
				break;

			if(data->type() == T_BRANCH)
			{
				Arrow *arrow = new Arrow(startItem, endItem,data);									//create arrow item for brunch
				arrow->setColor(myLineColor);
				startItem->addArrow(arrow);
				endItem->addArrow(arrow);
				arrow->setZValue(-1000.0);
				addItem(arrow);
				arrow->updatePosition();
			}
		}

	}	

}
void DiagramScene::viewStation(DiagramItem *item,iSTAT* stat)
{
	StationParameterDialog dlg(stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{
		QString name = dlg.GetStationName();
		stat->setName(name);
		
		//change station name item
		DiagramTextItem* itemName = stat->itemName();
		if(itemName)
			itemName->setPlainText(name);
	}		
}
void DiagramScene::deleteStation(DiagramItem *item,iSTAT* stat)
{
	item->removeArrows();
	removeItem(item);

	stat->itemRemoved();
	myDoc->STAT_delete(stat->Id());

	delete item;
}
void DiagramScene::viewStationName(DiagramTextItem *item,iSTAT* stat)
{
	//todo: show station name property dialog
	
	StationNameDialog dlg(stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{
		//item->setFont(dlg.GetFont());
		stat->itemName()->setFont(dlg.GetFont());
		
		if(dlg.IsApplyAll())
		{
			//To do : apply the font to all station 
		}
	}
}
void DiagramScene::viewStationValue(DiagramTextItem *item,iSTAT* stat)
{
	//todo: show station value property dialog
	StationValueDialog dlg(stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		
	}
}
void DiagramScene::viewBranch(iBRANCH* branch)
{
	BranchEditDialog dlg(branch,BranchView,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		
	}
}
void DiagramScene::editBranch(iBRANCH* branch)
{
	BranchEditDialog dlg(branch,BranchEdit,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		
	}
}
void DiagramScene::addNote(const QPointF& pos)
{
	DiagramTextItem* TextItem = new DiagramTextItem(NULL,this);	
	TextItem->setPos(pos);	
	TextItem->setPlainText("Add Text demo");
	addItem(TextItem);
}
void DiagramScene::viewNote()
{
}