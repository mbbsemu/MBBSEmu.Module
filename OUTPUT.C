#include "gcomm.h"
#include "majorbbs.h"
#include "mbbsemu.h"
#include "submod.h"
#include "output.h"

static int outputEntryHandler() {
  prfmsg(OPROMPT);
  return 1;
}

static int outputInputHandler() {
  int c = 0;
  char msg_13[] = "Test output\x13 with embedded 0x13\r\n\x00\x00\x00\x00";

  if (margc == 0) {
    prfmsg(OPROMPT);
    return 1;
  } 

  if (sscanf(margv[0], "0x%x", &c) == 0) {
    prfmsg(OPROMPT);
    return 1;
  }
  
  prf(msg_13);

  prfmsg(OOUTPUT, c);    

  btupbc(usrnum, c);
  msg_13[11] = c;
  sprintf(msg_13 + 29, "%X", c);
  msg_13[31] = '\r';
  msg_13[32] = '\n';
  msg_13[33] = 0;

  prf(msg_13);
 
  prfmsg(OPROMPT);
  
  return 1;
}

void initOutputSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = outputEntryHandler;
  subModule.onInput = outputInputHandler;
  
  registerSubModule(SUBMODULE_OUTPUT, &subModule); 
};
