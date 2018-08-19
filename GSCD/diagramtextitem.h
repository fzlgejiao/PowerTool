
#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class iData;
class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    DiagramTextItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~DiagramTextItem();

    int type() const
        { return Type; }

	void setDefaultPos(const QPointF& pos){defPos = pos;}
	void backToDefaultPos(){setPos(defPos);}


signals:
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QPointF		defPos;
};

#endif
