#include <QtGui>
#include "diagramnoteitem.h"
#include "diagramscene.h"

DiagramNoteItem::DiagramNoteItem(iData* data,QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsTextItem(parent,scene)
	,m_default_width(50)
	,m_default_height(20)
{
	setData(ITEM_DATA,(uint)data);
	this->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
	this->setZValue(100);

	m_rect=QRectF(0,0,m_default_width,m_default_height);
	m_resizepos=NONE;
	iNote *note=(iNote *)data;
	m_hasBorder=note->hasborder();
	m_alignmode=note->getAlignMode();	

	m_font=note->getTextFont();
	m_color=note->getTextColor();		
	setPlainText(note->text());
	m_center=m_rect.center();	
}

DiagramNoteItem::~DiagramNoteItem()
{
	
}

void DiagramNoteItem::setAlignMode(Qt::Alignment alignmode)
{
	m_alignmode=alignmode;
}
QRectF DiagramNoteItem::boundingRect() const
{
	//return QGraphicsTextItem::boundingRect() |  m_rect;
	return  m_rect;
}

QPainterPath DiagramNoteItem::shape () const
{
	QPainterPath path;
    path.addRect(m_rect);
    return path;
}

void DiagramNoteItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	/*QStyleOptionGraphicsItem op(*option);
	
	if (option->state & QStyle::State_Selected)
            op.state = QStyle::State_None; 
			
	QGraphicsTextItem::paint(painter,&op,widget);*/
	painter->setFont(m_font);
	painter->setPen(m_color);
	painter->drawText(m_rect,m_alignmode,this->toPlainText());
	//Draw my  own selected style
	if(option->state & QStyle::State_Selected )
	{	
		painter->setBrush(Qt::NoBrush);		
		painter->setPen(QPen(Qt::black,3,Qt::SolidLine));
		painter->drawPoint(m_rect.topLeft());
		painter->drawPoint(m_rect.topRight());
		painter->drawPoint(m_rect.bottomLeft());
		painter->drawPoint(m_rect.bottomRight());

		painter->drawPoint(QPointF(m_rect.left()+m_rect.width()/2,m_rect.top()));		//top center
		painter->drawPoint(QPointF(m_rect.left()+m_rect.width()/2,m_rect.bottom()));	//bottom center
		painter->drawPoint(QPointF(m_rect.left(),m_rect.top()+m_rect.height()/2));		//left center
		painter->drawPoint(QPointF(m_rect.right(),m_rect.top()+m_rect.height()/2));		//right center
	}
	if(m_hasBorder)
	{
		painter->setPen(QPen(Qt::black,0,Qt::SolidLine));
		painter->drawRect(m_rect);	
	}	
}

void DiagramNoteItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	if (event->button() == Qt::RightButton)
	{
		scene()->clearSelection();
		setSelected(true);
	}
	QGraphicsTextItem::mousePressEvent(event);
}

void DiagramNoteItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if(!isSelected()) return ;
	QPointF pos = event->pos();
	
	QRectF toprightRegion(m_rect.topRight().x()-2,m_rect.topRight().y()-2,4,4);	
	QRectF bottomleftRegion(m_rect.bottomLeft().x()-2,m_rect.bottomLeft().y()-2,4,4);

	QRectF topleftRegion(m_rect.topLeft().x()-2,m_rect.topLeft().y()-2,4,4);	
	QRectF bottomrightRegion(m_rect.bottomRight().x()-2,m_rect.bottomRight().y()-2,4,4);

	QRectF topRegion(m_rect.left()+m_rect.width()/2-2,m_rect.top()-2,4,4);	
	QRectF bottomRegion(m_rect.left()+m_rect.width()/2-2,m_rect.bottom()-2,4,4);
	
	QRectF leftRegion(m_rect.left()-2,m_rect.top()+m_rect.height()/2-2,4,4);	
	QRectF rightRegion(m_rect.right()-2,m_rect.top()+m_rect.height()/2-2,4,4);

	if ( toprightRegion.contains(pos))
	{
		m_resizepos=TopRight;
		this->setCursor(Qt::SizeBDiagCursor);	
	}else if(bottomleftRegion.contains(pos))
	{
		m_resizepos=BottomLeft;
		this->setCursor(Qt::SizeBDiagCursor);
	}
	else if (topleftRegion.contains(pos))
	{
		m_resizepos=TopLeft;
		this->setCursor(Qt::SizeFDiagCursor);
	}else if(bottomrightRegion.contains(pos))
	{
		m_resizepos=BottomRight;
		this->setCursor(Qt::SizeFDiagCursor);
	}else if(topRegion.contains(pos))
	{
		m_resizepos=Top;
		this->setCursor(Qt::SizeVerCursor);
	}else if(bottomRegion.contains(pos))
	{
		m_resizepos=Bottom;
		this->setCursor(Qt::SizeVerCursor);
	}else if(leftRegion.contains(pos))
	{
		m_resizepos=Left;
		this->setCursor(Qt::SizeHorCursor);
	}else if(rightRegion.contains(pos))
	{
		m_resizepos=Right;
		this->setCursor(Qt::SizeHorCursor);
	}
	else 
		this->setCursor(Qt::ArrowCursor);
	QGraphicsTextItem::hoverMoveEvent(event);
}

void DiagramNoteItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	if(!isSelected()) 
	{
		this->setCursor(Qt::ArrowCursor);
		return ;
	}
	QGraphicsTextItem::hoverEnterEvent(event);
}

void DiagramNoteItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF pos1 = event->pos();        
	QPointF pos2 = event->lastPos();
	//this->prepareGeometryChange();
	float delta_x = pos2.x()-pos1.x();
	float delta_y = pos2.y()-pos1.y();		
	qreal newwidth,newheight;

	if( this->cursor().shape() == Qt::SizeBDiagCursor )
	{
		if(m_resizepos==TopRight)
		{
			newwidth =m_rect.width()-delta_x*2;
			newheight=m_rect.height()+delta_y*2;
			if((newwidth<=m_default_width)||(newheight<=m_default_height)) return ;
			m_rect.adjust(delta_x,-delta_y,-delta_x,delta_y);		
			m_rect.moveTopRight(pos2);			
		}
		else if(m_resizepos==BottomLeft)
		{
			newwidth =m_rect.width()+delta_x*2;
			newheight=m_rect.height()-delta_y*2;
			if((newwidth<=m_default_width)||(newheight<=m_default_height)) return ;
			m_rect.adjust(-delta_x,delta_y,delta_x,-delta_y);				
			m_rect.moveBottomLeft(pos2);
		}			
	}else if(this->cursor().shape() == Qt::SizeFDiagCursor)
	{
		if(m_resizepos==TopLeft)
		{			
			newwidth =m_rect.width()+delta_x*2;
			newheight=m_rect.height()+delta_y*2;
			if ((newwidth <=m_default_width) || (newheight<=m_default_height)) return  ;
			m_rect.adjust(-delta_x,-delta_y,delta_x,delta_y);
			m_rect.moveTopLeft(pos2);
		}
		else if(m_resizepos==BottomRight)
		{			
			newwidth =m_rect.width()-delta_x*2;
			newheight=m_rect.height()-delta_y*2;
			if ((newwidth <= m_default_width) || (newheight<=m_default_height)) return  ;
			m_rect.adjust(delta_x,delta_y,-delta_x,-delta_y);
			m_rect.moveBottomRight(pos2);
		}		
	}else if(this->cursor().shape() == Qt::SizeVerCursor)
	{
		if(m_resizepos==Top)
		{					
			newheight=m_rect.height()+delta_y*2;
			if (newheight<=m_default_height) return  ;
			m_rect.adjust(0,-delta_y,0,delta_y);
			m_rect.moveTop(pos2.y());
		}
		else if(m_resizepos==Bottom)
		{						
			newheight=m_rect.height()-delta_y*2;
			if (newheight<=m_default_height) return  ;
			m_rect.adjust(0,delta_y,0,-delta_y);
			m_rect.moveBottom(pos2.y());
		}		
	}else if(this->cursor().shape() == Qt::SizeHorCursor)
	{
		if(m_resizepos==Left)
		{					
			newwidth =m_rect.width()+delta_x*2;
			if (newwidth<=m_default_width) return  ;
			m_rect.adjust(-delta_x,0,delta_x,0);
			m_rect.moveLeft(pos2.x());
		}
		else if(m_resizepos==Right)
		{						
			newwidth =m_rect.width()-delta_x*2;
			if (newwidth <= m_default_width) return  ;
			m_rect.adjust(delta_x,0,-delta_x,0);
			m_rect.moveRight(pos2.x());
		}
	}
	QGraphicsTextItem::mouseMoveEvent(event);
}

