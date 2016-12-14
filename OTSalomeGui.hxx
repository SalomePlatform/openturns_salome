// Author : Anthony Geay (EDF R&D)

#ifndef __OTSALOMEGUI_HXX__
#define __OTSALOMEGUI_HXX__

#include "LightApp_Module.h"

#include "otgui/YACSEvalSessionSingleton.hxx"

#include <QObject>

namespace OTGUI
{
  class StudyTreeView;
  class OTguiMdiArea;
}

class QDockWidget;

namespace OT
{
  class SalomeGui : public LightApp_Module
  {
    Q_OBJECT
  public:
    SalomeGui(const QString& module_name);
    void initialize(CAM_Application *app);
    void windows(QMap<int, int>& aMap) const;
    bool activateModule(SUIT_Study *study);
    bool deactivateModule(SUIT_Study *study);
  private:
    void showView(bool toShow);
  public slots:
    void evalPyFile(const QString& fileName);
    void showGraphConfigurationTabWidget(QWidget *graph);
    void importPython();
  private:
    OTGUI::StudyTreeView *studyTree_;
    QDockWidget *_dwTree;
    QDockWidget *configurationDock_;
    OTGUI::OTguiMdiArea *_mdiArea;
    OTGUI::YACSEvalSessionSingleton _YACSSession;
  };
}

#endif
