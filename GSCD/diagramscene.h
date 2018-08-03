
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "diagramtextitem.h"
#include "idata.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class iBUS;
class iBRANCH;
class iTRANSFORMER;
QT_END_NAMESPACE

class iDoc;
class iData;
//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    DiagramScene(iDoc* doc,QObject *parent = 0);

    QFont font() const
        { return myFont; }
    QColor textColor() const
        { return myTextColor; }
    QColor itemColor() const
        { return myItemColor; }
    QColor lineColor() const
        { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

	void addMenu(T_DATA type,QMenu *menu){myMenus.insert(type,menu);}
	QMenu* getMenu(T_DATA type){return myMenus.value(type,NULL);}

public slots:
    void editorLostFocus(DiagramTextItem *item);

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
	void itemDBClicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

	void addSTAT(iSTAT* stat,const QPointF& pos);

private:
    bool isItemChange(int type);

    Mode		myMode;
	iDoc*		myDoc;
	QMap<T_DATA,QMenu *> myMenus;

    bool		leftButtonDown;
    QPointF		startPoint;
    QGraphicsLineItem *line;
    QFont		myFont;
    DiagramTextItem *textItem;
    QColor		myTextColor;
    QColor		myItemColor;
    QColor		myLineColor;
};
//! [0]

#endif
