#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "diagramitem.h"


class MdiChild;
class DiagramScene;
class iDoc;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QButtonGroup;
class QToolBox;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
	QMenu* createPopupMenu() { return NULL; }

private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
	void print();
	void printPreview();
	void openRecentFile();
    void cut();
    void copy();
    void paste();

	void addStation();
	void addNote();
	void deleteItem();
	

    void about();
    void updateMenus();
    void updateWindowMenu();
    MdiChild *createMdiChild();
    void setActiveSubWindow(QWidget *window);
	//void buttonGroupClicked(int id);
	//void pointerGroupClicked(int id);
	void itemInserted(DiagramItem *item,DiagramScene* scene);
	void textInserted(QGraphicsTextItem *item, DiagramScene* scene);
	void sceneScaleChanged(const QString &scale);
	void OnScaleReset();
	void OnZoomOut();
	void OnZoomIn();

signals:
    void scaleChanged(const QString &scale);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
	void createToolBox();
    void readSettings();
    void writeSettings();
    MdiChild *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);
	//QWidget *createCellWidget(const QString &text,
	//	DiagramItem::DiagramType type);
	//QWidget *createBackgroundCellWidget(const QString &text,
	//	const QString &image);

	Ui::MainWindowClass ui;
    QMdiArea *mdiArea;
	QSignalMapper *windowMapper;


    QMenu *fileMenu;
    QMenu *editMenu;
	QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
	QMenu *stationcontextMenu;
    QToolBar *tBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
	QAction *printAct;
	QAction *previewAct;
	QAction *printSettingAct;
	QAction *pageSettingAct;
    QAction *exitAct;

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

	QAction *addItemAction;
	QAction *stationeditAction;
	QAction *addNoteAction;
	QAction *zoomOutAction;
	QAction *zoomInAction;
	QAction *zoomResetAction;

    QAction *deleteAction;
    QAction *toFrontAction;
    QAction *sendBackAction;

	QAction *toolbarAct;
	QAction *statusbarAct;
	QAction *parameterAct;

    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *separatorAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

	QButtonGroup *pointerTypeGroup;
	//QComboBox *sceneScaleCombo;
	QLabel *currentScale;

	QToolBox *toolBox;
	QButtonGroup *buttonGroup;
	QButtonGroup *backgroundButtonGroup;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
		
	int mScale;
	int mScaleMax;
	int mScaleMin;;
	int mScaleStep;
	int mScaleIndex;
};

#endif // MAINWINDOW_H
