

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "idata.h"
QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;
class iData;
//class DiagramItem : public QGraphicsPolygonItem
class DiagramItem : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 15 };

    DiagramItem(iData* data, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeArrow(Arrow *arrow);
    void removeArrows();

  //  QPainterPath path () const
    //    { return myPath;}

    void addArrow(Arrow *arrow);
	QList<Arrow *>&  getArows(){return arrows;}
    QPixmap image() const;
    int type() const
		{ return Type;}
	iData* myData();
	void updateData();

protected:
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	QRectF	boundingRect () const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
   // QPolygonF		myPolygon;
	QPainterPath	myPath;
    QList<Arrow *>	arrows;		
	
signals:

	
};


#endif
