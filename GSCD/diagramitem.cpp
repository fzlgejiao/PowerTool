
#include <QtGui>

#include "diagramitem.h"
#include "arrow.h"
#include "idata.h"
#include "diagramscene.h"

//! [0]
DiagramItem::DiagramItem(iData* data, QGraphicsItem *parent, QGraphicsScene *scene)
  //  : QGraphicsPolygonItem(parent, scene)
   : QGraphicsPathItem(parent, scene)
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
   /*  pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);	*/
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
	return 	QGraphicsPathItem::boundingRect() | childrenBoundingRect() | myPath.boundingRect();
}
QPainterPath DiagramItem::shape() const
{
	return myPath;
}
void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QStyleOptionGraphicsItem op(*option);
	//op.initFrom(widget);

	// set state to State_None when selected
	if (option->state & QStyle::State_Selected)
	{
		setPen(QPen(Qt::blue,1));		
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
			setPen(QPen(Qt::green,1));
		else
			setPen(QPen(Qt::darkCyan,1));
	}

	// call default func to draw
	QGraphicsPathItem::paint(painter, &op, widget);	
}
void DiagramItem::updateData()
{
	iSTAT* stat = (iSTAT *)myData();
	if(!stat)
		return;
	//myPolygon.clear();
		
    myPath=QPainterPath(QPointF(0,0));	
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
		case STAT_HYDROPLANT:
			{
			//myPolygon <<QPointF(-15, 10)<<QPointF(-15, -10)<<QPointF(15, -10)<<QPointF(15, 10)<<QPointF(-15, 10)<<QPointF(15, -10);			
			myPath.addRect(-15,-10,30,20);	
			myPath.moveTo(-15,10);
			myPath.lineTo(15,-10);
			}
			break;
		case STAT_THERMALPLANT:
			{
			//myPolygon <<QPointF(-15, 0)<<QPointF(-15, 10)<<QPointF(15, 10)<<QPointF(15, -10)<<QPointF(-15, -10)<<QPointF(-15, 0)<<QPointF(15, 0);
			myPath.addRect(-15,-10,30,20);
			myPath.moveTo(-15,0);
			myPath.lineTo(15,0);
			}
			break;
		case STAT_PUMPEDSTORAGEPLANT:
			{
			//myPolygon <<QPointF(-15, 0)<<QPointF(-15, 10)<<QPointF(15, 10)<<QPointF(15, -10)<<QPointF(-15, -10)<<QPointF(-15, 0)<<QPointF(15, 0);
			myPath.addRect(-15,-10,30,20);
			//myPath.moveTo(-15,-3);
			//myPath.lineTo(0,-3);
			//myPath.lineTo(0,10);
			//myPath.moveTo(0,3);
			//myPath.lineTo(15,3);
			myPath.moveTo(-15,-3);
			myPath.addRect(-15,-3,15,13);
			myPath.moveTo(0,3);
			myPath.addRect(0,3,15,7);
			}
			break;
		case STAT_NUCLEARPLANT:
			{					
			myPath.addRect(-15,-10,30,20);		
			myPath.addEllipse(QPointF(0,0),6,6);			
			//myPolygon = path.toFillPolygon();			
			}
			break;
		case STAT_110KV:
			{			 
			 myPath.addEllipse(QPointF(0,0),9,9);			 
			 //myPolygon = path.toFillPolygon();
			}
			break;
        case STAT_220KV:
			{			
			 myPath.addEllipse(QPointF(0,0),7,7);				
			 myPath.addEllipse(QPointF(0,0),11,11);			
			// myPolygon = path.toFillPolygon();
			}
            break;
		case STAT_330KV:
		case STAT_550KV:
           {			
			 myPath.addEllipse(QPointF(0,0),7,7);	
			 myPath.addEllipse(QPointF(0,0),11,11);
			 myPath.addEllipse(QPointF(0,0),15,15);			 
			 //myPolygon = path.toFillPolygon();
			}
            break;
		case STAT_SUBSYSTEM1:
			{			
			 myPath.addEllipse(QPointF(0,0),25,25);	
			 myPath.addEllipse(QPointF(0,0),30,30);				 
			}
			break;
		case STAT_SUBSYSTEM2:
			{			
			 myPath.addEllipse(QPointF(0,0),30,30);	
			 myPath.addEllipse(QPointF(0,0),35,35);				
			}
			break;
		case STAT_SUBSYSTEM3:
			{			
			 myPath.addEllipse(QPointF(0,0),35,35);	
			 myPath.addEllipse(QPointF(0,0),40,40);				 
			}
			break;
		case STAT_T_STATION:
			{			
			 myPath.addEllipse(QPointF(0,0),4,4);				
			}
			break;
		case STAT_SERIESCOMPENSATION:
			{			
			 myPath.addEllipse(QPointF(0,0),16,16);
			 myPath.moveTo(-16,0);
			 myPath.lineTo(-5,0);
			 myPath.moveTo(-5,-9);
			 myPath.lineTo(-5,9);
			 myPath.moveTo(5,-9);
			 myPath.lineTo(5,9);
			 myPath.moveTo(5,0);
			 myPath.lineTo(16,0);
			}
			break;
		case STAT_CONVERTER:
			{			
			 myPath.addEllipse(QPointF(0,0),16,16);		
			 myPath.moveTo(-11,-11);
			 myPath.lineTo(11,11);
			 myPath.moveTo(-7,7);
			 myPath.lineTo(3,7);

			 myPath.moveTo(-1,-5);
			 myPath.arcTo(-1,-8,6,6,180,-180);
			 myPath.moveTo(11,-5);
			 myPath.arcTo(5,-8,6,6,0,-180);		
			}
			break;
		case STAT_SWITCHING:
			{			
			 myPath.addEllipse(QPointF(0,0),13,13);		
			 myPath.moveTo(-9,9);
			 myPath.lineTo(9,-9);
			}
			break;		
        default:
            /*myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);*/
			myPath.addRect(-50,-50,100,100);
			
            break;
    }
	myPath.setFillRule(Qt::WindingFill);
    setPath(myPath);
}