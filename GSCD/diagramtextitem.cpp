
#include <QtGui>

#include "diagramtextitem.h"
#include "diagramscene.h"
#include "arrow.h"

//! [0]
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    //setFlag(QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	//setCacheMode(QGraphicsItem::ItemCoordinateCache);
	//setCacheMode(QGraphicsItem::DeviceCoordinateCache);
		
	defPos = QPointF(10,10);
	m_nScale = 1;

	connect(scene, SIGNAL(allDefPositioned()),	this, SLOT(backToDefaultPos()));
}
//! [0]

DiagramTextItem::~DiagramTextItem()
{
}
//! [1]
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);

    return value;
}
//! [1]


void DiagramTextItem::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	if (mouseEvent->button() == Qt::RightButton)
	{
		scene()->clearSelection();
		setSelected(true);
	}
	QGraphicsTextItem::mousePressEvent(mouseEvent);
}
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void DiagramTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QStyleOptionGraphicsItem op(*option);
	//op.initFrom(widget);

	// set state to State_None when selected
	if (option->state & QStyle::State_Selected)
	{
		setDefaultTextColor(Qt::blue);
		op.state = QStyle::State_None;
	}
	else
	{
		DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(parentItem());
		Arrow *arrow = qgraphicsitem_cast<Arrow *>(parentItem());
		if((statItem && statItem->isSelected())
			||(arrow && arrow->isSelected()))
			setDefaultTextColor(Qt::green);
		else
			setDefaultTextColor(Qt::darkCyan);
	}
	// call default func to draw
	QGraphicsTextItem::paint(painter, &op, widget);
}
