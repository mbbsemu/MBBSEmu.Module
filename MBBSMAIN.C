#include "gcomm.h"
#include "majorbbs.h"
#include "stdio.h"
#include "mbbsemu.h"
#include "globals.h"
#include "submod.h"
#include "topmenu.h"
#include "echo.h"
#include "timer.h"
#include "fsedit.h"
#include "fsdata.h"

// Globals
int moduleState;
FILE *mcv = NULL;

static int initializeUserData() {
  memset(getUserData(), 0, sizeof(UserData));

  return pushSubModule(SUBMODULE_TOPMENU);
}

static int sttrou() {
  int ret = 1;
  SubModule* subModule;
  
  setmbk(mcv);

  if (usrptr->substt == 0) {
    usrptr->substt = 1;
    ret = initializeUserData();
  } else {
    // user has entered marc, margv
    if (margc == 1 && strcmp(margv[0], "x") == 0) {
      ret = popSubModule();
    } else { 
      subModule = getSubModule();
      if (subModule->onInput != NULL) {
        ret = subModule->onInput();
      }
    }
  }

  rstmbk();
  outprf(usrnum);
  return ret; // return 0 to exit module
};

static void exit_routine();
   
struct module example = {
  "", // should be empty and gotten from .MDF
  NULL,   // user logon routine
  sttrou, // input routine
  NULL, // status-input routine
  NULL, // injoth"
  NULL, // user logoff
  NULL, // hangup
  NULL, // midnight cleanup
  NULL, // delete account routine
  exit_routine}; // finish up routine

static void initSubModules() {
  initTopMenuSubModule();
  initEchoSubModule();
  initTimerSubModule();
  initFullScreenEditorSubModule();
  initFullScreenDataSubModule();
}

void EXPORT init__mbbsemu() {
  initSubModules();
  // get name from .MDF file
  stzcpy(example.descrp, gmdnam("MBBSEMU.MDF"), MNMSIZ);

  moduleState = register_module(&example);        
  
  mcv = opnmsg("MBBSEMU.MCV");

  // allocate memory per user
  dclvda(sizeof(UserData));

  setmbk(mcv); // pushes
  LOG(getmsg(STARTUP));
  rstmbk();  // pops
};

static void exit_routine() {
  if (mcv != NULL) {
    clsmsg(mcv);
    mcv = NULL;
  }

  shocst(ACCOUNT_LOG, "Shutting down");
}

