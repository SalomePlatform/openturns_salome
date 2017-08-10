// Author : Anthony Geay (EDF R&D)

#include "OTSalomeGui.hxx"
#include "OTViewer_ViewManager.hxx"
#include "OTViewer_Viewer.hxx"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "CAM_Application.h"
#include "SUIT_ResourceMgr.h"
#include "PyConsole_Console.h"
#include "QtxPopupMgr.h"
#include "LightApp_Application.h"

#include "otgui/StudyTreeView.hxx"
//#include "otgui/OTguiMenuBar.hxx"
//#include "otgui/OTguiToolBar.hxx"
//#include "otgui/OTguiStatusBar.hxx"
#include "otgui/OTguiMdiArea.hxx"
#include "otgui/WelcomeWindow.hxx"
#include "otgui/YACSEvalSessionSingleton.hxx"

#include <QDockWidget>
#include <QSplitter>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <iostream>

OT::SalomeGui::SalomeGui(const QString& module_name):LightApp_Module(module_name),studyTree_(0),_dwTree(0),configurationDock_(0),_mdiArea(0)
{
}

/*QAction *createAction( const int id, const QString& text, const QIcon& icon,
  const QString& menu, const QString& tip, const int key,
  QObject* parent, const bool toggle, QObject* reciever, 
  const char* member, const QString& shortcutAction )*/
void OT::SalomeGui::initialize(CAM_Application *app)
{
  LightApp_Module::initialize(app);
  YACSEvalSession *yacsSession(OTGUI::YACSEvalSessionSingleton::Get());
  yacsSession->launchUsingCurrentSession();
  SUIT_Desktop *parent(application()->desktop());
  
  _dwTree = new QDockWidget(parent);
  _dwTree->setVisible(true);
  _dwTree->setWindowTitle("OpenTURNS Study tree view");
  _dwTree->setObjectName("OTTreeViewDock");
  //
  QSplitter *leftSideSplitter(new QSplitter(Qt::Vertical));
  leftSideSplitter->setStretchFactor(0, 8);
  studyTree_=new OTGUI::StudyTreeView(0);//_dwTree
  
  dockControllerWidget_ = new QDockWidget(tr("Current analysis"));
  dockControllerWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  leftSideSplitter->setStretchFactor(1, 2);
  
  configurationDock_=new QDockWidget(tr("Graph settings"));
  configurationDock_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  //
  connect(studyTree_, SIGNAL(graphWindowActivated(QWidget*)), this, SLOT(showGraphConfigurationTabWidget(QWidget*)));
  connect(studyTree_, SIGNAL(graphWindowDeactivated()), configurationDock_, SLOT(close()));
  parent->addDockWidget(Qt::LeftDockWidgetArea,_dwTree);
  leftSideSplitter->addWidget(studyTree_);
  leftSideSplitter->addWidget(configurationDock_);
  leftSideSplitter->addWidget(dockControllerWidget_);
  configurationDock_->close();
  dockControllerWidget_->close();
  _dwTree->setWidget(leftSideSplitter);
  //
  SUIT_ResourceMgr *resMgr(SUIT_Session::session()->resourceMgr());
  enum
  {
    agCreateStudy,
    agOpenStudy,
    agSaveStudy,
    agSaveAsStudy,
    agImportPy,
    agCount
  };
  createAction(agCreateStudy,tr("TOP_CREATE_STUDY"),resMgr->loadPixmap("OPENTURNS","document-new22x22.png"),
               tr("MEN_CREATE_STUDY"),tr("STB_CREATE_SUDY"),0,parent,false,studyTree_,SLOT(createNewOTStudy()));
  createAction(agOpenStudy,tr("TOP_OPEN_STUDY"),resMgr->loadPixmap("OPENTURNS","document-open.png"),
               tr("MEN_OPEN_STUDY"),tr("STB_OPEN_STUDY"),0,parent,false,studyTree_,SLOT(openOTStudy()));
  createAction(agSaveStudy,tr("TOP_SAVE_STUDY"),resMgr->loadPixmap("OPENTURNS","document-save.png"),
               tr("MEN_SAVE_STUDY"),tr("STB_SAVE_STUDY"),0,parent,false,studyTree_,SLOT(saveCurrentOTStudy()));
  createAction(agSaveAsStudy,tr("TOP_SAVEAS_STUDY"),resMgr->loadPixmap("OPENTURNS","document-save-as.png"),
               tr("MEN_SAVEAS_STUDY"),tr("STB_SAVEAS_STUDY"),0,parent,false,studyTree_,SLOT(saveAsCurrentOTStudy()));
  _importPythonAction = createAction(agImportPy,tr("TOP_IMPORT_PY"),
                          resMgr->loadPixmap("OPENTURNS","document-import.png"),
                          tr("MEN_IMPORT_PY"),tr("STB_IMPORT_PY"),0,parent,
                          false,this,SLOT(importPython()));
  //
  int fileMnu(createMenu(tr("MEN_OT_FILE"),-1,-1,50));
  int tbId(createTool(tr("MEN_OT_FILE"),QString("Ot")));
  for(int i=0;i<agCount;i++)
    {
      createMenu(i,fileMnu,10);
      createTool(i,tbId);
    }
  _mdiArea=new OTGUI::OTguiMdiArea;
  connect(studyTree_, SIGNAL(showWindow(OTguiSubWindow *)), _mdiArea, SLOT(showSubWindow(OTguiSubWindow *)));
  connect(studyTree_, SIGNAL(itemSelected(QStandardItem*)), _mdiArea, SLOT(showSubWindow(QStandardItem *)));
  connect(studyTree_, SIGNAL(removeSubWindow(QStandardItem *)), _mdiArea, SLOT(removeSubWindow(QStandardItem *)));
  connect(studyTree_, SIGNAL(analysisInProgressStatusChanged(bool)),
          this,       SLOT(updateActionsAvailability(bool)));

  // welcome page
  OTGUI::WelcomeWindow * welcomeWindow = new OTGUI::WelcomeWindow;
  leftSideSplitter->insertWidget(0,welcomeWindow);
  connect(welcomeWindow, SIGNAL(createNewOTStudy()), studyTree_, SLOT(createNewOTStudy()));
  connect(welcomeWindow, SIGNAL(openOTStudy()), studyTree_, SLOT(openOTStudy()));
  connect(welcomeWindow, SIGNAL(importPython()), this, SLOT(importPython()));

  connect(_mdiArea, SIGNAL(mdiAreaEmpty(bool)), welcomeWindow, SLOT(setVisible(bool)));
  connect(_mdiArea, SIGNAL(mdiAreaEmpty(bool)), _mdiArea, SLOT(setHidden(bool)));
}

void OT::SalomeGui::updateActionsAvailability(bool analysisInProgress)
{
  _importPythonAction->setDisabled(analysisInProgress);
}

void OT::SalomeGui::windows(QMap<int, int>& aMap) const
{
  aMap.insert( LightApp_Application::WT_PyConsole, Qt::BottomDockWidgetArea);
}

bool OT::SalomeGui::activateModule (SUIT_Study *study)
{
  bool isDone(LightApp_Module::activateModule(study));
  if(!isDone)
    return false;
  showView(true);
  setMenuShown(true);
  setToolShown(true);
  return isDone;
}

bool OT::SalomeGui::deactivateModule(SUIT_Study *study)
{
  setMenuShown(false);
  setToolShown(false);
  return LightApp_Module::deactivateModule(study);
}

void OT::SalomeGui::showView(bool toShow)
{
  LightApp_Application *anApp(getApp());
  OTViewer_ViewManager *viewMgr(dynamic_cast<OTViewer_ViewManager*>(anApp->getViewManager(OTViewer_Viewer::Type(),false)));
  if(!viewMgr)
    {
      viewMgr=new OTViewer_ViewManager(anApp->activeStudy(),anApp->desktop(),_mdiArea);
      anApp->addViewManager(viewMgr);
      connect(viewMgr,SIGNAL(lastViewClosed(SUIT_ViewManager*)),anApp,SLOT(onCloseView(SUIT_ViewManager*)));
    }
  OTViewer_ViewWindow *pvWnd(dynamic_cast<OTViewer_ViewWindow*>(viewMgr->getActiveView()));
  if(!pvWnd)
    {
      pvWnd = dynamic_cast<OTViewer_ViewWindow*>(viewMgr->createViewWindow());
      // this also connects to the pvserver and instantiates relevant PV behaviors
    }
  pvWnd->setVisible( toShow );
  if(toShow)
    pvWnd->setFocus();
}

void OT::SalomeGui::evalPyFile(const QString& fileName)
{
  LightApp_Application *anApp(getApp());
  if(!anApp)
    return;
  PyConsole_Console *cons(anApp->pythonConsole(false));
  if(!cons)
    return;
  QString cmd(QString("execfile(\"%1\")").arg(fileName));
  cons->exec(cmd);
  std::cerr << "***************************" << std::endl;
}

void OT::SalomeGui::importPython()
{
  if (studyTree_->model()->rowCount())
  {
    int ret = QMessageBox::warning(NULL, tr("Warning"),
                                   tr("Cannot import a Python script when other studies are opened.\nDo you want to continue and close the other studies?"),
                                   QMessageBox::Cancel | QMessageBox::Ok,
                                   QMessageBox::Ok);
    if (ret == QMessageBox::Ok)
    {
      bool allStudiesClosed = studyTree_->closeAllOTStudies();
      if (!allStudiesClosed)
        return;
    }
    else
      return;
  }

  QSettings settings;
  QString currentDir = settings.value("currentDir").toString();
  if (currentDir.isEmpty())
    currentDir = QDir::homePath();
  const QString fileName = QFileDialog::getOpenFileName(NULL, tr("Import Python..."),
                           currentDir,
                           tr("Python source files (*.py)"));

  if (!fileName.isEmpty())
  {
    QFile file(fileName);
    settings.setValue("currentDir", QFileInfo(fileName).absolutePath());

    // check
    if (!file.open(QFile::ReadOnly))
    {
      QMessageBox::warning(NULL, tr("Warning"),
                           tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
    }
    // load
    {
      evalPyFile(fileName);
    }
  }
}

void OT::SalomeGui::showGraphConfigurationTabWidget(QWidget *graph)
{
  configurationDock_->setWidget(graph);
  configurationDock_->show();
}

void OT::SalomeGui::showControllerWidget(QWidget* widget)
{
  dockControllerWidget_->setWidget(widget);
  dockControllerWidget_->show();
  configurationDock_->close();
}

// --- Export the module
extern "C"
{
  CAM_Module *createModule()
  {
    return new OT::SalomeGui("OPENTURNS");
  }

  char *getModuleVersion()
  {
    static const char OT_VERSION_STR[]="1.0.0";
    return const_cast<char*>(OT_VERSION_STR);
  }
}
