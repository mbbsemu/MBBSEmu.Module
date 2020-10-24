#include "gcomm.h"
#include "majorbbs.h"
#include "stdio.h"
#include "mbbsemu.h"
#include "globals.h"
#include "submod.h"
#include "topmenu.h"
#include "echo.h"
#include "float.h"
#include "timer.h"
#include "fsedit.h"
#include "fsdata.h"
#include "btrieve.h"

// Globals
int moduleState;
FILE *mcv = NULL;
BTVFILE* bdb = NULL;

static int initializeUserData() {
  memset(getUserData(), 0, sizeof(UserData));
  
  return pushSubModule(SUBMODULE_TOPMENU);
}

static void cleanupUserData() {
  
}

static int sttrou() {
  int doDefaultInputProcessing = 1;
  int ret = 1;
  SubModule* subModule;
  
  setmbk(mcv);
  setbtv(bdb);

  if (usrptr->substt == 0) {
    usrptr->substt = 1;
    ret = initializeUserData();
  } else {
    // user has entered marc, margv
    subModule = getSubModule();
    doDefaultInputProcessing = 
      subModule == NULL || 
      ((subModule->flags & NO_DEFAULT_INPUT_PROCESSING) == 0);

    if (doDefaultInputProcessing && margc == 1 && strcmp(margv[0], "x") == 0) {
      ret = popSubModule();
    } else { 
      if (subModule->onInput != NULL) {
        ret = subModule->onInput();
      }
    }
  }

  rstmbk();
  rstbtv();

  outprf(usrnum);

  if (ret == 0) {
    cleanupUserData();
  }
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
  initFloatSubModule();
  initBtrieveSubModule();
}

void EXPORT init__mbbsemu() {
  initSubModules();
  // get name from .MDF file
  stzcpy(example.descrp, gmdnam("MBBSEMU.MDF"), MNMSIZ);

  moduleState = register_module(&example);        
  
  mcv = opnmsg("MBBSEMU.MCV");

  bdb = opnbtv("MBBSEMU.DAT", sizeof(DBRECORD));

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

  if (bdb != NULL) {
    clsbtv(bdb);
    bdb = NULL;
  }

  shocst(ACCOUNT_LOG, "Shutting down");
}
