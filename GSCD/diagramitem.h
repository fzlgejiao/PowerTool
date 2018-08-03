

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;
class iData;
class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };

    DiagramItem(iData* data, QMenu *contextMenu = 0,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeArrow(Arrow *arrow);
    void removeArrows();

    QPolygonF polygon() const
        { return myPolygon; }
    void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const
		{ return Type;}
	iData* myData();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPolygonF		myPolygon;
    QMenu *			myContextMenu;
    QList<Arrow *>	arrows;

signals:

};


#endif
