
#include <QtGui>
#include <QList>
#include <QDialog>
#include <QPrintPreviewDialog>
#include "mainwindow.h"
#include "mdichild.h"
#include "adddialog.h"
#include "idata.h"
#include "scaledialog.h"
#include "controlpaneldialog.h"
#include "areasetting.h"
#include "diagramitem.h"
#include "arrow.h"
#include "newdialog.h"

const int InsertTextButton = 10;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	//,mScale(100)
	//,mScaleMax(200)
	//,mScaleMin(10)
	//,mScaleStep(10)
	//,mScaleIndex((mScaleMax-mScaleMin)/mScaleStep/2)
{
	ui.setupUi(this);

	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setViewMode(QMdiArea::TabbedView);
	mdiArea->setTabsClosable(true);
	//mdiArea->setTabPosition(QTabWidget::West);

	//setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),this, SLOT(OnSubWindowActivated(QMdiSubWindow*)));
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget*)),	this, SLOT(setActiveSubWindow(QWidget*)));

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	updateMenus();

	readSettings();


	//createToolBox();

	QHBoxLayout *layout = new QHBoxLayout;
	//layout->addWidget(toolBox);
	layout->addWidget(mdiArea);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	setCentralWidget(widget);

	setWindowTitle(tr("GWD"));
	setUnifiedTitleAndToolBarOnMac(true);

	m_nActMode = M_MoveItem;
}

MainWindow::~MainWindow()
{

}
void MainWindow::closeEvent(QCloseEvent *event)
{
	mdiArea->closeAllSubWindows();
	if (mdiArea->currentSubWindow()) {
		event->ignore();
	} else {
		writeSettings();
		event->accept();
	}
}
void MainWindow::updateMenus()
{
	bool hasMdiChild = (activeMdiChild() != 0);
	saveAct->setEnabled(hasMdiChild);
	saveAsAct->setEnabled(hasMdiChild);
	pasteAct->setEnabled(hasMdiChild);
	closeAct->setEnabled(hasMdiChild);
	closeAllAct->setEnabled(hasMdiChild);
	tileAct->setEnabled(hasMdiChild);
	cascadeAct->setEnabled(hasMdiChild);
	separatorAct->setVisible(hasMdiChild);
	arrowCursor->setEnabled(hasMdiChild);
	handCursor->setEnabled(hasMdiChild);
	fitwidthAction->setEnabled(hasMdiChild);

	bool hasSelection = true;//(activeMdiChild() &&
	//activeMdiChild()->textCursor().hasSelection());
	cutAct->setEnabled(hasSelection);
	copyAct->setEnabled(hasSelection);

	if(hasMdiChild)
	{
		if(activeMdiChild()->dragMode()==QGraphicsView::RubberBandDrag)
		{
			arrowCursor->setChecked(true);
		}else 
			handCursor->setChecked(true);
		int activescale=activeMdiChild()->getchildScale();
		QString scaletxt=QString("%1%").arg(activescale,3,10,QChar(' '));	
		currentScale->setText(scaletxt);
	}
	OnSelectionChanged();																			//to update child related menu status
}

void MainWindow::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAct);
	windowMenu->addAction(closeAllAct);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAct);
	windowMenu->addAction(cascadeAct);
	windowMenu->addSeparator();
	windowMenu->addAction(separatorAct);

	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	separatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) {
		MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

		QString text;
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		} else {
			text = tr("%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		QAction *action  = windowMenu->addAction(text);
		action->setCheckable(true);
		action ->setChecked(child == activeMdiChild());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

MdiChild *MainWindow::createMdiChild()
{
	iDoc *doc			= new iDoc(this);
	DiagramScene* scene	= new DiagramScene(doc,this);
	MdiChild *child		= new MdiChild(scene,doc);

	mdiArea->addSubWindow(child);
	child->showMaximized();

	connect(child, SIGNAL(copyAvailable(bool)),		cutAct, SLOT(setEnabled(bool)));
	connect(child, SIGNAL(copyAvailable(bool)),		copyAct, SLOT(setEnabled(bool)));

	//connect(this,  SIGNAL(scaleChanged(const QString &)),child, SLOT(OnScaleChanged(const QString &)));
	connect(scene, SIGNAL(selectionChanged()),this, SLOT(OnSelectionChanged()));
	connect(scene, SIGNAL(modeDone()),this, SLOT(OnModeDone()));

	connect(doc,SIGNAL(areaSizeChanged(QSize &)),child,SLOT(OnAreaSizeChanged(QSize &)));
	connect(child,SIGNAL(scaleChanged(int)),this,SLOT(OnscaleChanged(int)));
	return child;
}

void MainWindow::createActions()
{
	newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	printAct = new QAction(tr("&Print..."), this);
	printAct->setShortcuts(QKeySequence::Print);
	printAct->setStatusTip(tr("Print the current document"));
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

	previewAct = new QAction(tr("Print preview..."), this);
	previewAct->setStatusTip(tr("Preview the current document"));
	connect(previewAct, SIGNAL(triggered()), this, SLOT(printPreview()));

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(openRecentFile()));
	}


	//! [0]
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	//! [0]

	cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
		"clipboard"));
	connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

	copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

	pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));


	toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
		tr("Bring to &Front"), this);
	toFrontAction->setShortcut(tr("Ctrl+F"));
	toFrontAction->setStatusTip(tr("Bring item to front"));
	connect(toFrontAction, SIGNAL(triggered()),
		this, SLOT(bringToFront()));
	//! [23]

	sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
		tr("Send to &Back"), this);
	sendBackAction->setShortcut(tr("Ctrl+B"));
	sendBackAction->setStatusTip(tr("Send item to back"));
	connect(sendBackAction, SIGNAL(triggered()),
		this, SLOT(sendToBack()));

	deleteAction = new QAction(QIcon(":/images/delete.png"),
		tr("&Delete"), this);
	deleteAction->setShortcut(tr("Delete"));
	deleteAction->setStatusTip(tr("Delete selected object"));
	connect(deleteAction, SIGNAL(triggered()),
		this, SLOT(deleteItems()));

	addStationAction = new QAction(QIcon(":/images/addnet.png"),
		tr("&Add Station"), this);		
	addStationAction->setStatusTip(tr("Add a Station"));
	addStationAction->setCheckable(true);

	addNoteAction = new QAction(QIcon(":/images/addcomment.png"),
		tr("Add &Note"), this);		
	addNoteAction->setStatusTip(tr("Add a Note"));
	addNoteAction->setCheckable(true);

	zoomOutAction = new QAction(QIcon(":/images/zoomout.png"),
		tr("Zoom &Out"), this);	
	zoomOutAction->setStatusTip(tr("Zoom out"));
	connect(zoomOutAction, SIGNAL(triggered()),this, SLOT(OnZoomOut()));

	zoomInAction = new QAction(QIcon(":/images/zoomin.png"),
		tr("Zoom &In"), this);	
	zoomInAction->setStatusTip(tr("Zoom in"));
	connect(zoomInAction, SIGNAL(triggered()),this, SLOT(OnZoomIn()));

	arrowCursor = new QAction(QIcon(":/images/arrow.png"),
		tr("&Select Mode"), this);	
	arrowCursor->setStatusTip(tr("Select Mode"));
	connect(arrowCursor, SIGNAL(toggled(bool)),this, SLOT(OnSelectModeChanged()));
	arrowCursor->setCheckable(true);
	arrowCursor->setChecked(true);

	handCursor = new QAction(QIcon(":/images/hand.png"),
		tr("&Drag Mode"), this);	
	handCursor->setStatusTip(tr("Drag Mode"));
	connect(handCursor, SIGNAL(toggled(bool)),this, SLOT(OnSelectModeChanged()));
	handCursor->setCheckable(true);
	handCursor->setChecked(false);

	modeActionGroup=new QActionGroup(this);
	modeActionGroup->addAction(arrowCursor);
	modeActionGroup->addAction(handCursor);
	modeActionGroup->setExclusive(true);

	fitwidthAction = new QAction(QIcon(":/images/fitwidth.png"),
		tr("&Fit In View"), this);	
	fitwidthAction->setStatusTip(tr("Fit In View"));
	connect(fitwidthAction, SIGNAL(triggered()),this, SLOT(OnfitInView()));

	zoomResetAction = new QAction(QIcon(":/images/reset.png"),
		tr("Fit to &View"), this);	
	zoomResetAction->setStatusTip(tr("Fit Size to This View"));
	connect(zoomResetAction, SIGNAL(triggered()),this, SLOT(OnfitInView()));

	toolbarAct = new QAction(tr("Show &Toolbar"), this);
	toolbarAct->setStatusTip(tr("Show or hide Toolbar"));
	connect(toolbarAct, SIGNAL(triggered()),
		mdiArea, SLOT(showToolbar()));

	statusbarAct = new QAction(tr("Show &Statusbar"), this);
	statusbarAct->setStatusTip(tr("Show or hide Statusbar"));
	connect(statusbarAct, SIGNAL(triggered()),
		mdiArea, SLOT(showStatusbar()));

	scaledialogAction = new QAction(tr("&Scaling..."), this);
	scaledialogAction->setStatusTip(tr("Change the scale"));
	connect(scaledialogAction, SIGNAL(triggered()),this, SLOT(OnZoomDialog()));

	propertyAction = new QAction(tr("&Properties..."), this);
	propertyAction->setStatusTip(tr("Show object property"));
	connect(propertyAction, SIGNAL(triggered()),this, SLOT(viewProperty()));

	controlpanelAction = new QAction(tr("&Control Panel..."), this);
	controlpanelAction->setStatusTip(tr("Control Panel Dialog"));
	connect(controlpanelAction, SIGNAL(triggered()),this, SLOT(OnControlPanelDialog()));

	editObjectAction = new QAction(tr("&Edit Object..."), this);
	editObjectAction->setStatusTip(tr("Edit object"));
	connect(editObjectAction, SIGNAL(triggered()),this, SLOT(editObject()));

	//actions for menu 'settings'
	imageAreaAction = new QAction(tr("&Image Area..."), this);
	imageAreaAction->setStatusTip(tr("Image area"));
	connect(imageAreaAction, SIGNAL(triggered()),this, SLOT(imageArea()));

	viewFontAction = new QAction(tr("&Font..."), this);
	viewFontAction->setStatusTip(tr("View font"));
	connect(viewFontAction, SIGNAL(triggered()),this, SLOT(viewFont()));

	voltageLevelAction = new QAction(tr("&Voltage Level..."), this);
	voltageLevelAction->setStatusTip(tr("Voltage level"));
	connect(voltageLevelAction, SIGNAL(triggered()),this, SLOT(voltageLevel()));

	optionsAction = new QAction(tr("&Options..."), this);
	optionsAction->setStatusTip(tr("options"));
	connect(optionsAction, SIGNAL(triggered()),this, SLOT(options()));

	//actions for menu 'window'
	closeAct = new QAction(tr("Cl&ose"), this);
	closeAct->setStatusTip(tr("Close the active window"));
	connect(closeAct, SIGNAL(triggered()),
		mdiArea, SLOT(closeActiveSubWindow()));

	closeAllAct = new QAction(tr("Close &All"), this);
	closeAllAct->setStatusTip(tr("Close all the windows"));
	connect(closeAllAct, SIGNAL(triggered()),
		mdiArea, SLOT(closeAllSubWindows()));

	tileAct = new QAction(tr("&Tile"), this);
	tileAct->setStatusTip(tr("Tile the windows"));
	connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

	cascadeAct = new QAction(tr("&Cascade"), this);
	cascadeAct->setStatusTip(tr("Cascade the windows"));
	connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

	separatorAct = new QAction(this);
	separatorAct->setSeparator(true);

	//actions for menu 'about'
	aboutAct = new QAction(QIcon(":/images/about.png"),tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	escAct = new QAction(tr("&Esc"), this);
	escAct->setShortcut(Qt::Key_Escape);
	connect(escAct, SIGNAL(triggered()), this, SLOT(Esc()));



	actionModeGroup = new QActionGroup(this);
	actionModeGroup->setExclusive(true);
	actionModeGroup->addAction(addStationAction);
	actionModeGroup->addAction(addNoteAction);
	connect(actionModeGroup, SIGNAL(triggered(QAction *)), this, SLOT(OnModeActionGroupClicked(QAction *)));

	//init status for actions
	deleteAction->setEnabled(false);
	editObjectAction->setEnabled(false);
	propertyAction->setEnabled(false);

}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(printAct);
	fileMenu->addAction(previewAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(addStationAction);
	editMenu->addAction(addNoteAction);
	editMenu->addSeparator();
	editMenu->addAction(deleteAction);
	editMenu->addAction(editObjectAction);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(toolbarAct);
	viewMenu->addAction(statusbarAct);
	viewMenu->addSeparator();
	/*viewMenu->addAction(zoomInAction);
	viewMenu->addAction(zoomResetAction);
	viewMenu->addAction(zoomOutAction);*/
	viewMenu->addAction(scaledialogAction);
	viewMenu->addSeparator();
	viewMenu->addAction(propertyAction);
	viewMenu->addAction(controlpanelAction);

	settingMenu = menuBar()->addMenu(tr("&Settings"));
	settingMenu->addAction(imageAreaAction);
	settingMenu->addAction(viewFontAction);
	settingMenu->addAction(voltageLevelAction);
	settingMenu->addAction(optionsAction);


	windowMenu = menuBar()->addMenu(tr("&Window"));
	updateWindowMenu();
	connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);

}

void MainWindow::createToolBars()
{
	QList<QToolBar *> toolbars = findChildren<QToolBar *>();
	tBar = toolbars[0];//addToolBar(tr("File"));
	tBar->setWindowTitle(tr("File"));

	//tBar->addAction(cutAct);
	//tBar->addAction(copyAct);
	//tBar->addAction(pasteAct);
	tBar->setIconSize(QSize(32,32));
	tBar->setMovable(false);

	QToolButton *btnAddStation = new QToolButton;
	btnAddStation->setCheckable(true);
	btnAddStation->setIcon(QIcon(":/images/addnet.png"));
	btnAddStation->setStatusTip("Add A New Station");
	btnAddStation->setShortcut(tr("Add Station"));

	QToolButton *btnAddNote = new QToolButton;
	btnAddNote->setCheckable(true);
	btnAddNote->setIcon(QIcon(":/images/addcomment.png"));
	btnAddNote->setStatusTip("Add A Text Note");
	btnAddNote->setShortcut(tr("Add Note"));

	buttonModeGroup = new QButtonGroup(this);
	buttonModeGroup->addButton(btnAddStation, M_AddStation);
	buttonModeGroup->addButton(btnAddNote,	  M_AddNote);
	buttonModeGroup->setExclusive(false);
	connect(buttonModeGroup, SIGNAL(buttonClicked(int)),this, SLOT(OnModeButtonGroupClicked(int)));

	/*sceneScaleCombo = new QComboBox;

	QStringList scales;
	scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");	
	sceneScaleCombo->addItems(scales);
	sceneScaleCombo->setCurrentIndex((mScaleMax-mScaleMin)/mScaleStep/2);
	connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),this, SLOT(OnScaleChanged(QString)));
	*/

	tBar->addAction(newAct);
	tBar->addAction(openAct);
	tBar->addAction(saveAct);

	tBar->addWidget(new QLabel("      "));
	tBar->addAction(deleteAction);

	tBar->addWidget(new QLabel("      "));
	tBar->addWidget(btnAddStation);
	tBar->addWidget(btnAddNote);

	tBar->addWidget(new QLabel("      "));
	tBar->addAction(zoomOutAction);
	currentScale=new QLabel("80%");	
	currentScale->setFixedWidth(40);
	currentScale->setAlignment(Qt::AlignCenter);
	tBar->addWidget(currentScale);
	tBar->addAction(zoomInAction);
	//tBar->addAction(zoomResetAction);
	tBar->addWidget(new QLabel("      "));
	tBar->addSeparator();
	tBar->addWidget(new QLabel("      "));
	tBar->addAction(arrowCursor);
	tBar->addAction(handCursor);
	tBar->addWidget(new QLabel("      "));
	tBar->addAction(fitwidthAction);


	//tBar->addWidget(new QLabel("      "));
	//tBar->addWidget(pointerButton);
	//tBar->addWidget(linePointerButton);

	tBar->addWidget(new QLabel("      "));
	tBar->addAction(aboutAct);


	

}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));

	barDataFile = new QLabel(this);
	barDataFile->setMinimumSize(350,20); 
    barDataFile->setAlignment(Qt::AlignLeft|Qt::AlignVCenter); 
    barDataFile->setFrameShape(QFrame::Panel);
    barDataFile->setFrameShadow(QFrame::Sunken);
	barDataFile->setText("");
	statusBar()->insertPermanentWidget(0,barDataFile);

	barVersion = new QLabel(this);
	barVersion->setMinimumSize(80,20); 
	barVersion->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter); 
    barVersion->setFrameShape(QFrame::Panel);
    barVersion->setFrameShadow(QFrame::Sunken);
	barVersion->setText(qApp->applicationVersion());
	statusBar()->insertPermanentWidget(1,barVersion);
}
//
//void MainWindow::createToolBox()
//{
//	buttonGroup = new QButtonGroup(this);
//	buttonGroup->setExclusive(false);
//	connect(buttonGroup, SIGNAL(buttonClicked(int)),
//		this, SLOT(buttonGroupClicked(int)));
//	QGridLayout *layout = new QGridLayout;
//	layout->addWidget(createCellWidget(tr("Station"),											//Conditional
//		DiagramItem::Conditional), 0, 0);
//	layout->addWidget(createCellWidget(tr("Generator"),										//Process
//		DiagramItem::Step), 0, 1);
//	layout->addWidget(createCellWidget(tr("Transformer"),										//IO
//		DiagramItem::Io), 1, 0);
//	//! [21]
//
//	QToolButton *textButton = new QToolButton;
//	textButton->setCheckable(true);
//	buttonGroup->addButton(textButton, InsertTextButton);
//	textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
//		.scaled(30, 30)));
//	textButton->setIconSize(QSize(50, 50));
//	QGridLayout *textLayout = new QGridLayout;
//	textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
//	textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
//	QWidget *textWidget = new QWidget;
//	textWidget->setLayout(textLayout);
//	layout->addWidget(textWidget, 1, 1);
//
//	layout->setRowStretch(3, 10);
//	layout->setColumnStretch(2, 10);
//
//	QWidget *itemWidget = new QWidget;
//	itemWidget->setLayout(layout);
//
//	backgroundButtonGroup = new QButtonGroup(this);
//	connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
//		this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));
//
//	QGridLayout *backgroundLayout = new QGridLayout;
//	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
//		":/images/background1.png"), 0, 0);
//	backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
//		":/images/background2.png"), 0, 1);
//	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
//		":/images/background3.png"), 1, 0);
//	backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
//		":/images/background4.png"), 1, 1);
//
//	backgroundLayout->setRowStretch(2, 10);
//	backgroundLayout->setColumnStretch(2, 10);
//
//	QWidget *backgroundWidget = new QWidget;
//	backgroundWidget->setLayout(backgroundLayout);
//
//
//	//! [22]
//	toolBox = new QToolBox;
//	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
//	toolBox->setMinimumWidth(itemWidget->sizeHint().width());
//	toolBox->addItem(itemWidget, tr("Basic Shapes"));
//	toolBox->addItem(backgroundWidget, tr("Backgrounds"));
//}
//
//QWidget *MainWindow::createCellWidget(const QString &text,
//	DiagramItem::DiagramType type)
//{
//	DiagramItem item(type); 
//	QIcon icon(item.image());
//
//	QToolButton *button = new QToolButton;
//	button->setIcon(icon);
//	button->setIconSize(QSize(50, 50));
//	button->setCheckable(true);
//	buttonGroup->addButton(button, int(type));
//
//	QGridLayout *layout = new QGridLayout;
//	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
//
//	QWidget *widget = new QWidget;
//	widget->setLayout(layout);
//
//	return widget;
//}
//QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
//	const QString &image)
//{
//	QToolButton *button = new QToolButton;
//	button->setText(text);
//	button->setIcon(QIcon(image));
//	button->setIconSize(QSize(50, 50));
//	button->setCheckable(true);
//	backgroundButtonGroup->addButton(button);
//
//	QGridLayout *layout = new QGridLayout;
//	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
//
//	QWidget *widget = new QWidget;
//	widget->setLayout(layout);
//
//	return widget;
//}

void MainWindow::readSettings()
{
	QSettings settings("Qt", "GSCD");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	move(pos);
	resize(size);
}

void MainWindow::writeSettings()
{
	QSettings settings("Qt", "GSCD");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

MdiChild *MainWindow::activeMdiChild()
{
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<MdiChild *>(activeSubWindow->widget());
	return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
	QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (mdiChild->currentFile() == canonicalFilePath)
			return window;
	}
	return 0;
}
void MainWindow::OnSubWindowActivated(QMdiSubWindow* window)
{
	if(window)
	{
		updateMenus();
		MdiChild *child = (MdiChild *)window->widget();
		if(child)
			barDataFile->setText(child->doc()->dataFile());
	}
}
QList<QGraphicsItem *> MainWindow::selectedItems()
{
	QList<QGraphicsItem *> items;
	MdiChild* child = activeMdiChild();
	if(!child)
		return items;

	return child->scene()->selectedItems();
}

void MainWindow::OnModeActionGroupClicked(QAction* action)
{
	if(action == addStationAction)
		buttonModeGroup->button(M_AddStation)->click();
	else if(action == addNoteAction)
		buttonModeGroup->button(M_AddNote)->click();
}

void MainWindow::OnModeButtonGroupClicked(int id)
{
	foreach(QAbstractButton *button, buttonModeGroup->buttons()) 
	{
		if (buttonModeGroup->button(id) != button)
			button->setChecked(false);
	}
	bool bChecked = buttonModeGroup->button(id)->isChecked();
	if(id == M_AddStation)
		addStationAction->setChecked(bChecked?true:false);
	else if(id == M_AddNote)
		addNoteAction->setChecked(bChecked?true:false);

	if(bChecked == true)
		m_nActMode = (ACT_MODE)id;
}

void MainWindow::OnSelectionChanged()
{
	QList<QGraphicsItem *> items = selectedItems();
	if(items.count())
	{
		//todo: enable/disable menu items of mainwindow
		if(items.count()==1)
		{
			propertyAction->setEnabled(true);
			QGraphicsItem* item = selectedItems().first();

			if(!item)
				return;
			iData* data = (iData *)item->data(ITEM_DATA).toUInt();
			if(!data)
				return;
			switch(data->type())
			{
			case T_STAT:
				if(item->type() == DiagramItem::Type)
				{
					deleteAction->setEnabled(true);
					editObjectAction->setEnabled(true);
				}
				else
				{
					deleteAction->setEnabled(false);
					editObjectAction->setEnabled(false);
				}
				break;
			case T_BRANCH:
				editObjectAction->setEnabled(true);
				break;
			case T_SLINK:
				if(item->type() == Arrow::Type)
				{
					deleteAction->setEnabled(false);
					editObjectAction->setEnabled(true);
				}
				else
				{
					deleteAction->setEnabled(false);
					editObjectAction->setEnabled(false);
				}
				break;

			case T_NOTE:
				{
					deleteAction->setEnabled(true);
					editObjectAction->setEnabled(true);
				}
				break;
			}
		}
	}
	else
	{
		//todo: enable/disable menu items of mainwindow
		deleteAction->setEnabled(false);
		editObjectAction->setEnabled(false);
		propertyAction->setEnabled(false);
	}
}

void MainWindow::newFile()
{
	NewDialog dlg(this);
	if(dlg.exec() == QDialog::Rejected)
		return;

	MdiChild *child = createMdiChild();																//create child when new a file
	child->newFile(dlg.FileName());
	statusBar()->showMessage(tr("File loaded"), 2000);
	child->show();
	barDataFile->setText(dlg.FileName());
}

void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open map file"),".",tr("Map File (*.xml)"));
	if (!fileName.isEmpty()) {
		QMdiSubWindow *existing = findMdiChild(fileName);
		if (existing) {
			mdiArea->setActiveSubWindow(existing);
			return;
		}

		MdiChild *child = createMdiChild();															//create child when open a file

		if (child->loadFile(fileName)) {
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();
			barDataFile->setText(child->doc()->dataFile());
		} else {
			child->close();
		}
	}
}

void MainWindow::save()
{
	if (activeMdiChild() && activeMdiChild()->save())
		statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
	if (activeMdiChild() && activeMdiChild()->saveAs())
		statusBar()->showMessage(tr("File saved"), 2000);
}
void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
	//QTextDocument *document = textEdit->document();
	//QPrinter printer;

	//QPrintDialog *dlg = new QPrintDialog(&printer, this);
	//if (dlg->exec() != QDialog::Accepted)
	//    return;

	//document->print(&printer);

	//statusBar()->showMessage(tr("Ready"), 2000);

	 MdiChild* child = activeMdiChild(); 
	 if(!child) return;
	 child->scene()->clearSelection();
	 QPrinter printer(QPrinter::HighResolution);
	 printer.setOrientation(QPrinter::Landscape);
	 printer.setPageMargins(0.7,0.75,0.7,0.75,QPrinter::Inch);				//Default Margins
     QPrintDialog dialog(&printer, this);
     if (dialog.exec() == QDialog::Accepted) {
         QPainter painter(&printer);
		 child->scene()->render(&painter,QRectF(),QRectF(),Qt::IgnoreAspectRatio);
     }

#endif
}
void MainWindow::printPreview()
{
	MdiChild* child = activeMdiChild(); 
	if(!child) return;

	//child->scene()->clearSelection();
	QPrinter printer(QPrinter::ScreenResolution);
	printer.setOrientation(QPrinter::Landscape);						//Default to landscape
	QPrintPreviewDialog preview(&printer,this);	
	preview.setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);		

	connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(Onpaintrequested(QPrinter*)));
	preview.showMaximized();
	preview.exec();	
}

void MainWindow::Onpaintrequested(QPrinter *printer)
{		
	printer->setPageMargins(0.7,0.75,0.7,0.75,QPrinter::Inch);				//Default Margins
	
	QPainter painter(printer);	
	activeMdiChild()->scene()->render(&painter,QRectF(),QRectF(),Qt::IgnoreAspectRatio);
}

void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		MdiChild *child = createMdiChild();															//create child when open a recent file
		if (child->loadFile(action->data().toString())) {
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();
		} else {
			child->close();
		}
	}
}

void MainWindow::cut()
{
	//if (activeMdiChild())
	//    activeMdiChild()->cut();
}

void MainWindow::copy()
{
	//if (activeMdiChild())
	//    activeMdiChild()->copy();
}

void MainWindow::paste()
{
	//if (activeMdiChild())
	//    activeMdiChild()->paste();
}

void MainWindow::deleteItems()
{
	MdiChild* child = activeMdiChild();
	if(!child)
		return;

	child->scene()->deleteItems();
}

void MainWindow::editObject()
{
	MdiChild* child = activeMdiChild();
	if(!child)
		return;

	child->scene()->editItem();
}
void MainWindow::viewProperty()
{
	MdiChild* child = activeMdiChild();
	if(!child)
		return;

	child->scene()->viewItem();
}
void MainWindow::OnControlPanelDialog()
{
	MdiChild* child = activeMdiChild();
	if(!child)	return;
	ControlPanelDialog dlg(child->doc()->getControlPanel(),this);
	if(dlg.exec()==QDialog::Accepted)
	{
		child->doc()->setControlPanel(dlg.getControlPanel(),dlg.getchanges());
	}
}
void MainWindow::imageArea()
{
	MdiChild* child = activeMdiChild();
	if(!child)	return;
	AreaSetting dlg(child->doc()->getAreaSize(),this);
	if(dlg.exec()==QDialog::Accepted)
	{
		child->doc()->setAreaSize(dlg.getAreasize());
	}
}
void MainWindow::viewFont()
{
}
void MainWindow::voltageLevel()
{
}
void MainWindow::options()
{
}
void MainWindow::about()
{
	QMessageBox::about(this, tr("About GWD"),
		tr("The <b>GWD</b> is Geographical wiring diagram for power system."));
}

void MainWindow::esc()
{

}

//void MainWindow::OnScaleChanged(const QString &scale)
//{
//	emit scaleChanged(scale);
//}

void MainWindow::OnfitInView()
{	
	MdiChild* child = activeMdiChild();
	if(!child)	return;
		
	QRectF sceneRect=child->sceneRect();		
	child->fitInView(sceneRect,Qt::KeepAspectRatio);
	
	qreal current=child->mapToScene(QRect(0,0,1,1)).boundingRect().width();
	qreal scale=1/current;

	child->setScale((int)(scale*100));
}

void MainWindow::OnZoomOut()
{		
	MdiChild *child=activeMdiChild();
	if(child==NULL) return;

	int newscale=child->getchildScale()*0.9;
	child->updatechildScale(newscale);
}

void MainWindow::OnZoomIn()
{	
	MdiChild *child=activeMdiChild();
	if(child==NULL) return;

	int newscale=child->getchildScale()*1.1;
	child->updatechildScale(newscale);
}

void MainWindow::OnSelectModeChanged()
{
	MdiChild* child = activeMdiChild();
	if(!child)	return;
	bool isSelectedmode=arrowCursor->isChecked();
	child->setDragMode(isSelectedmode?QGraphicsView::RubberBandDrag: QGraphicsView::ScrollHandDrag);
	child->setInteractive(arrowCursor->isChecked());

	addStationAction->setEnabled(isSelectedmode);
	addNoteAction->setEnabled(isSelectedmode);

	foreach(QAbstractButton *button, buttonModeGroup->buttons())
		button->setEnabled(isSelectedmode);
}
void MainWindow::OnZoomDialog()
{
	MdiChild* child = activeMdiChild();
	if(!child)	return;
	ScaleDialog scaledialog(child->getchildScale(),this);
	if(scaledialog.exec()==QDialog::Accepted)
	{
		int scale=scaledialog.GetScale();
		QString scaletxt=QString("%1%").arg(scale,3,10,QChar(' '));	
		currentScale->setText(scaletxt);	
		child->updatechildScale(scale);
	}
}

void MainWindow::OnscaleChanged(int scale)
{
	QString scaletxt=QString("%1%").arg(scale,3,10,QChar(' '));		
	this->currentScale->setText(scaletxt);
}

void MainWindow::OnModeDone()
{
	foreach(QAction *action, actionModeGroup->actions())
		action->setChecked(false);

	foreach(QAbstractButton *button, buttonModeGroup->buttons())
		button->setChecked(false);

	m_nActMode = M_MoveItem;
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) 
	{
		OnModeDone();
    } 
	else 
	{
        QMainWindow::keyPressEvent(event);
    }
}