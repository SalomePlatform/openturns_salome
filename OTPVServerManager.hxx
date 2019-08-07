#ifndef PVSERVERMANAGEROT_H
#define PVSERVERMANAGEROT_H

#include <persalys/PVServerManagerInterface.hxx>

class OTPVServerManager : public PERSALYS::PVServerManagerInterface
{
public:
  virtual pqServer* fetchServer(bool *isRemote=0);
};

#endif // PVSERVERMANAGEROT_H
