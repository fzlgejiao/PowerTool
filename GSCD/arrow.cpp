
#include <QtGui>

#include "arrow.h"
#include <math.h>
#include "idata.h"
#include "diagramscene.h"

#define LINE_HALF_GAP		3
#define ARROW_SIZE		   15

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem,iSLINK* slink,int groupId,
	QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsLineItem(parent, scene)
{
	setData(ITEM_DATA,(uint)slink);
	myGroupId	= groupId;
	myStartItem = startItem;
	myEndItem	= endItem;
	myTextItem  = NULL;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	myColor = Qt::darkCyan;
	setPen(QPen(myColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
Arrow::~Arrow()
{
	if(myTextItem)
		delete myTextItem;
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

	QRectF rect = arrowLine.boundingRect();

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra)| arrowLine.boundingRect() | arrowHead.boundingRect() | childrenBoundingRect();

	//return arrowLine.boundingRect() | arrowHead.boundingRect();
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
	QPainterPath path = QGraphicsLineItem::shape();
	path.addPolygon(arrowHead);
	path.addPolygon(arrowLine);
	return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
	QPointF startPoint	=myStartItem->scenePos();
	QPointF endPoint	=myEndItem->scenePos();
	QPointF centerPoint= QPointF((endPoint.x() + startPoint.x()) / 2.0, (startPoint.y() + endPoint.y()) / 2.0);
	setPos(centerPoint);
	QPointF pt = this->pos();

	//QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
	qreal dx = endPoint.x() - startPoint.x();
	qreal dy = endPoint.y() - startPoint.y();
	QLineF line(-dx/2,-dy/2,dx/2,dy/2);
	setLine(line);

	//rotate line text item
	if(myTextItem)
	{
		float degree = ::atanf((endPoint.y() - startPoint.y())/(endPoint.x() - startPoint.x()));
		myTextItem->setRotation(degree*180/M_PI);
		//qDebug(qPrintable(QString("degree::%1").arg(degree,4,'f',1)));
	}

}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *op,QWidget *widget)
{
	if (myStartItem->collidesWithItem(myEndItem))
		return;

	//QGraphicsLineItem::paint(painter,op,widget);
	//return;
	
	QPen myPen = pen();
	myPen.setColor(myColor);
	qreal arrowSize = ARROW_SIZE;
	painter->setPen(myPen);
	painter->setBrush(Qt::white);
	//! [4] //! [5]

	QPointF startPoint=myStartItem->scenePos();
	QPointF endPoint=myEndItem->scenePos();	
	QLineF centerLine(startPoint, endPoint);
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


	//setLine(QLineF(endPoint,startPoint));
	//! [5] //! [6]

	QPointF middlepoint= QPointF((endPoint.x() + startPoint.x()) / 2.0, (startPoint.y() + endPoint.y()) / 2.0) - pos();

	double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (M_PI * 2) - angle;

	QPointF arrowP1 = middlepoint + QPointF(sin(angle + M_PI / 3) * arrowSize,cos(angle + M_PI / 3) * arrowSize);
	QPointF arrowP2 = middlepoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,cos(angle + M_PI - M_PI / 3) * arrowSize);

	arrowHead.clear();
	//arrowHead << line().p1() << arrowP1 << arrowP2;	
	arrowHead << middlepoint << arrowP1 << arrowP2;
	//! [6] //! [7]
	QPen oldPen = pen();
	if (isSelected())
		oldPen.setColor(Qt::blue);
	else
		oldPen.setColor(myColor);
	painter->setPen(oldPen);
	QLineF line1 = line();
	//painter->drawLine(line());

	//draw lines
	iSLINK* slink = (iSLINK *)myData();
	if(!slink)
		return;
	arrowLine.clear();
	int nLines = slink->groupLineCount(1);															//just deal with group=1
    qreal radAngle = centerLine.angle()* M_PI / 180;
	for(int i=0;i<nLines;i++)
	{
		qreal dx = (nLines-1-i*2) * LINE_HALF_GAP * sin(radAngle);
		qreal dy = (nLines-1-i*2) * LINE_HALF_GAP * cos(radAngle);
		QPointF offset = QPointF(dx, dy);
		QLineF  xline;
		xline.setP1(centerLine.p1() + offset - pos());
		xline.setP2(centerLine.p2() + offset - pos());
		if(i == 0)
		{
			arrowLine.append(xline.p1());
			arrowLine.append(xline.p2());
		}
		if(i == nLines - 1)
		{
			arrowLine.append(xline.p2());
			arrowLine.append(xline.p1());
		}
		painter->drawLine(xline);
	}

	//draw arrow head
	painter->drawPolygon(arrowHead);

	//if (isSelected()) 
	//{
	//	painter->setPen(QPen(myColor, 1, Qt::DashLine));
	//	QLineF myLine = line();
	//	myLine.translate(4, 4.0);
	//	painter->drawLine(myLine);
	//	myLine.translate(4,-8.0);
	//	painter->drawLine(myLine);
	//}
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
		mouseEvent->accept();
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