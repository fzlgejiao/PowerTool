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

    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
	DiagramScene* scene(){return m_scene;}
	iDoc *GetChildDoc(){return m_doc;}
	void deleteItem();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void documentWasModified();
	void itemInserted(DiagramItem *item);
	void textInserted(QGraphicsTextItem *item);
	void itemSelected(QGraphicsItem *item);
	void OnScaleChanged(const QString &);

signals:
	void itemInserted(DiagramItem *item, DiagramScene *scene);
	void textInserted(QGraphicsTextItem *item, DiagramScene *scene);

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    DiagramScene *m_scene;
	iDoc		 *m_doc;

    QString curFile;
    bool isUntitled;

};

#endif // MDICHILD_H
