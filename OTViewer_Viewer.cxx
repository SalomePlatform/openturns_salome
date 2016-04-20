// Author : Anthony Geay (EDF R&D)

#include "OTViewer_Viewer.hxx"

#include "otgui/OTguiMdiArea.hxx"

OTViewer_ViewWindow::OTViewer_ViewWindow(SUIT_Desktop *theDesktop, OTViewer_Viewer *theModel,OTGUI::OTguiMdiArea *mdiArea):SUIT_ViewWindow(theDesktop),myDesktop(theDesktop),myModel(theModel),_mdiArea(mdiArea)
{
  setViewManager(myModel->getViewManager());
  setCentralWidget(mdiArea);
}

OTViewer_ViewWindow::~OTViewer_ViewWindow()
{
  _mdiArea->setParent(0);
  setCentralWidget(0);
}

OTViewer_Viewer::OTViewer_Viewer(OTGUI::OTguiMdiArea *mdiArea):_mdiArea(mdiArea)
{
}

SUIT_ViewWindow *OTViewer_Viewer::createView(SUIT_Desktop *theDesktop)
{
  OTViewer_ViewWindow *aPVView(new OTViewer_ViewWindow(theDesktop,this,_mdiArea));
  return aPVView;
}
