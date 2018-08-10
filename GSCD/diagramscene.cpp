
#include <QtGui>

#include "diagramscene.h"
#include "arrow.h"
#include "idoc.h"
#include "adddialog.h"
#include "mainwindow.h"
#include "stationparameterdialog.h"

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

	//context menus
	propertyAction = new QAction(tr("&Properties..."), this);
	propertyAction->setStatusTip(tr("Show object property"));
	connect(propertyAction, SIGNAL(triggered()),this, SLOT(viewProperty()));

	editStationAction = new QAction(tr("&Edit Station..."), this);
	editStationAction->setStatusTip(tr("Edit station"));
	connect(editStationAction, SIGNAL(triggered()),this, SLOT(editStation()));

	editBranchAction = new QAction(tr("&Edit Branch..."), this);
	editBranchAction->setStatusTip(tr("Edit branch"));
	connect(editBranchAction, SIGNAL(triggered()),this, SLOT(editBranch()));

	deleteAction = new QAction(QIcon(":/images/delete.png"),tr("&Delete"), this);
	deleteAction->setStatusTip(tr("Delete item from diagram"));
	connect(deleteAction, SIGNAL(triggered()),	this, SLOT(deleteItem()));

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


	//add menus to scene
	addMenu(T_NONE,sceneMenu);
	//addMenu(T_STAT,editMenu);
	addMenu(T_STAT,statMenu);
	addMenu(T_BRANCH,statMenu);
	addMenu(T_TRANSFORMER,statMenu);
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
        item->setDefaultTextColor(myTextColor);
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
        QGraphicsTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]


//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
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
		}

		
	/*
		DiagramItem *item;
		switch (myMode) {
			case InsertItem:
				item = new DiagramItem(myItemType, myItemMenu);
				item->setBrush(myItemColor);
				addItem(item);
				item->setPos(mouseEvent->scenePos());
				emit itemInserted(item);
				break;
	//! [6] //! [7]
			case InsertLine:
				line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
				line->setPen(QPen(myLineColor, 2));
				addItem(line);
				break;
	//! [7] //! [8]
			case InsertText:
				textItem = new DiagramTextItem();
				textItem->setFont(myFont);
				textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
				textItem->setZValue(1000.0);
				connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),   this, SLOT(editorLostFocus(DiagramTextItem*)));
				connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),this, SIGNAL(itemSelected(QGraphicsItem*)));
				addItem(textItem);
				textItem->setDefaultTextColor(myTextColor);
				textItem->setPos(mouseEvent->scenePos());
				emit textInserted(textItem);
	//! [8] //! [9]
		default:
			;
		}
	*/
	}
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (pMain->mode() == M_MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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
}
void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem *item = selectedItems().first();//qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
	if(item)
		viewProperty();																				//to show 'property' dialog
}
void DiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
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
		QGraphicsScene::contextMenuEvent(event);
	}
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
void DiagramScene::viewProperty()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;
	iData* data = (iData *)item->data(ITEM_DATA).toUInt();
	if(!data)
		return;

	if(data->type()==T_STAT)
	{	
		iSTAT* stat = (iSTAT *)data;
		StationParameterDialog dlg(stat,pMain);
		if(dlg.exec()==QDialog::Accepted)
		{
			QString name = dlg.GetStationName();
			stat->setName(name);
			DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
			 //if(statItem)
				// statItem->setName(name);
		}		
	}
	else if(data->type()==T_BRANCH)
	{
		//To do : branch parameter dialog
	}
}
void DiagramScene::addStation(const QPointF& pos)
{
	AddDialog dlg(myDoc,NULL,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{				
		if(!dlg.IsAddSite()) return ;
		//To do : add new station			
		iSTAT* stat= myDoc->STAT_new(dlg.NewStationName());								//create a new station object
		QList<iNodeData *> addednodes;
		dlg.GetNewAddedNodes(addednodes);
		stat->setNodes(addednodes);

		addStationItem(stat,pos);														//create a new station item
	}
}
void DiagramScene::editStation()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;
    if (item->type() != DiagramItem::Type) 
		return;

	iData *data=qgraphicsitem_cast<DiagramItem *>(item)->myData();
	if(!data)
		return;
	if(data->type()!=T_STAT)
		return;
	
	iSTAT* stat = (iSTAT *)data;
	AddDialog dlg(myDoc,stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{		
		//update the name
		stat->setName(dlg.NewStationName());										
		////Removed the nodes	
		//QList<iNodeData *> removednodes;
		//dlg.GetNewRemovedNodes(removednodes);								
		//stat->removeNodes(removednodes);		
		//Added  nodes
		QList<iNodeData *> addednodes;
		dlg.GetNewAddedNodes(addednodes);
		stat->setNodes(addednodes);
		//To do :update the branchs in the  diagram scence
	}				

}
void DiagramScene::editBranch()
{
}
void DiagramScene::deleteItem()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;
    if (item->type() == DiagramItem::Type) 
	{
        qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();

		removeItem(item);
		iData* data = qgraphicsitem_cast<DiagramItem *>(item)->myData();
		if(data && data->type() == T_STAT)
		{
			((iSTAT *)data)->itemRemoved();
			myDoc->STAT_delete(data->Id());
		}
		delete item;
	}
}
void DiagramScene::deleteItems()
{
    foreach (QGraphicsItem *item, selectedItems()) 
	{
         if (item->type() == DiagramItem::Type) 
		 {
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();

			 removeItem(item);
			 iData* data = qgraphicsitem_cast<DiagramItem *>(item)->myData();
			 if(data && data->type() == T_STAT)
			 {
				 ((iSTAT *)data)->itemRemoved();
				 myDoc->STAT_delete(data->Id());
			 }
			 delete item;
		}
     }
}
void DiagramScene::addStationItem(iSTAT* stat,const QPointF& pos)
{
	DiagramItem *item = new DiagramItem(stat, getMenu(T_STAT),0,this);								//create diagram item for station
	item->setBrush(myItemColor);
	addItem(item);
	item->setPos(pos);
	stat->itemAdded(item);																			//set item to station

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
				Arrow *arrow = new Arrow(startItem, endItem,data,getMenu(T_BRANCH));				//create arrow item for brunch
				arrow->setColor(myLineColor);
				startItem->addArrow(arrow);
				endItem->addArrow(arrow);
				arrow->setZValue(-1000.0);
				addItem(arrow);
				arrow->updatePosition();
			}
		}

	}

	emit itemInserted(item);
}
