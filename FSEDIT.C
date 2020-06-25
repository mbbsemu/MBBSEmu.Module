#include "gcomm.h"
#include "majorbbs.h"
#include "globals.h"
#include "submod.h"
#include "mbbsemu.h"

static void copyToPrfptr(const char *str) {
  strcpy(prfptr, str);
  prfptr += strlen(str);
}

static int onDoneEditing(int quitex) {
  UserData *userData = getUserData();

  // full screen editing wipes our state, restore it
  usrptr->state = moduleState;
  usrptr->substt = 1;

  setmbk(mcv);
  
  if (quitex == 0) {
    prfmsg(OUTPUT);
    copyToPrfptr(userData->fullScreenEditorData.topic);
    *(prfptr++) = '\r';
    *prfptr = 0;
    outprf(usrnum);

    prfmsg(OUTPUT);
    copyToPrfptr(userData->fullScreenEditorData.text);
    *(prfptr++) = '\r';
    *prfptr = 0;
    outprf(usrnum);
  }
  return 1;
}

static int fullScreenEditorEntryHandler() {
  UserData *userData = getUserData();

  do {
    usrptr->substt = 0;

    bgncnc();
    switch (usrptr->substt) {
      case 0:
        cncchr();
        // print message here?
        strcpy(
          userData->fullScreenEditorData.text, 
          "Starter text - edit this\r");
        userData->fullScreenEditorData.topic[0] = 0;
        bgnedt(
          FSED_TEXT_LENGTH, 
          userData->fullScreenEditorData.text, 
          FSED_TOPIC_LENGTH, 
          userData->fullScreenEditorData.topic,
          onDoneEditing, 
          ED_CLRTOP);
        break;
    }
  } while (!endcnc());

  setmbk(mcv);

  return popSubModule();
}

static int fullScreenEditorInputHandler() {
  return 1;
}

void initFullScreenEditorSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = fullScreenEditorEntryHandler;
  subModule.onInput = fullScreenEditorInputHandler;
  
  registerSubModule(SUBMODULE_FULL_SCREEN_EDIT, &subModule); 
};

