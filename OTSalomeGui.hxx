// Author : Anthony Geay (EDF R&D)

#ifndef __OTSALOMEGUI_HXX__
#define __OTSALOMEGUI_HXX__

#include "LightApp_Module.h"

#include <QObject>


namespace OTGUI
{
  class MainWidget;
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
  private:
    OTGUI::MainWidget * _mainWindow;
  };
}

#endif
