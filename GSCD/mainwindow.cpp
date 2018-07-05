
#include <QtGui>
#include <QList>

#include "mainwindow.h"
#include "mdichild.h"


const int InsertTextButton = 10;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    readSettings();


	createToolBox();

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(toolBox);
	layout->addWidget(mdiArea);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	setCentralWidget(widget);

    setWindowTitle(tr("GWD"));
    setUnifiedTitleAndToolBarOnMac(true);

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

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();																//create child when new a file
    //child->newFile();
    child->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();															//create child when open a file
        //if (child->loadFile(fileName)) {
        //    statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        //} else {
        //    child->close();
        //}
    }
}

void MainWindow::save()
{
    //if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
    //if (activeMdiChild() && activeMdiChild()->saveAs())
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
 #endif
}
void MainWindow::printPreview()
{
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

void MainWindow::about()
{
   QMessageBox::about(this, tr("About GWD"),
            tr("The <b>GWD</b> is Geographical wiring diagram for power system."));
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

    bool hasSelection = true;//(activeMdiChild() &&
                         //activeMdiChild()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
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
    MdiChild *child = new MdiChild(new DiagramScene(editMenu, this));
    mdiArea->addSubWindow(child);
	child->showMaximized();

    connect(child, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));

	connect(child, SIGNAL(itemInserted(DiagramItem*, DiagramScene*)),this, SLOT(itemInserted(DiagramItem*, DiagramScene*)));
	connect(child, SIGNAL(textInserted(QGraphicsTextItem*, DiagramScene*)),	this, SLOT(textInserted(QGraphicsTextItem*, DiagramScene*)));
	connect(this, SIGNAL(scaleChanged(const QString &)),child, SLOT(OnScaleChanged(const QString &)));

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


    toolbarAct = new QAction(tr("Show &Toolbar"), this);
    toolbarAct->setStatusTip(tr("Show or hide Toolbar"));
    connect(toolbarAct, SIGNAL(triggered()),
            mdiArea, SLOT(showToolbar()));

    statusbarAct = new QAction(tr("Show &Statusbar"), this);
    statusbarAct->setStatusTip(tr("Show or hide Statusbar"));
    connect(statusbarAct, SIGNAL(triggered()),
            mdiArea, SLOT(showStatusbar()));

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

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(toolbarAct);
    viewMenu->addAction(statusbarAct);
    viewMenu->addSeparator();

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

    tBar = addToolBar(tr("File"));
    tBar->addAction(newAct);
    tBar->addAction(openAct);
    tBar->addAction(saveAct);
	tBar->addSeparator();
    tBar->addAction(cutAct);
    tBar->addAction(copyAct);
    tBar->addAction(pasteAct);
	tBar->setMovable(false);

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(QIcon(":/images/pointer.png"));
	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

	pointerTypeGroup = new QButtonGroup(this);
	pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
	pointerTypeGroup->addButton(linePointerButton,
		int(DiagramScene::InsertLine));
	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(pointerGroupClicked(int)));

	sceneScaleCombo = new QComboBox;
	QStringList scales;
	scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
	sceneScaleCombo->addItems(scales);
	sceneScaleCombo->setCurrentIndex(2);
	connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
		this, SLOT(sceneScaleChanged(QString)));

	tBar->addSeparator();
	tBar->addWidget(pointerButton);
	tBar->addWidget(linePointerButton);
	tBar->addWidget(sceneScaleCombo);

	QList<QToolBar *> toolbars = findChildren<QToolBar *>("");

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createToolBox()
{
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(false);
	connect(buttonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(buttonGroupClicked(int)));
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(createCellWidget(tr("Station"),											//Conditional
		DiagramItem::Conditional), 0, 0);
	layout->addWidget(createCellWidget(tr("Generator"),										//Process
		DiagramItem::Step), 0, 1);
	layout->addWidget(createCellWidget(tr("Transformer"),										//IO
		DiagramItem::Io), 1, 0);
	//! [21]

	QToolButton *textButton = new QToolButton;
	textButton->setCheckable(true);
	buttonGroup->addButton(textButton, InsertTextButton);
	textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
		.scaled(30, 30)));
	textButton->setIconSize(QSize(50, 50));
	QGridLayout *textLayout = new QGridLayout;
	textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
	textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
	QWidget *textWidget = new QWidget;
	textWidget->setLayout(textLayout);
	layout->addWidget(textWidget, 1, 1);

	layout->setRowStretch(3, 10);
	layout->setColumnStretch(2, 10);

	QWidget *itemWidget = new QWidget;
	itemWidget->setLayout(layout);

	backgroundButtonGroup = new QButtonGroup(this);
	connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
		this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

	QGridLayout *backgroundLayout = new QGridLayout;
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
		":/images/background1.png"), 0, 0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
		":/images/background2.png"), 0, 1);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
		":/images/background3.png"), 1, 0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
		":/images/background4.png"), 1, 1);

	backgroundLayout->setRowStretch(2, 10);
	backgroundLayout->setColumnStretch(2, 10);

	QWidget *backgroundWidget = new QWidget;
	backgroundWidget->setLayout(backgroundLayout);


	//! [22]
	toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	toolBox->setMinimumWidth(itemWidget->sizeHint().width());
	toolBox->addItem(itemWidget, tr("Basic Shapes"));
	toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

QWidget *MainWindow::createCellWidget(const QString &text,
	DiagramItem::DiagramType type)
{

	DiagramItem item(type);
	QIcon icon(item.image());

	QToolButton *button = new QToolButton;
	button->setIcon(icon);
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	buttonGroup->addButton(button, int(type));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}
QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
	const QString &image)
{
	QToolButton *button = new QToolButton;
	button->setText(text);
	button->setIcon(QIcon(image));
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	backgroundButtonGroup->addButton(button);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}
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

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::buttonGroupClicked(int id)
{
	QList<QAbstractButton *> buttons = buttonGroup->buttons();
	foreach(QAbstractButton *button, buttons) {
		if (buttonGroup->button(id) != button)
			button->setChecked(false);
	}
	foreach(QMdiSubWindow *window, mdiArea->subWindowList()) {
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (!mdiChild) continue;

		DiagramScene *scene = mdiChild->scene();
		if (!scene) continue;

		if (id == InsertTextButton) {
			scene->setMode(DiagramScene::InsertText);
		}
		else {
			scene->setItemType(DiagramItem::DiagramType(id));
			scene->setMode(DiagramScene::InsertItem);
		}

	}
}
void MainWindow::pointerGroupClicked(int id)
{
	QList<QAbstractButton *> buttons = pointerTypeGroup->buttons();
	foreach(QAbstractButton *button, buttons) {
		if (pointerTypeGroup->button(id) != button)
			button->setChecked(false);
	}
	foreach(QMdiSubWindow *window, mdiArea->subWindowList()) {
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (!mdiChild) continue;

		DiagramScene *scene = mdiChild->scene();
		if (!scene) continue;

		scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
	}
}
void MainWindow::sceneScaleChanged(const QString &scale)
{
	emit scaleChanged(scale);
}
void MainWindow::itemInserted(DiagramItem *item, DiagramScene* scene)
{
	pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
	buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow::textInserted(QGraphicsTextItem *, DiagramScene* scene)
{
	buttonGroup->button(InsertTextButton)->setChecked(false);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}