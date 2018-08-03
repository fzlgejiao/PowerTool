
#include <QtGui>

#include "diagramscene.h"
#include "arrow.h"
#include "idoc.h"

//! [0]
DiagramScene::DiagramScene(iDoc* doc,QObject *parent)
    : QGraphicsScene(parent)
{
	myDoc		= doc;
    myMode		= MoveItem;

    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
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
    if (mouseEvent->button() != Qt::LeftButton)
        return;

	//QList<iBUS*> list;
	//myDoc->getAvailableBus(list);
	//
	//if(list.isEmpty() == false)
	//{
	//	iBUS* bus = list.first();
	//	if(bus)
	//		addBUS(bus,mouseEvent->scenePos());
	//}
	iSTAT* stat = myDoc->getFreeSTAT();
	if(stat)
		addSTAT(stat,mouseEvent->scenePos());

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
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
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
		emit itemDBClicked(item);																	//send out item double clicked signal
}
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::addSTAT(iSTAT* stat,const QPointF& pos)
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