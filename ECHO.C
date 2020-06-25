#include "gcomm.h"
#include "majorbbs.h"
#include "mbbsemu.h"
#include "submod.h"

static void printArguments() {
  // normally data is in margc, margv[], but we restore input to raw input
  rstrin();

  memcpy(prfptr, input, inplen);
  prfptr += inplen;
  *(prfptr++) = '\r';
  *prfptr = 0;
}

static int echoEntryHandler() {
  prfmsg(PROMPT);
  return 1;
}

static int echoInputHandler() {
  if (margc == 0) {
    prfmsg(PROMPT);
    return 1;
  } 

  prfmsg(OUTPUT);    
  printArguments();
  prfmsg(PROMPT);
  
  return 1;
}

void initEchoSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = echoEntryHandler;
  subModule.onInput = echoInputHandler;
  
  registerSubModule(SUBMODULE_ECHO, &subModule); 
};

