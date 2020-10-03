#include "gcomm.h"
#include "majorbbs.h"
#include "submod.h"
#include "globals.h"

static SubModule handlers[SUBMODULE_LAST_INDEX];

void registerSubModule(int index, SubModule *subModule) {
  if (subModule->flags & NO_DEFAULT_INPUT_PROCESSING && 
    subModule->onInput == NULL) {
    catastro("must set onInput if NO_DEFAULT_INPUT_PROCESSING set");
  }

  handlers[index] = *subModule;
}

static int switchToModule(SubModule *oldModule, SubModule *newModule) {
  if (oldModule != NULL && oldModule->onExit != NULL) {
    oldModule->onExit();
  }

  return (newModule->onEnter == NULL) ? 1 : newModule->onEnter();
}

int pushSubModule(int newSubModule) {
  UserData *userData = getUserData();
  SubModule *oldModule = getSubModule();

  userData->stateData.stack[userData->stateData.numItems++] = newSubModule;

  return switchToModule(oldModule, handlers + newSubModule);
}

int popSubModule() {
  UserData* userData = getUserData();
  SubModule *oldModule = getSubModule();
  int newSubModule;

  if (userData->stateData.numItems <= 1) {
    if (oldModule != NULL && oldModule->onExit != NULL) {
      oldModule->onExit();
    }
    userData->stateData.numItems = 0;  
    return 0;
  }

  newSubModule = userData->stateData.stack[--userData->stateData.numItems - 1];

  return switchToModule(oldModule, handlers + newSubModule);
}

SubModule* getSubModule() {
  UserData* userData = getUserData();
  int currentModule;

  if (userData->stateData.numItems == 0) {
    return NULL;
  }

  currentModule =
    userData->stateData.stack[userData->stateData.numItems - 1];
  
  return handlers + currentModule;
}

