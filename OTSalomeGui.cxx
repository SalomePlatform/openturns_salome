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
#include <otgui/MainWidget.hxx>
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
  PVViewer_InitSingleton::Init(parent, app2->logWindow());
  OTGUI::PVServerManagerSingleton::Init(new OTPVServerManager());
  _mainWindow = new OTGUI::MainWidget(parent);
  _mainWindow->setWindowTitle("OpenTURNS window");

  int fileMnu(createMenu(tr("MEN_OT_FILE"),-1,-1,50));
  int tbId(createTool(tr("MEN_OT_FILE"),QString("Ot")));
  OTGUI::OTguiActions* actions = _mainWindow->getActions();
  int idx = 0;
  createMenu(actions->newAction(),idx,fileMnu,10);
  createTool(actions->newAction(),idx,tbId);
  idx++;

  createMenu(actions->openAction(),idx,fileMnu,10);
  createTool(actions->openAction(),idx,tbId);
  idx++;

  createMenu(actions->saveAction(),idx,fileMnu,10);
  createTool(actions->saveAction(),idx,tbId);
  idx++;

  createMenu(actions->saveAsAction(),idx,fileMnu,10);
  createTool(actions->saveAsAction(),idx,tbId);
  idx++;

  createMenu(actions->importPyAction(),idx,fileMnu,10);
  createTool(actions->importPyAction(),idx,tbId);
  idx++;
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
