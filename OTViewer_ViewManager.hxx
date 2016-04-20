// Author : Anthony Geay (EDF R&D)

#ifndef __OTVIEWER_VIEWMANAGER_HXX__
#define __OTVIEWER_VIEWMANAGER_HXX__

#include "SUIT_ViewManager.h"

namespace OTGUI
{
  class OTguiMdiArea;
}

class OTViewer_ViewManager : public SUIT_ViewManager
{
  Q_OBJECT
public:
  OTViewer_ViewManager( SUIT_Study*, SUIT_Desktop*, OTGUI::OTguiMdiArea *mdiArea);
  ~OTViewer_ViewManager() { }
protected slots:
  void onWindowActivated(SUIT_ViewWindow*);
private:
  SUIT_Desktop *_desktop;
};

#endif
