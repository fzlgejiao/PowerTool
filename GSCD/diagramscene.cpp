
#include <QtGui>
#include <QtCore/qmath.h>
#include "diagramscene.h"
#include "arrow.h"
#include "idoc.h"
#include "adddialog.h"
#include "mainwindow.h"
#include "stationparameterdialog.h"
#include "textdialog.h"
#include "stationnamedialog.h"
#include "stationvaluedialog.h"
#include "brancheditdialog.h"
#include "diagramnoteitem.h"
//! [0]
DiagramScene::DiagramScene(iDoc* doc,QObject *parent)
    : QGraphicsScene(parent)
{
	pMain		= (MainWindow *)parent;
	myDoc		= doc;

    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::darkCyan;
	myLineColor = Qt::darkCyan;

	myFont =  QFont("Times New Roman", 12, QFont::Bold);
	//myFont.setWeight(QFont::Bold);
	setItemIndexMethod(NoIndex);																	//to fix bug of Qt: QTBUG-18021: Crash in QGraphicsSceneFindItemBspTreeVisitor::visit(QList<QGraphicsItem*>*) when delete items

	//context menus
	propertyAction = new QAction(tr("&Properties..."), this);
	propertyAction->setStatusTip(tr("Show object property"));

	editStationAction = new QAction(tr("&Edit Station..."), this);
	editStationAction->setStatusTip(tr("Edit station"));

	editSLinkAction = new QAction(tr("&Edit link..."), this);
	editSLinkAction->setStatusTip(tr("Edit link"));

	deleteAction = new QAction(QIcon(":/images/delete.png"),tr("&Delete"), this);
	deleteAction->setStatusTip(tr("Delete item from diagram"));

	defPositionAction = new QAction(tr("&Return to default position"), this);
	defPositionAction->setStatusTip(tr("Return to default position"));

	sceneMenu=new QMenu();
	//sceneMenu->addAction("Scaling...");
	//sceneMenu->addAction("Select all Stations");
	//sceneMenu->addAction("Return labels to default position");
	//sceneMenu->addAction("Map properties...");
	//sceneMenu->addAction("Image area...");
	//sceneMenu->addAction("Font...");

	multiMenu=new QMenu();
	multiMenu->addAction("multi-select1");
	multiMenu->addAction("multi-select2");


	statMenu=new QMenu();
	statMenu->addAction(propertyAction);
	statMenu->addAction(editStationAction);
	statMenu->addSeparator();
	statMenu->addAction(deleteAction);

	statValueMenu=new QMenu();
	statValueMenu->addAction(propertyAction);
	statValueMenu->addAction(defPositionAction);

	statNameMenu=new QMenu();
	statNameMenu->addAction(propertyAction);
	statNameMenu->addAction(defPositionAction);

	statLinkMenu=new QMenu();
	statLinkMenu->addAction(propertyAction);
	statLinkMenu->addAction(editSLinkAction);

	noteMenu=new QMenu();
	noteMenu->addAction(propertyAction);
	noteMenu->addAction(deleteAction);

	//add menus to scene and items
	addMenu(MENU_SCENE,sceneMenu);
	addMenu(MENU_STAT,statMenu);
	addMenu(MENU_STAT_NAME,statNameMenu);
	addMenu(MENU_STAT_VALUE,statValueMenu);
	addMenu(MENU_STAT_LINK,statLinkMenu);
	addMenu(MENU_NOTE,noteMenu);

	connect(doc, SIGNAL(statAdded(iSTAT*,const QPointF&,const QFont&,QPointF&,QPointF&)),	this, SLOT(addStation(iSTAT*,const QPointF&,const QFont&,QPointF&,QPointF&)));
	connect(doc, SIGNAL(noteAdded(iNote*,const QPointF&,const QSizeF&)),	this, SLOT(addNote(iNote*,const QPointF&,const QSizeF&)));
	
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item =
            qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item =
            qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]


//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (pMain->mode() == M_MoveItem) 
	{
    }   
	mouseEvent->accept();
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
   if (mouseEvent->button() == Qt::LeftButton)
	{
		switch (pMain->mode()) 
		{
		case M_AddStation:
			{
				QList<iNodeData *> Nodelist;
				myDoc->getAvailableNode(Nodelist);

				if(Nodelist.count())
				{
					AddDialog dlg(myDoc,NULL,pMain);
					if(dlg.exec()!=QDialog::Accepted)
						break;

					if(!dlg.IsAddSite()) 
						break ;
					//add new station data object			
					iSTAT* stat= myDoc->STAT_new(myDoc->STAT_getId(),dlg.NewStationName());												//create a new station object
					if(stat)
					{	
						QList<iNodeData *> addednodes=dlg.GetAddedNodes();
						stat->setNodes(addednodes);
						stat->setsType(dlg.getstationtype());

						addStation(stat,mouseEvent->scenePos(),dlg.GetFont());
					}
				}

				
			}
			break;

		case M_AddNote:
			{	
				TextDialog dlg(NULL,pMain);
				if(dlg.exec()!=QDialog::Accepted) 
					break ;
	
				if(dlg.GetText().isEmpty()) 
					break ;

				iNote* note= myDoc->Note_new(dlg.GetText());
				if(note) 
				{
					note->setTextFont(dlg.GetFont());
					note->setTextColor(dlg.GetTextcolor());
					note->setAlignmode(dlg.GetAlignmode());
					note->setborder(dlg.HasBorder());
					addNote(note,mouseEvent->scenePos(),QSizeF(50,50));				
				}
			}
			break;
		}
	}

	emit modeDone();

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton)
	{
	QList<QGraphicsItem *> &list = selectedItems();
	if(list.count() <= 1)																			//none-selected
	{	
		viewItem();	
	}
	}
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);								
}
void DiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	qreal     x = event->scenePos().x();
	qreal     y = event->scenePos().y();

	QList<QGraphicsItem *> &list = selectedItems();
	if(list.count() == 0)																			//none-selected
	{	
		if(sceneMenu)
			sceneMenu->exec(event->screenPos());
	}
	else if(list.count() > 1)																		//multi-selected
	{
		if(multiMenu)
			multiMenu->exec(event->screenPos());

	}
	else																							//one-selected
	{
		QGraphicsItem* item = qgraphicsitem_cast<QGraphicsItem *>(selectedItems().first());//itemAt( event->scenePos());
		if(!item)
			return;
		iData* data = (iData *)item->data(ITEM_DATA).toUInt();
		if(!data)
			return;
		switch(data->type())																		
		{
		case T_STAT:																				//station 
			{
				iSTAT* stat = (iSTAT *)data;
				if(item->type() == DiagramItem::Type)												//station item									
				{
					DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
					QMenu* menu = getMenu(MENU_STAT);
					if(!menu)
						return;
					QAction* action = menu->exec(event->screenPos());
					if(action == propertyAction)
						viewStation(statItem,stat);
					else if(action == editStationAction)
						editStation(statItem,stat);
					else if(action == deleteAction)
						deleteStation(statItem,stat);

				}
				else if(item->type() == DiagramTextItem::Type)										//station value/name item
				{
					DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);
					if(item == stat->itemName())
					{
						QMenu* menu = getMenu(MENU_STAT_NAME);
						if(!menu)
							return;
						QAction* action = menu->exec(event->screenPos());
						if(action == propertyAction)
							viewStationName(textItem,stat);
						else if(action == defPositionAction)
							textItem->backToDefaultPos();
					}
					else if(item == stat->itemValue())
					{
						QMenu* menu = getMenu(MENU_STAT_VALUE);
						if(!menu)
							return;
						QAction* action = menu->exec(event->screenPos());
						if(action == propertyAction)
							viewStationValue(textItem,stat);
						else if(action == defPositionAction)
							textItem->backToDefaultPos();
					}
				}
			}
			break;
		case T_SLINK:
			{
				iSLINK* slink = (iSLINK *)data;
				if(item->type() == Arrow::Type)														//arrow item
				{
					QMenu* menu = getMenu(MENU_STAT_LINK);
					if(!menu)
						return;
					QAction* action = menu->exec(event->screenPos());
					if(action == propertyAction)
						viewSLink(slink);
					else if(action == editSLinkAction)
						editSLink(slink);
				}
				else if(item->type() == DiagramTextItem::Type)										//arrow value item
				{
					DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);
					QMenu* menu = getMenu(MENU_STAT_NAME);
					if(!menu)
						return;
					propertyAction->setEnabled(false);
					QAction* action = menu->exec(event->screenPos());
					if(action == defPositionAction)
						textItem->backToDefaultPos();
					propertyAction->setEnabled(true);
				}
			}
			break;
		case T_NOTE:
			{
				if(item->type() == DiagramNoteItem::Type)	
				{
					DiagramNoteItem *noteitem = qgraphicsitem_cast<DiagramNoteItem *>(item);
					iNote *note=(iNote *)data;
					QMenu* menu = getMenu(MENU_NOTE);
					if(!menu)	return;
					QAction* action = menu->exec(event->screenPos());
					if(action==propertyAction)
						viewNote(note);
					else if(action==deleteAction)
						deleteNote(noteitem,note);
				}
			}
			break;
		}
	}
	QGraphicsScene::contextMenuEvent(event);
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
void DiagramScene::viewItem()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;

	procItem(ACT_VIEW,item);

}
void DiagramScene::editItem()
{
	if(selectedItems().isEmpty())
		return;
	QGraphicsItem* item = selectedItems().first();
	if(!item)
		return;

	procItem(ACT_EDIT,item);
}
void DiagramScene::deleteItems()
{
	foreach (QGraphicsItem *item, selectedItems()) 
	{
		iData* data = (iData *)item->data(ITEM_DATA).toUInt();
		if(!data)
			continue;
		if (data->type() == T_STAT && item->type() == DiagramItem::Type) 
		{
			DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
			 if(statItem)
				deleteStation(statItem,(iSTAT *)data);
		}else if(data->type()==T_NOTE)
		{
			DiagramNoteItem *noteitem = qgraphicsitem_cast<DiagramNoteItem *>(item);
			deleteNote(noteitem,(iNote*)data);
		}
	}
}
void DiagramScene::procItem(ACT_TYPE act,QGraphicsItem* item)
{
	iData* data = (iData *)item->data(ITEM_DATA).toUInt();
	if(!data)
		return;
	switch(data->type())																		
	{
	case T_STAT:																				//station 
		{
			iSTAT* stat = (iSTAT *)data;
			if(item->type() == DiagramItem::Type)												//station item									
			{
				DiagramItem *statItem = qgraphicsitem_cast<DiagramItem *>(item);
				if(act == ACT_VIEW)
					viewStation(statItem,stat);
				else if(act == ACT_EDIT)
					editStation(statItem,stat);
				else if(act == ACT_DELETE)
					deleteStation(statItem,stat);
			}
			else if(item->type() == DiagramTextItem::Type)										//station value/name item
			{
				DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);
				if(act == ACT_VIEW)
				{
					if(textItem == stat->itemName())
						viewStationName(textItem,stat);
					else
						viewStationValue(textItem,stat);
				}
			}
		}
		break;
	case T_SLINK:
		{
			if(item->type() == Arrow::Type)														//arrow item		
				viewSLink((iSLINK *)data);
		}
		break;
	case T_NOTE:
		{
			if(item->type() == DiagramNoteItem::Type)														//arrow item		
				viewNote((iNote *)data);
		}
		break;
	}
}
void DiagramScene::updateArrows(iSTAT* stat)
{
	//create station to station arrow item
	QList<iSLINK*> listSLINK;
	foreach(iNodeData* node,stat->nodeDatas())
	{
		if(!node)
			continue;

		//to check if both fromBus and toBus of the attached data to this bus are already added
		foreach(iLinkData* linkdata,node->linkDatas())
		{
			if(!linkdata)
				continue;

			int from = linkdata->fromUid();
			int to	 = linkdata->toUid();
			iNodeData* node1 = myDoc->getNode(from);
			iNodeData* node2 = myDoc->getNode(to);
			if(!node1 || !node2)
				continue;

			//the from and to node of this linkdata are both added to two different stations
			if(node1->statId() == 0 || node2->statId() == 0 || node1->statId() == node2->statId())
				continue;

			//get the from and to station data for this linkdata
			iSTAT* stat1 = myDoc->STAT_get(node1->statId());
			iSTAT* stat2 = myDoc->STAT_get(node2->statId());
			if(!stat1 || !stat2)
				continue;

			DiagramItem *startItem	= stat1->myItem();
			DiagramItem *endItem	= stat2->myItem();
			if(!startItem || !endItem)
				continue;

			iSLINK* slink = myDoc->SLINK_get(stat1,stat2);
			if(!slink)
			{
				slink = myDoc->SLINK_new(stat1,stat2);
				if(slink)
				{
					slink->setStartItem(startItem);
					slink->setEndItem(endItem);
					stat1->addSlink(slink);
					stat2->addSlink(slink);
					listSLINK.append(slink);														//newly created station link
				}
			}

			if(slink)
			{
				slink->addLinkData(1,linkdata);														//default groupid=1
			}
		}

	}

	foreach(iSLINK *slink,listSLINK)
	{
		int nGroups = slink->groupCount();
		for(int i=1;i<=nGroups;i++)
		{
			DiagramItem *startItem	= slink->startItem();
			DiagramItem *endItem	= slink->endItem();
			if(!startItem || !endItem)
				continue;

			//arrow item
			Arrow *arrow = new Arrow(startItem, endItem,slink,i,0,this);							//create arrow item for one line group
			arrow->setColor(myLineColor);
			startItem->addArrow(arrow);
			endItem->addArrow(arrow);
			arrow->setZValue(-1000.0);
			arrow->setVisible(myDoc->getControlPanel().isShowBranchLine);
			addItem(arrow);
			slink->setArrow(arrow);

			//arrow text item
			DiagramTextItem* linkvalueItem = new DiagramTextItem(arrow,this);			
			linkvalueItem->setPlainText(slink->linkvalue(myDoc->sBase(),myDoc->getControlPanel().unittype,myDoc->getControlPanel().isShowReactivePowerValue));
			linkvalueItem->setFont(myFont);
			//nameItem->setDefaultTextColor(Qt::red);
			//nameItem->setPos(QPointF(10,10));
			//nameItem->setDefaultPos(QPointF(10,10));
			linkvalueItem->setData(ITEM_DATA,(uint)slink);
			linkvalueItem->setVisible(myDoc->getControlPanel().isShowBranchValue);
			//nameItem->setZValue(-1000.0);
			addItem(linkvalueItem);	
			arrow->setTextItem(linkvalueItem);
			arrow->updatePosition();																//update arrow line position when add/edit station item 
		}
	}
}
void DiagramScene::addStation(iSTAT* stat,const QPointF& posStat,const QFont& fontName,QPointF& posName,QPointF& posValue)
{	
	connect(this,SIGNAL(applyNameFont2all(QFont &)),stat,SLOT(OnapplyNameFont2all(QFont &)));

	//create station diagram item
	DiagramItem *item = new DiagramItem(stat, 0,this);												//create diagram item for station
	item->setBrush(myItemColor);
	addItem(item);
	item->setPos(posStat);
	stat->setItem(item);																			//set item to station
	
	//create station name text item
	DiagramTextItem* nameItem = new DiagramTextItem(item,this);
	nameItem->setFont(fontName);
	nameItem->setPlainText(stat->nodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
	nameItem->setDefaultTextColor(Qt::red);
	nameItem->setPos(posName);
	nameItem->setDefaultPos(QPointF(10,10));
	nameItem->setData(ITEM_DATA,(uint)stat);	
	addItem(nameItem);
	nameItem->setVisible(myDoc->getControlPanel().isShowStationName);
	stat->setItemName(nameItem);

	//create station value text item
	DiagramTextItem* valueItem = new DiagramTextItem(item,this);
	valueItem->setFont(myFont);	
	valueItem->setPlainText(stat->value(myDoc->sBase(),myDoc->getControlPanel().unittype,myDoc->getControlPanel().isShowReactivePowerValue));
	valueItem->setDefaultTextColor(Qt::red);
	valueItem->setPos(posValue);
	valueItem->setDefaultPos(QPointF(10,-20));
	valueItem->setData(ITEM_DATA,(uint)stat);
	addItem(valueItem);
	valueItem->setVisible(myDoc->getControlPanel().isShowStationValue);
	stat->setItemValue(valueItem);

	//QGraphicsRectItem* itemRect = new QGraphicsRectItem(0,0,180,20,0,this);
	//itemRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
	//itemRect->setFlag(QGraphicsItem::ItemIsFocusable, true);
	//itemRect->setFlag(QGraphicsItem::ItemIsMovable);
	//itemRect->setVisible(false);
	//addItem(itemRect);
	//QGraphicsLineItem* itemLine = new QGraphicsLineItem(0,0,180,0,itemRect,this);
	//itemLine->setFlag(QGraphicsItem::ItemIsSelectable, true);
	//itemLine->setFlag(QGraphicsItem::ItemIsFocusable, true);
	//itemLine->setPos(QPointF(0,10));
	//addItem(itemLine);

	updateArrows(stat);																				//udpate station links and arrows

	item->setSelected(true);

}
void DiagramScene::editStation(DiagramItem *item,iSTAT* stat)
{	
	AddDialog dlg(myDoc,stat,pMain);
	if(dlg.exec()!=QDialog::Accepted)
		return;		

	//update station type icon
	if(dlg.changes() & CHG_STAT_STYPE)
	{
		stat->setsType(dlg.getstationtype());
		stat->myItem()->updateData();
	}

	//update station name and nodes voltage value
	if((dlg.changes() & CHG_STAT_NAME)
	 ||(dlg.changes() & CHG_STAT_DATA))
	{
		//update station name
		stat->itemName()->setFont(dlg.GetFont());
		stat->itemName()->setPlainText(stat->nodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
	}

	//change links of station due to nodes changed
	if(dlg.changes() & CHG_STAT_DATA)
	{
		item->removeArrows();
		stat->removeSlinks();

		updateArrows(stat);																			//udpate station links and arrows				
	}
}
void DiagramScene::viewStation(DiagramItem *item,iSTAT* stat)
{
	StationParameterDialog dlg(stat,myDoc->getControlPanel(), pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		//change station name item
		DiagramTextItem* item = stat->itemName();
		if(item)
		{
			if(myDoc->getControlPanel().isShowAllNodeVoltage)
			{
				stat->itemName()->setPlainText(stat->allNodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
			}else
				item->setPlainText(stat->nodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
		}
	}		
}
void DiagramScene::deleteStation(DiagramItem *item,iSTAT* stat)
{
	item->removeArrows();
	removeItem(item);
	stat->setItem(NULL);

	stat->removeSlinks();
	myDoc->STAT_delete(stat->Id());

	delete item;
}
void DiagramScene::viewStationName(DiagramTextItem *item,iSTAT* stat)
{
	//todo: show station name property dialog
	
	StationNameDialog dlg(stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{		
		//Update the voltage and name text		
		stat->itemName()->setFont(dlg.GetFont());
		if(myDoc->getControlPanel().isShowAllNodeVoltage)
		{
			stat->itemName()->setPlainText(stat->allNodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
		}else
		stat->itemName()->setPlainText(stat->nodeVoltage(myDoc->getControlPanel().isShowVoltageAngle,myDoc->getControlPanel().unittype) + stat->name());
	}
}
void DiagramScene::viewStationValue(DiagramTextItem *item,iSTAT* stat)
{
	//todo: show station value property dialog
	StationValueDialog dlg(stat,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		stat->itemValue()->setPlainText(stat->value(myDoc->sBase(),myDoc->getControlPanel().unittype,myDoc->getControlPanel().isShowReactivePowerValue));
	}
}
void DiagramScene::viewSLink(iSLINK* slink)
{
	BranchEditDialog dlg(slink,BranchView,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		//to do : group change
	}
}
void DiagramScene::editSLink(iSLINK* slink)
{
	BranchEditDialog dlg(slink,BranchEdit,pMain);
	if(dlg.exec()==QDialog::Accepted)
	{	
		//to do : group change
	}
}
void DiagramScene::addNote(iNote* note,const QPointF& pos,const QSizeF& size)
{
	DiagramNoteItem* noteitem = new DiagramNoteItem(note,NULL,this);
	noteitem->setPos(pos);	
	noteitem->setSize(size);
	addItem(noteitem);

	note->setnoteitem(noteitem);
}
void DiagramScene::viewNote(iNote *note)
{
	TextDialog dlg(note,pMain);
	if(dlg.exec()!=QDialog::Accepted) return ;

	note->noteitem()->setBorder(dlg.HasBorder());
	note->noteitem()->setTextColor(dlg.GetTextcolor());
	note->noteitem()->setTextFont(dlg.GetFont());
	note->noteitem()->setAlignMode(dlg.GetAlignmode());
	note->noteitem()->setPlainText(dlg.GetText());
}

void DiagramScene::deleteNote(DiagramNoteItem *itme,iNote *note)
{
	this->removeItem(itme);
	note->setnoteitem(NULL);
	myDoc->Note_delete(note->Id());

	delete itme;
	//itme->deleteLater();
}

void DiagramScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
	Q_UNUSED(rect);
     
    QRectF sceneRect = this->sceneRect();
	painter->setPen(QPen(Qt::darkBlue, 4, Qt::SolidLine));
	painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}
void DiagramScene::selectAllStations()
{
	QMap<int,iSTAT *>& listStats = myDoc->getStatlist();
	foreach(iSTAT *stat,listStats)
	{
		DiagramItem* item = stat->myItem();
		if(item)
			item->setSelected(true);
	}
}
void DiagramScene::defAllPositions()
{
	emit allDefPositioned();																		//tell all text item back to def position
}