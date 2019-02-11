
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "diagramitem.h"
#include "diagramtextitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class iSLINK;
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(DiagramItem *startItem, DiagramItem *endItem,iSLINK* slink,int groupId,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~Arrow();

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
	void setTextItem(DiagramTextItem* item){myTextItem = item;}
	DiagramTextItem *textItem(){return myTextItem;}

	void setshowArrow(bool ishow){m_isarrowshow=ishow;}

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
	void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent );
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    DiagramItem*	myStartItem;
    DiagramItem*	myEndItem;
	DiagramTextItem* myTextItem;
    QColor			myColor;
    QPolygonF		arrowHead;
	QPolygonF		arrowLine;
	int				myGroupId;
	bool			m_isarrowshow;
};


#endif
