
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
   if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        if (value == true)
        {
            // do stuff if selected
			foreach(QGraphicsItem *item,childItems())
			{
				DiagramTextItem* txtItem = qgraphicsitem_cast<DiagramTextItem *>(item);
				if(txtItem)
				{
					//txtItem->setPen(QPen(Qt::green,1));
					txtItem->setDefaultTextColor(QColor("green"));
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
	return 	QGraphicsPathItem::boundingRect() | childrenBoundingRect() | myPath.boundingRect();
}
QPainterPath DiagramItem::shape() const
{
	return myPath;
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
		case STAT_HYDROPOWER:
			{
			//myPolygon <<QPointF(-15, 10)<<QPointF(-15, -10)<<QPointF(15, -10)<<QPointF(15, 10)<<QPointF(-15, 10)<<QPointF(15, -10);			
			myPath.addRect(-15,-10,30,20);	
			myPath.moveTo(-15,10);
			myPath.lineTo(15,-10);
			}
			break;
		case STAT_THERMALPOWER:
			{
			//myPolygon <<QPointF(-15, 0)<<QPointF(-15, 10)<<QPointF(15, 10)<<QPointF(15, -10)<<QPointF(-15, -10)<<QPointF(-15, 0)<<QPointF(15, 0);
			myPath.addRect(-15,-10,30,20);
			myPath.moveTo(-15,0);
			myPath.lineTo(15,0);
			}
			break;
		case STAT_PUMPEDSTORAGEPOWER:
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
		case STAT_NUCLEARPOWER:
			{					
			myPath.addRect(-15,-10,30,20);		
			myPath.addEllipse(QPointF(0,0),8,8);			
			//myPolygon = path.toFillPolygon();			
			}
			break;
		case STAT_110KV:
			{			 
			 myPath.addEllipse(QPointF(0,0),15,15);			 
			 //myPolygon = path.toFillPolygon();
			}
			break;
        case STAT_220KV:
			{			
			 myPath.addEllipse(QPointF(0,0),15,15);				
			 myPath.addEllipse(QPointF(0,0),20,20);			
			// myPolygon = path.toFillPolygon();
			}
            break;
		case STAT_330KV:
		case STAT_550KV:
           {			
			 myPath.addEllipse(QPointF(0,0),15,15);	
			 myPath.addEllipse(QPointF(0,0),20,20);
			 myPath.addEllipse(QPointF(0,0),25,25);			 
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
		case STAT_T_NODE:
			{			
			 myPath.addEllipse(QPointF(0,0),5,5);				
			}
			break;
		case STAT_SERIESCOMPENSATION:
			{			
			 myPath.addEllipse(QPointF(0,0),25,25);
			 myPath.moveTo(-25,0);
			 myPath.lineTo(-8,0);
			 myPath.moveTo(-8,-15);
			 myPath.lineTo(-8,15);
			 myPath.moveTo(8,-15);
			 myPath.lineTo(8,15);
			 myPath.moveTo(8,0);
			 myPath.lineTo(25,0);
			}
			break;
		case STAT_CONVERTOR:
			{			
			 myPath.addEllipse(QPointF(0,0),25,25);		
			 myPath.moveTo(-17,-17);
			 myPath.lineTo(17,17);
			 myPath.moveTo(-14,10);
			 myPath.lineTo(3,10);

			 myPath.moveTo(-5,-10);
			 myPath.arcTo(-5,-15,10,10,180,-180);
			 myPath.arcTo(5,-15,10,10,180,180);		
			}
			break;
		case STAT_SWITCHING:
			{			
			 myPath.addEllipse(QPointF(0,0),20,20);		
			 myPath.moveTo(-14,14);
			 myPath.lineTo(14,-14);
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