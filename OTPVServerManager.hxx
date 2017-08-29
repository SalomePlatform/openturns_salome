#ifndef PVSERVERMANAGEROT_H
#define PVSERVERMANAGEROT_H

#include <otgui/PVServerManagerInterface.hxx>

class OTPVServerManager : public OTGUI::PVServerManagerInterface
{
public:
  virtual pqServer* fetchServer(bool *isRemote=0);
};

#endif // PVSERVERMANAGEROT_H
