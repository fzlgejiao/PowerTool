#ifndef DIAGRAMNOTEITEM_H
#define DIAGRAMNOTEITEM_H

#include <QGraphicsTextItem>
#include <QFont>
#include "idata.h"


QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


typedef enum 
{
	TopLeft =0,
	Top,
	TopRight,
	Left,
	Right,
	BottomLeft,
	Bottom,
	BottomRight,
	NONE
}ResizePosition;


class DiagramNoteItem : public QGraphicsTextItem
{
	Q_OBJECT
		
public:
	enum { Type = UserType + 23 };
	int type() const
        { return Type; }
	DiagramNoteItem(iData* data,QGraphicsItem *parent=0,QGraphicsScene *scene = 0);
	~DiagramNoteItem();
	bool hasborder(){return m_hasBorder;}
	void setBorder(bool value){m_hasBorder=value;}
	void setTextColor(QColor color){m_color=color;}
	void setTextFont(QFont font){m_font=font;}
	void setAlignMode(Qt::Alignment alignmode);
	void setSize(const QSizeF & size){ m_rect=QRectF(QPointF(0,0),size);}
	QSizeF size(){return m_rect.size();}

signals:


protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	QRectF	boundingRect () const;
	QPainterPath shape () const;

private:	
	bool m_hasBorder;	
	QRectF m_rect;
	ResizePosition m_resizepos;
	Qt::Alignment m_alignmode;	
	QFont m_font;
	QColor m_color;
	qreal m_default_width;
	qreal m_default_height;	
	QPointF m_center;	
};

#endif // DIAGRAMNOTEITEM_H
