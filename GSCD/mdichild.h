#ifndef MDICHILD_H
#define MDICHILD_H

#include <QGraphicsView>
#include "diagramscene.h"
#include "idoc.h"

class QGraphicsScene;
class MdiChild : public QGraphicsView
{
	Q_OBJECT

public:
	MdiChild(QGraphicsScene * scene,iDoc* doc);
	~MdiChild();

    void newFile(const QString& datafile);
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
	DiagramScene* scene(){return m_scene;}
	iDoc *doc(){return m_doc;}
	int getchildScale(){return m_scale;}
	void updatechildScale(int scale);
	void setScale(int scale);

protected:
    void closeEvent(QCloseEvent *event);
	void wheelEvent(QWheelEvent * wheelEvent);

public slots:
    void documentWasModified();
	//void OnScaleChanged(const QString &);
	void OnAreaSizeChanged(QSize & size);

signals:
	void scaleChanged(int scale);

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
	
    DiagramScene *m_scene;
	iDoc		 *m_doc;

    QString curFile;
    bool isUntitled;
	int dpi;
	int m_scale;
	int scale_min;
	int scale_max;

};

#endif // MDICHILD_H
