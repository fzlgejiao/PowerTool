
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
	m_linewidth=3;
	setPen(QPen(myColor, m_linewidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	m_isarrowshow=true;
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
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (myStartItem->collidesWithItem(myEndItem))
		return;
	double totalPowerActive=0;
	double totalPowerReactive=0;
	QStyleOptionGraphicsItem op(*option);

	QPen myPen = pen();
	// set state to State_None when selected
	if (option->state & QStyle::State_Selected)
	{
		myPen.setColor(Qt::blue);
		op.state = QStyle::State_None;
	}
	else
	{
		bool bChildSelected = false;
		foreach(QGraphicsItem *item,childItems())
		{
			DiagramTextItem* txtItem = qgraphicsitem_cast<DiagramTextItem *>(item);
			if(txtItem && txtItem->isSelected())
				bChildSelected = true;
		}
		if(bChildSelected)
			myPen.setColor(Qt::green);
		else
			myPen.setColor(myColor);
	}	
	
	myPen.setWidth(m_linewidth);
	painter->setBrush(Qt::white);


	QPointF startPoint	=myStartItem->scenePos();
	QPointF endPoint	=myEndItem->scenePos();	
	QLineF	centerLine(startPoint, endPoint);


	//draw lines
	iSLINK* slink = (iSLINK *)myData();
	if(!slink)
		return;
	arrowLine.clear();
	int nLines = slink->groupLineCount(1);															//just deal with group=1
    qreal radAngle = centerLine.angle()* M_PI / 180;
	for(int i=0;i<nLines;i++)
	{
		iLinkData *link=slink->groupLinkDatas(1)[i];								//just for Group 1
		if(!link) continue;
		totalPowerActive+=link->P1_active();
		totalPowerReactive+=link->Q1_reactive();
		if(link->type()==T_BRANCH)
		{
			iBRANCH *branch=(iBRANCH *)link;
			if(branch->onService())
				myPen.setStyle(Qt::SolidLine);
			else
				myPen.setStyle(Qt::DashLine);
		}	
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
		painter->setPen(myPen);
		painter->drawLine(xline);
	}

	//draw arrow head
	QPointF middlepoint= QPointF((endPoint.x() + startPoint.x()) / 2.0, (startPoint.y() + endPoint.y()) / 2.0) - pos();

	double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (M_PI * 2) - angle;

	QPointF arrowP1,arrowP2;
	if(totalPowerActive<0)
	{
		arrowP1 = middlepoint + QPointF(sin(angle + M_PI / 3) * ARROW_SIZE,cos(angle + M_PI / 3) * ARROW_SIZE);
		arrowP2 = middlepoint + QPointF(sin(angle + M_PI - M_PI / 3) * ARROW_SIZE,cos(angle + M_PI - M_PI / 3) * ARROW_SIZE);
	}else
	{
		arrowP1 = middlepoint - QPointF(sin(angle + M_PI / 3) * ARROW_SIZE,cos(angle + M_PI / 3) * ARROW_SIZE);
		arrowP2 = middlepoint - QPointF(sin(angle + M_PI - M_PI / 3) * ARROW_SIZE,cos(angle + M_PI - M_PI / 3) * ARROW_SIZE);
	}
	if((totalPowerActive!=0) || (totalPowerReactive!=0))
	{
		if(m_isarrowshow)
		{
			arrowHead.clear();
			arrowHead << middlepoint << arrowP1 << arrowP2;
			myPen.setStyle(Qt::SolidLine);
			painter->setPen(myPen);
			painter->drawPolygon(arrowHead);
		}
	}
	//QGraphicsLineItem::paint(painter, &op, widget);	

}
QVariant Arrow::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) 
	{
    }
	if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
		foreach(QGraphicsItem *item,childItems())
		{
			DiagramTextItem* txtItem = qgraphicsitem_cast<DiagramTextItem *>(item);
			if(txtItem)
				txtItem->update();
		}
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
