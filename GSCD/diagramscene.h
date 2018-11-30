
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "diagramtextitem.h"
#include "idata.h"

//action type
typedef enum{
	ACT_NONE			= 0,
	ACT_VIEW,
	ACT_EDIT,
	ACT_DELETE,
}ACT_TYPE;


QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QAction;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsSimpleTextItem;
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

	void addMenu(MENU_TYPE type,QMenu *menu){myMenus.insert(type,menu);}
	QMenu* getMenu(MENU_TYPE type){return myMenus.value(type,NULL);}
	
	

	void viewItem();																				//to deal with view property event from main menu
	void editItem();																				//to deal with edit object event from main menu
	void deleteItems();																				//to deal with delete items event from main menu
	
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);	
	void drawBackground ( QPainter * painter, const QRectF & rect );
	void procItem(ACT_TYPE act,QGraphicsItem* item);

	//actions for station
	void editStation(DiagramItem *item,iSTAT* stat);
	void viewStation(DiagramItem *item,iSTAT* stat);
	void deleteStation(DiagramItem *item,iSTAT* stat);

	void viewStationName(DiagramTextItem *item,iSTAT* stat);
	void viewStationValue(DiagramTextItem *item,iSTAT* stat);

	//actions for station link and arrows
	void viewSLink(iSLINK* slink);																//for 'property' of branch menu item
	void editSLink(iSLINK* slink);																//for 'edit link' menu item
	void updateArrows(iSTAT* stat);

	//actions for note
	void addNote(const QPointF& pos);
	void viewNote(iNote *note);
	void deleteNote(iNote *note);

private:
    bool isItemChange(int type);

	MainWindow* pMain;
	iDoc*		myDoc;
	QMap<MENU_TYPE,QMenu *> myMenus;
	QMenu*		sceneMenu;
	QMenu*		multiMenu;
	QMenu*		statMenu;
	QMenu*		statValueMenu;
	QMenu*		statNameMenu;
	QMenu*		statLinkMenu;
	QMenu*		noteMenu;

	QAction*	propertyAction;
    QAction*	deleteAction;
	QAction*	editStationAction;
	QAction*	editSLinkAction;
	QAction*	defPositionAction;

    bool		leftButtonDown;
    QPointF		startPoint;
    QGraphicsLineItem *line;
    QFont		myFont;
    DiagramTextItem *textItem;
    QColor		myTextColor;
    QColor		myItemColor;
    QColor		myLineColor;

	

public slots:
	void addStation(iSTAT* stat,const QPointF& posStat,const QFont& fontName,QPointF& posName = QPointF(10,10),QPointF& posValue = QPointF(10,-20));


signals:
	void modeDone();
};
//! [0]

#endif
