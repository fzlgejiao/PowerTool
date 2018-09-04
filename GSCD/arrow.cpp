
#include <QtGui>

#include "arrow.h"
#include <math.h>
#include "idata.h"
#include "diagramscene.h"

const qreal Pi = 3.14;

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem,iData* data,
	QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsLineItem(parent, scene)
{
	setData(ITEM_DATA,(uint)data);
	myStartItem = startItem;
	myEndItem	= endItem;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	myColor = Qt::black;
	setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]
iData* Arrow::myData()
{
	return (iData *)data(ITEM_DATA).toUInt();
}
//! [1]
QRectF Arrow::boundingRect() const
{
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
	QPainterPath path = QGraphicsLineItem::shape();
	path.addPolygon(arrowHead);
	return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
	QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
	setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *op,QWidget *widget)
{
	if (myStartItem->collidesWithItem(myEndItem))
		return;
	
	QPen myPen = pen();
	myPen.setColor(myColor);
	qreal arrowSize = 10;
	painter->setPen(myPen);
	painter->setBrush(myColor);
	//! [4] //! [5]

	//QLineF centerLine(myStartItem->pos(), myEndItem->pos());
	////QPolygonF endPolygon = myEndItem->polygon();
	//QPolygonF endPolygon = myEndItem->path().toFillPolygon();
	//
	//QPointF p1 = endPolygon.first() + myEndItem->pos();
	//QPointF p2;
	//QPointF intersectPoint;
	//QLineF polyLine;
	//for (int i = 1; i < endPolygon.count(); ++i) {
	//	p2 = endPolygon.at(i) + myEndItem->pos();
	//	polyLine = QLineF(p1, p2);
	//	QLineF::IntersectType intersectType =
	//		polyLine.intersect(centerLine, &intersectPoint);
	//	if (intersectType == QLineF::BoundedIntersection)
	//		break;
	//	p1 = p2;
	//}
	QPointF startPoint=myStartItem->pos();
	QPointF endPoint=myEndItem->pos();

	setLine(QLineF(endPoint,startPoint));
	//! [5] //! [6]

	QPointF middlepoint= QPointF((endPoint.x() + startPoint.x()) / 2.0, (startPoint.y() + endPoint.y()) / 2.0);

	double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (Pi * 2) - angle;

	QPointF arrowP1 = middlepoint + QPointF(sin(angle + Pi / 3) * arrowSize,
		cos(angle + Pi / 3) * arrowSize);
	QPointF arrowP2 = middlepoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
		cos(angle + Pi - Pi / 3) * arrowSize);

	arrowHead.clear();
	//arrowHead << line().p1() << arrowP1 << arrowP2;	
	arrowHead << middlepoint << arrowP1 << arrowP2;
	//! [6] //! [7]
	if (isSelected()) {
		/*painter->setPen(QPen(myColor, 1, Qt::DashLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0,-8.0);
		painter->drawLine(myLine);*/

		painter->setPen(QPen(Qt::blue,2));
	}
	painter->drawLine(line());	
	painter->drawPolygon(arrowHead);
	
}
QVariant Arrow::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) 
	{
		return value;
    }
	if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
		return value;
	}
	return value;
}
void Arrow::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	int count = 0;
	if (mouseEvent->button() == Qt::RightButton)
	{
		scene()->clearSelection();
		setSelected(true);
		count = scene()->selectedItems().count();
	}
	else if(mouseEvent->button() == Qt::LeftButton)
	{
		count = scene()->selectedItems().count();
		QGraphicsItem::mousePressEvent(mouseEvent);
	}
}
void Arrow::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	//int count = 0;
	//if (mouseEvent->button() == Qt::RightButton)
	//{
	//	//scene()->clearSelection();
	//	//setSelected(true);
	//	count = scene()->selectedItems().count();
	//}
	//else if(mouseEvent->button() == Qt::LeftButton)
	//{
	//	count = scene()->selectedItems().count();
	//}
	QGraphicsItem::mouseReleaseEvent(mouseEvent);
}
void Arrow::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseDoubleClickEvent(event);
}