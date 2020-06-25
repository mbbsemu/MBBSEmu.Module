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
    if (!strcmp(margv[0], "e") || !strcmp(margv[0], "E")) {
      return pushSubModule(SUBMODULE_ECHO);
    } else if (!strcmp(margv[0], "t") || !strcmp(margv[0], "T")) {
      return pushSubModule(SUBMODULE_TIMING);
    } else if (!strcmp(margv[0], "f") || !strcmp(margv[0], "F")) {
      return pushSubModule(SUBMODULE_FULL_SCREEN_EDIT);
    } else if (!strcmp(margv[0], "d") || !strcmp(margv[0], "D")) {
      return pushSubModule(SUBMODULE_FULL_SCREEN_DATA);
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
