// Author : Anthony Geay (EDF R&D)

#ifndef __OTVIEWER_VIEWER_HXX__
#define __OTVIEWER_VIEWER_HXX__

#include "SUIT_ViewModel.h"
#include "SUIT_ViewWindow.h"

namespace OTGUI
{
  class OTguiMdiArea;
}

class OTViewer_Viewer;

class OTViewer_ViewWindow : public SUIT_ViewWindow
{
public:
  OTViewer_ViewWindow(SUIT_Desktop*,OTViewer_Viewer *,OTGUI::OTguiMdiArea *);
  virtual ~OTViewer_ViewWindow();
private:
  SUIT_Desktop *myDesktop;
  OTViewer_Viewer *myModel;
  OTGUI::OTguiMdiArea *_mdiArea;
};

class OTViewer_Viewer : public SUIT_ViewModel
{
public:
  OTViewer_Viewer(OTGUI::OTguiMdiArea *mdiArea);
  virtual ~OTViewer_Viewer() { }
  virtual SUIT_ViewWindow* createView(SUIT_Desktop* theDesktop);
  virtual QString getType() const { return Type(); }
  static QString Type() { return "OTT"; }
private:
  OTGUI::OTguiMdiArea *_mdiArea;
};

#endif

