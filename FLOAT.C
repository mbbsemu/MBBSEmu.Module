#include "gcomm.h"
#include "majorbbs.h"
#include "mbbsemu.h"
#include "submod.h"

static int floatInputHandler() {
  return 1;
}                        

double addDoubles(double x, double y) {
  return x + y;
}

float addFloats(float x, float y) {
  return x + y;
}

float subtractFloats(float x, float y) {
  return x - y;
}

float multiplyFloats(float x, float y) {
  return x * y;
}

float divideFloats(float x, float y) {
  return x / y;
}

static int floatEntryHandler() {
  char buf1[16];
  char buf2[16];

  sprintf(buf1, "%f", addFloats(1.5f, 2.2f));
  sprintf(buf2, "%lf", addDoubles(1.5, 2.2));

  prfmsg(FLTOUT, buf1, buf2);
  outprf(usrnum);  
  
  return popSubModule(); 
}

void initFloatSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = floatEntryHandler;
  subModule.onInput = floatInputHandler;
  
  registerSubModule(SUBMODULE_FLOAT, &subModule); 
};

