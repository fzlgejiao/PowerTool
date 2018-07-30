
#include <QtGui>
#include "mdichild.h"
#include "arrow.h"


MdiChild::MdiChild(QGraphicsScene * scene,iDoc* doc)
	: QGraphicsView(scene)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
	m_scene = (DiagramScene *)scene;
	m_doc	= doc;


    QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());
    scale(1, 1);

	m_scene->setSceneRect(QRectF(0, 0, 1240, 1754));												//·Ö±æÂÊ:150ÏñËØ/Ó¢´ç
	connect(m_scene, SIGNAL(itemInserted(DiagramItem*)),		this, SLOT(itemInserted(DiagramItem*)));
	connect(m_scene, SIGNAL(textInserted(QGraphicsTextItem*)),	this, SLOT(textInserted(QGraphicsTextItem*)));
	connect(m_scene, SIGNAL(itemSelected(QGraphicsItem*)),		this, SLOT(itemSelected(QGraphicsItem*)));


}

MdiChild::~MdiChild()
{
	if(m_doc)
		delete m_doc;
	if(m_scene)
		delete m_scene;
}
void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

	//test code
	m_doc->test();

    connect(m_scene, SIGNAL(changed ( const QList<QRectF> &)),
            this, SLOT(documentWasModified()));
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    //QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    	
	//todo: open map file and data file
	m_doc->openDataFile(fileName);
	m_doc->openMapFile(fileName);

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
        ret = QMessageBox::warning(this, tr("MDI"),
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

void MdiChild::itemInserted(DiagramItem *item)
{
	emit itemInserted(item, m_scene);
}

void MdiChild::textInserted(QGraphicsTextItem *item)
{
	emit textInserted(item, m_scene);
}
void MdiChild::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    QColor color = textItem->defaultTextColor();
    //fontCombo->setCurrentFont(font);
    //fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    //boldAction->setChecked(font.weight() == QFont::Bold);
    //italicAction->setChecked(font.italic());
    //underlineAction->setChecked(font.underline());
}
void MdiChild::OnScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = this->matrix();
    this->resetMatrix();
    this->translate(oldMatrix.dx(), oldMatrix.dy());
    this->scale(newScale, newScale);
}
void MdiChild::deleteItem()
{
    //foreach (QGraphicsItem *item, m_scene->selectedItems()) {
    //    if (item->type() == Arrow::Type) {
    //        m_scene->removeItem(item);
    //        Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
    //        arrow->startItem()->removeArrow(arrow);
    //        arrow->endItem()->removeArrow(arrow);
    //        delete item;
    //    }
    //}

    foreach (QGraphicsItem *item, m_scene->selectedItems()) {
         if (item->type() == DiagramItem::Type) 
		 {
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();

			 m_scene->removeItem(item);
			 iData* data = qgraphicsitem_cast<DiagramItem *>(item)->data();
			 if(data && data->type() == T_STAT)
			 {
				 ((iSTAT *)data)->itemRemoved();
				 m_doc->STAT_delete(data->Id());
			 }
			 delete item;
		}
     }
}