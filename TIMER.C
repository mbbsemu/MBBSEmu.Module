#include "gcomm.h"
#include "majorbbs.h"
#include "timer.h"
#include "submod.h"
#include "mbbsemu.h"
#include "globals.h"

static int rtiCounter = 0;
static BOOL rtkickInstalled = FALSE;

static void timerFunction() {
  int usr;
  unsigned long now = hrtval();
  unsigned int seconds = (unsigned int)((now >> 16) & 0xFFFF);
  unsigned int fraction = (unsigned int)(now & 0xFFFF);

  setmbk(mcv);
  
  for (usr = 0; usr < nterms; ++usr) {
    if (user[usr].state == moduleState) {
      prfmsg(TMROUT, seconds, fraction, rtiCounter);
      outprf(usr);
    }
  }
  
  rstmbk();

  rtkick(1, timerFunction);
}

static void rtiFunction() {                       
  rtiCounter++;
}

static int timingEntryHandler() {
  if (!rtkickInstalled) {
    rtkick(1, timerFunction);
    rtihdlr(rtiFunction);
  
    prfmsg(TMRINT);

    rtkickInstalled = TRUE;
  }
  return 1;
}

static int timingInputHandler() {
  return 1;
}

void initTimerSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = timingEntryHandler;
  subModule.onInput = timingInputHandler;  
  
  registerSubModule(SUBMODULE_TIMING, &subModule); 
};

