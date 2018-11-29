
#include <QtGui>
#include "mdichild.h"
#include "arrow.h"
#include "stationparameterdialog.h"

MdiChild::MdiChild(QGraphicsScene * scene,iDoc* doc)
	: QGraphicsView(scene)
	,dpi(100)
	,scale_min(10)
	,scale_max(500)
	,m_scale(80)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
	m_scene = (DiagramScene *)scene;
	m_doc	= doc;
	
	QSizeF viewsize=m_doc->getAreaSize();
	int width=viewsize.width()*dpi/25.4;
	int height=viewsize.height()*dpi/25.4;
   /* QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());*/
	m_scene->setSceneRect(0, 0, width, height);												
	setScene(m_scene);
	setCacheMode(CacheBackground);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    scale(qreal(m_scale/100.0), qreal(m_scale/100.0));
	
	setDragMode(QGraphicsView::RubberBandDrag);
	//setDragMode(QGraphicsView::ScrollHandDrag);	
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
		
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	
	//m_scene->addEllipse(QRectF(0,0,200,200),QPen(Qt::green, 1, Qt::SolidLine));	
	//m_scene->addEllipse(QRectF(500,500,100,100),QPen(Qt::red, 1, Qt::SolidLine));	
}
MdiChild::~MdiChild()
{
	if(m_scene)
		delete m_scene;	
	if(m_doc)
		delete m_doc;
}
void MdiChild::newFile(const QString& datafile)
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("GWD%1").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    QApplication::setOverrideCursor(Qt::WaitCursor);
    	
	//todo: open data file
	m_doc->readDataFile(datafile);

    QApplication::restoreOverrideCursor();

    connect(m_scene, SIGNAL(changed ( const QList<QRectF> &)),
            this, SLOT(documentWasModified()));
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("GWD"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    //QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    	
	//todo: open map file
	m_doc->readMapFile(fileName);

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(m_scene, SIGNAL(changed ( const QList<QRectF> &)),
            this, SLOT(documentWasModified()));

    return true;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("GWD"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    //QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    
	//todo: save map file

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
	setWindowModified(true);
}

bool MdiChild::maybeSave()
{
    //if (document()->isModified()) 
	{
	QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("GWD"),
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
		     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;

    //document()->setModified(false);

    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
//void MdiChild::OnScaleChanged(const QString &scale)
//{
//    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
//	m_scale=scale.left(scale.indexOf(tr("%"))).toInt();
//    QMatrix oldMatrix = this->matrix();
//    this->resetMatrix();
//    this->translate(oldMatrix.dx(), oldMatrix.dy());
//    this->scale(newScale, newScale);
//}

void MdiChild::setchildScale(int scale)    
{
	if((scale<scale_min) || (scale>scale_max)) return ;
	qreal newScale = scale / 100.0;
	m_scale=scale;
    QMatrix oldMatrix = this->matrix();
    this->resetMatrix();
    this->translate(oldMatrix.dx(), oldMatrix.dy());
    this->scale(newScale, newScale);

	emit scaleChanged(m_scale);
}

void MdiChild::wheelEvent(QWheelEvent * wheelEvent)
{
	int delta=wheelEvent->delta();
	if (wheelEvent->modifiers() == Qt::ControlModifier){		//scale function: Ctrl+ wheel,up is zoom in ,down is zoom out		
		//qreal ss= pow((double)2, delta / 240.0);
		qreal ss=0;		
		if(delta>0) 
			ss=1.1;else ss=0.9;		
	
		qreal factor = transform().scale(ss, ss).mapRect(QRectF(0, 0, 1, 1)).width();
		m_scale=factor*100;											//this  scale is in percent mode		
		if((m_scale<scale_min)||(m_scale>scale_max)) return;	
		emit scaleChanged(m_scale);
		scale(ss,ss);
		wheelEvent->accept();
	}else if(wheelEvent->modifiers()  == Qt::ShiftModifier)	//Shift + wheel . horizontal scrollbar
	{		
		if(this->horizontalScrollBar()->isVisible())
		{
			int value=this->horizontalScrollBar()->value();
			this->horizontalScrollBar()->setValue(value-delta);
		}
	}else	
	QGraphicsView::wheelEvent(wheelEvent);
}

void MdiChild::OnAreaSizeChanged(QSize & size)
{
	int width=size.width()*dpi/25.4;
	int height=size.height()*dpi/25.4;
  
	m_scene->setSceneRect(0, 0, width, height);			
}

