
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "diagramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class iData;
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(DiagramItem *startItem, DiagramItem *endItem,iData* data,QMenu *contextMenu = 0,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
	iData* myData();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    DiagramItem *startItem() const
        { return myStartItem; }
    DiagramItem *endItem() const
        { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

private:
    DiagramItem*	myStartItem;
    DiagramItem*	myEndItem;
    QColor			myColor;
    QPolygonF		arrowHead;
	QMenu *			myContextMenu;
};


#endif
