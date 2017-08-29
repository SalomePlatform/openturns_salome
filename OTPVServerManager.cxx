#include <openturns/Exception.hxx>

#include <pqActiveObjects.h>
#include "pqApplicationCore.h"
#include "pqServerManagerModel.h"

#include "OTPVServerManager.hxx"

pqServer* OTPVServerManager::fetchServer(bool *isRemote)
{
  pqServer *ret(pqApplicationCore::instance()->getServerManagerModel()->findServer(pqServerResource("builtin:")));
  if(!ret)
    throw OT::InvalidArgumentException(HERE) << "Ooops no server !";
  if(isRemote)
    {
      if(!ret)
        *isRemote=false;
      else
        *isRemote=ret->isRemote();
    }
  return ret;
}
