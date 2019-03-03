#ifndef MDICHILD_H
#define MDICHILD_H

#include <QGraphicsView>
#include "diagramscene.h"
#include "idoc.h"
#include "messageoutput.h"

class QGraphicsScene;
class QRubberBand;
class MdiChild : public QGraphicsView
{
	Q_OBJECT

public:
	MdiChild(QGraphicsScene * scene,iDoc* doc);
	~MdiChild();

    void	newFile(const QString& datafile);
    bool	loadFile(const QString &mapFile,const QString& dataFile);								//laod files: map,data. pf is from same name of data file
	bool	loadFile(const QString &mapFile,const QString& dataFile,const QString& pfFile);			//load files: map,data and pf
    bool	save();
    bool	saveAs();
    bool	saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
	DiagramScene* scene(){return m_scene;}
	iDoc *	doc(){return m_doc;}
	int		getchildScale(){return m_scale;}
	void	updatechildScale(int scale);
	void	setScale(int scale);

protected:
    void	closeEvent(QCloseEvent *event);
	void	wheelEvent(QWheelEvent * wheelEvent);
	void	mouseMoveEvent ( QMouseEvent * event );
	void	mousePressEvent ( QMouseEvent * event );
	void	mouseReleaseEvent ( QMouseEvent * event );

public slots:
    void	documentWasModified();
	//void	OnScaleChanged(const QString &);
	void	OnAreaSizeChanged(QSize & size);

signals:
	void scaleChanged(int scale);

private:
    bool	maybeSave();
    void	setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
	
    DiagramScene *m_scene;
	iDoc		 *m_doc;

    QString		curFile;
    bool		isUntitled;
	int			dpi;
	int			m_scale;
	int			scale_min;
	int			scale_max;
	QPoint		posRubberOrigin;
	QRubberBand	*rubberBand;
	MessageOutput *m_messageoutput;

};

#endif // MDICHILD_H
