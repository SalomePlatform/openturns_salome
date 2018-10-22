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
#include "PVViewer_InitSingleton.h"
#include "PVViewer_ViewModel.h"

#include <otgui/YACSEvalSessionSingleton.hxx>
#include <otgui/StudyManager.hxx>
#include <otgui/PVServerManagerSingleton.hxx>

#include "OTPVServerManager.hxx"

#include <QtWidgets>
#include <QDockWidget>
#include <QSplitter>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <iostream>

OT::SalomeGui::SalomeGui(const QString& module_name):
LightApp_Module(module_name)
{
}

void OT::SalomeGui::initialize(CAM_Application *app)
{
  LightApp_Module::initialize(app);
  LightApp_Application* app2(dynamic_cast< LightApp_Application* >
                              ( SUIT_Session::session()->activeApplication() ));
  if( !app2 )
    return;

  YACSEvalSession *yacsSession(OTGUI::YACSEvalSessionSingleton::Get());
  yacsSession->launchUsingCurrentSession();
  SUIT_Desktop *parent(application()->desktop());
  PVViewer_InitSingleton::Init(parent);
  OTGUI::PVServerManagerSingleton::Init(new OTPVServerManager());

  _mainWindow = new OTGUI::MainWidget(parent);
  _mainWindow->setWindowTitle("OpenTURNS window");

  // set manager_
  _manager = new OTGUI::StudyManager(_mainWindow, this);
  connect(_manager, SIGNAL(commandExecutionRequested(QString)),
          this, SLOT(evalPyFile(QString)));

  int fileMnu(createMenu("OpenTURNS",-1,-1,10));
  int tbId(createTool(tr("OpenTURNS Toolbar"),QString("OtToolbar")));
  OTGUI::OTguiActions* actions = _mainWindow->getActions();

  createMenu(actions->newAction(),fileMnu);
  createTool(actions->newAction(),tbId);
  actions->newAction()->setShortcut(QKeySequence()); // remove OTGUI shortcut

  createMenu(actions->openAction(),fileMnu);
  createTool(actions->openAction(),tbId);
  actions->openAction()->setShortcut(QKeySequence());

  createMenu(actions->saveAction(),fileMnu);
  createTool(actions->saveAction(),tbId);
  actions->saveAction()->setShortcut(QKeySequence());

  createMenu(actions->saveAsAction(),fileMnu);
  createTool(actions->saveAsAction(),tbId);
  actions->saveAsAction()->setShortcut(QKeySequence());

  createMenu(actions->importPyAction(),fileMnu);
  createTool(actions->importPyAction(),tbId);
  actions->importPyAction()->setShortcut(QKeySequence());

  connect(_mainWindow->getMdiArea(), SIGNAL(errorMessageChanged(QString)),
          this, SLOT(showMessage(QString)));
}

void OT::SalomeGui::windows(QMap<int, int>& aMap) const
{
  aMap.insert( LightApp_Application::WT_PyConsole, Qt::BottomDockWidgetArea);
}

bool OT::SalomeGui::activateModule (SUIT_Study *study)
{
  bool isDone(LightApp_Module::activateModule(study));
  if(isDone)
  {
    LightApp_Application* app(dynamic_cast< LightApp_Application* >
                              ( SUIT_Session::session()->activeApplication() ));
    if( !app )
      return false;
    SUIT_ViewManager* aViewManager(app->getViewManager(PVViewer_Viewer::Type(),
                                                       false)); // create if necessary
    if(aViewManager)
      aViewManager->setShown(false);
    showView(true);
    setMenuShown(true);
    setToolShown(true);
  }
  return isDone;
}

bool OT::SalomeGui::deactivateModule(SUIT_Study *study)
{
  setMenuShown(false);
  setToolShown(false);
  //
  LightApp_Application* app(dynamic_cast< LightApp_Application* >( SUIT_Session::session()->activeApplication() ));
  if( !app )
    return false;
  SUIT_ViewManager *aViewManager(app->getViewManager(OTViewer_Viewer::Type(),false)); // create if necessary
  if(!aViewManager)
    return false;
  aViewManager->setShown(false);
  //
  return LightApp_Module::deactivateModule(study);
}

void OT::SalomeGui::showView(bool toShow)
{
  LightApp_Application *anApp(getApp());
  OTViewer_ViewManager *viewMgr(dynamic_cast<OTViewer_ViewManager*>(anApp->getViewManager(OTViewer_Viewer::Type(),false)));
  if(!viewMgr)
    {
      viewMgr=new OTViewer_ViewManager(anApp->activeStudy(),anApp->desktop(), _mainWindow);
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
  cons->execAndWait(fileName);
}

void OT::SalomeGui::showMessage(const QString & message)
{
  // ignore text between <>
  QString rawMessage;
  QString::const_iterator it;
  bool pass = false;
  for(it = message.begin(); it != message.end(); it++)
  {
    if(pass)
    {
      if( *it == '>')
        pass = false;
    }
    else
    {
      if( *it == '<')
        pass = true;
      else
        rawMessage.append(*it);
    }
  }
  application()->desktop()->statusBar()->showMessage(rawMessage);
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
    static const char OT_VERSION_STR[]="2.0.0";
    return const_cast<char*>(OT_VERSION_STR);
  }
}
