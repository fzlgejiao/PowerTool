#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
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
	void keyPressEvent(QKeyEvent *event);
	QList<QGraphicsItem *> selectedItems();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
	void createToolBox();
    void readSettings();
    void writeSettings();

    MdiChild *	createMdiChild();
    void		setActiveSubWindow(QWidget *window);
    MdiChild *	activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);
	QString		getDataFile(const QString& mapFile);

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

	//actions for 'File' menu
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
	QAction *printAct;
	QAction *previewAct;
	QAction *printSettingAct;
	QAction *pageSettingAct;
    QAction *exitAct;

	//actions for 'Edit' menu
	QAction *addStationAction;
	QAction *addNoteAction;
    QAction *deleteAction;
	QAction *editObjectAction;

	//actions for 'View' menu
	QAction *toolbarAct;
	QAction *statusbarAct;	
	QAction *scaledialogAction;
	QAction *propertyAction;
	QAction *controlpanelAction;

	//actions for 'Settings' menu
	QAction *imageAreaAction;
	QAction *resultFontAction;
	QAction *voltageLevelAction;
	QAction *optionsAction;

	//actions for 'Window' menu
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *separatorAct;

	//actions for 'Help' menu
    QAction *aboutAct;
    QAction *aboutQtAct;

	//actions for toolbar
	QAction *zoomOutAction;
	QAction *zoomInAction;
	QAction *zoomResetAction;
	QAction *arrowCursor;
	QAction *handCursor;
	QAction *fitwidthAction;
	QActionGroup *dragModeGroup;
	QButtonGroup *buttonModeGroup;
	QActionGroup *actionModeGroup;

	//actions for context menu
	QAction *selectAllAction;
	QAction *defAllPositionAction;


	//other actions
    QAction *toFrontAction;
    QAction *sendBackAction;
	
	QAction *escAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;


	//QComboBox *sceneScaleCombo;
	QLabel *currentScale;

	QToolBox *toolBox;
	QButtonGroup *buttonGroup;
	QButtonGroup *backgroundButtonGroup;

	//for status bar
	QLabel*	barVersion;
	QLabel* barDataFile;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
		
	ACT_MODE	m_nActMode;
	//int mScale;
	//int mScaleMax;
	//int mScaleMin;;
	//int mScaleStep;
	//int mScaleIndex;

private slots:
	//file menu
    void newFile();
    void open();
    void save();
    void saveAs();
	void print();
	void printPreview();
	void OnPaintRequested(QPrinter *printer);
	void openRecentFile();
	void OnSubWindowActivated(QMdiSubWindow*);

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
	void OnScaling();
	void viewProperty();
	void OnControlPanelDialog();
	//void viewCtrl();
	//void viewDoc();

	//setting menu
	void imageArea();
	void resultFont();
	void voltageLevel();
	void options();

	//window menu
    void updateWindowMenu();

	//help menu
    void about();

	void esc();

	//toolbar events
	void OnfitInView();
	void OnScaleChanged(int scale);
	void OnZoomOut();
	void OnZoomIn();

	void OnSelectModeChanged();	
	//void OnFitViewAction();

    void updateMenus();
	void OnModeActionGroupClicked(QAction* action);
	void OnModeButtonGroupClicked(int id);
	void OnModeDone();
	//void OnScaleChanged(const QString &scale);
	void OnSelectionChanged();
	void OnSelectAllStations();
	void OnDefAllPositions();


	//not in use
    void cut();
    void copy();
    void paste();
signals:
 //   void scaleChanged(const QString &scale);
};

#endif // MAINWINDOW_H
