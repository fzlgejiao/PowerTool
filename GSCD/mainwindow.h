#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "diagramitem.h"
#include "idef.h"


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
	ACT_MODE mode(){return m_nActMode;}

protected:
    void closeEvent(QCloseEvent *event);
	QList<QGraphicsItem *> selectedItems();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
	void createToolBox();
    void readSettings();
    void writeSettings();

    MdiChild *createMdiChild();
    void setActiveSubWindow(QWidget *window);
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
	QMenu *settingMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

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

	QAction *addStationAction;
	QAction *addNoteAction;
    QAction *deleteAction;
	QAction *editObjectAction;

	QAction *zoomOutAction;
	QAction *zoomInAction;
	QAction *zoomResetAction;


    QAction *toFrontAction;
    QAction *sendBackAction;

	QAction *toolbarAct;
	QAction *statusbarAct;
	QAction *propertyAction;

	QAction *imageAreaAction;
	QAction *viewFontAction;
	QAction *voltageLevelAction;
	QAction *optionsAction;

    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *separatorAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

	QAction *escAct;

	QButtonGroup *buttonModeGroup;
	QActionGroup *actionModeGroup;
	//QComboBox *sceneScaleCombo;
	QLabel *currentScale;

	QToolBox *toolBox;
	QButtonGroup *buttonGroup;
	QButtonGroup *backgroundButtonGroup;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
		
	ACT_MODE	m_nActMode;
	int mScale;
	int mScaleMax;
	int mScaleMin;;
	int mScaleStep;
	int mScaleIndex;

private slots:
	//file menu
    void newFile();
    void open();
    void save();
    void saveAs();
	void print();
	void printPreview();
	void openRecentFile();

	//edit menu
	//void addStation();
	//void addNote();
	//void addInfoNote();
	//void addLegend();
	void deleteItems();
	//void selectAll();
	void editObject();

	//view menu
	//void showToolBar();
	//void showStatusBar();
	//void zoom();
	void viewProperty();
	//void viewCtrl();
	//void viewDoc();

	//setting menu
	void imageArea();
	void viewFont();
	void voltageLevel();
	void options();

	//window menu
    void updateWindowMenu();

	//help menu
    void about();

	void esc();

	//toolbar events
	void OnScaleReset();
	void OnZoomOut();
	void OnZoomIn();

	

    void updateMenus();
	void OnModeActionGroupClicked(QAction* action);
	void OnModeButtonGroupClicked(int id);
	void OnModeDone();
	void itemInserted(DiagramItem *item,DiagramScene* scene);
	void textInserted(QGraphicsTextItem *item, DiagramScene* scene);
	void OnScaleChanged(const QString &scale);
	void sceneSelectionChanged();


	//not in use
    void cut();
    void copy();
    void paste();
signals:
    void scaleChanged(const QString &scale);
};

#endif // MAINWINDOW_H
