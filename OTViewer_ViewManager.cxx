// Author : Anthony Geay (EDF R&D)

#include "OTViewer_ViewManager.hxx"
#include "OTViewer_Viewer.hxx"

#include "SUIT_Desktop.h"

#include <iostream>

OTViewer_ViewManager::OTViewer_ViewManager( SUIT_Study* study, SUIT_Desktop* desk, OTGUI::OTguiMdiArea *mdiArea):SUIT_ViewManager(study,desk,new OTViewer_Viewer(mdiArea)),_desktop(desk)
{
  setTitle(tr("OTVIEWER_TITLE"));
  connect(desk, SIGNAL( windowActivated( SUIT_ViewWindow* ) ),this, SLOT( onWindowActivated( SUIT_ViewWindow* ) ) );
}

void OTViewer_ViewManager::onWindowActivated(SUIT_ViewWindow *view)
{
}
