#include "gcomm.h"
#include "majorbbs.h"
#include "stdio.h"
#include "mbbsemu.h"
#include "submod.h"

static int topMenuEntryHandler() {
  prfmsg(MENU);
  return 1;
}

static int topMenuInputHandler() {
  if (margc == 0) {
    prfmsg(MENU);
    return 1;
  }

  if (margc == 1) {
    if (sameas(margv[0], "e")) {
      return pushSubModule(SUBMODULE_ECHO);
    } else if (sameas(margv[0], "t")) {
      return pushSubModule(SUBMODULE_TIMING);
    } else if (sameas(margv[0], "f")) {
      return pushSubModule(SUBMODULE_FULL_SCREEN_EDIT);
    } else if (sameas(margv[0], "d")) {
      return pushSubModule(SUBMODULE_FULL_SCREEN_DATA);
    } else if (sameas(margv[0], "s")) {
      return pushSubModule(SUBMODULE_FLOAT);
    } else if (sameas(margv[0], "B")) {
      return pushSubModule(SUBMODULE_BTRIEVE);
    }
  }
  return 1;
}

void initTopMenuSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = topMenuEntryHandler;
  subModule.onInput = topMenuInputHandler;
  
  registerSubModule(SUBMODULE_TOPMENU, &subModule); 
};
