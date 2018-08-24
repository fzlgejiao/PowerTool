
#include <QtGui>

#include "diagramitem.h"
#include "arrow.h"
#include "idata.h"
#include "diagramscene.h"

//! [0]
DiagramItem::DiagramItem(iData* data, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
	setData(ITEM_DATA,(uint)data);

	updateData();

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
}
//! [0]
iData* DiagramItem::myData()
{
	return (iData *)data(ITEM_DATA).toUInt();
}
//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
//! [3]

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }
   if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        if (value == true)
        {
            // do stuff if selected
			foreach(QGraphicsItem *item,childItems())
			{
				QGraphicsSimpleTextItem* txtItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(item);
				if(txtItem)
				{
					txtItem->setPen(QPen(Qt::green,1));
				}
			}
        }
        else
        {
            // do stuff if not selected
        }
    }
    return value;
}
//! [6]
void DiagramItem::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	if (mouseEvent->button() == Qt::RightButton)
	{
		scene()->clearSelection();
		setSelected(true);
	}
	QGraphicsItem::mousePressEvent(mouseEvent);
}
void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsItem::mouseDoubleClickEvent(mouseEvent);
}
QRectF DiagramItem::boundingRect() const
{
    return QGraphicsPolygonItem::boundingRect() | childrenBoundingRect();
}
void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QStyleOptionGraphicsItem op;
	op.initFrom(widget);

	// set state to State_None when selected
	if (option->state & QStyle::State_Selected)
	{
		setPen(QPen(Qt::blue,2));
		op.state = QStyle::State_None;
	}
	else
	{
		setPen(QPen(Qt::black,2));
	}

	// call default func to draw
	QGraphicsPolygonItem::paint(painter, &op, widget);
}
void DiagramItem::updateData()
{
	iSTAT* stat = (iSTAT *)myData();
	if(!stat)
		return;
	myPolygon.clear();

    QPainterPath path;
	switch (stat->sType()) {
 /*       case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;*/
        case STAT_220KV:
            myPolygon << QPointF(-20, 0) << QPointF(0, 20)
                      << QPointF(20, 0) << QPointF(0, -20)
                      << QPointF(-20, 0);
            break;
		case STAT_330KV:
            myPolygon << QPointF(-15, -15) << QPointF(15, -15)
                      << QPointF(15, 15) << QPointF(-15, 15)
                      << QPointF(-15, -15);
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
}