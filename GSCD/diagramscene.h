
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "diagramtextitem.h"
#include "idata.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QAction;
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
class MainWindow;
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
//    enum Mode { M_MoveItem, M_AddStation, M_AddNote,InsertItem, InsertLine, InsertText, MoveItem };

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
	void deleteItems();																				//to deal with delete event from main menu
	
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	void addStation(const QPointF& pos);
	void addStationItem(iSTAT* stat,const QPointF& pos);

private:
    bool isItemChange(int type);

	MainWindow* pMain;
	iDoc*		myDoc;
	QMap<T_DATA,QMenu *> myMenus;
	QMenu*		sceneMenu;
	QMenu*		multiMenu;
	QMenu*		statMenu;

	QAction*	propertyAction;
    QAction*	deleteAction;
	QAction*	editStationAction;
	QAction*	editBranchAction;

    bool		leftButtonDown;
    QPointF		startPoint;
    QGraphicsLineItem *line;
    QFont		myFont;
    DiagramTextItem *textItem;
    QColor		myTextColor;
    QColor		myItemColor;
    QColor		myLineColor;

public slots:
    void editorLostFocus(DiagramTextItem *item);
	void viewProperty();																			//for 'property' menu item
	void editStation();																				//for 'edit station' menu item
	void editBranch();																				//for 'edit branch' menu item
	void deleteItem();																				//for 'delete' menu item

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
	void modeDone();
};
//! [0]

#endif
