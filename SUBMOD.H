#ifndef __SUBMODULE_H__
#define __SUBMODULE_H__

#define SUBMODULE_INVALID 0
#define SUBMODULE_TOPMENU 1
#define SUBMODULE_ECHO 2
#define SUBMODULE_TIMING 3
#define SUBMODULE_FULL_SCREEN_EDIT 4
#define SUBMODULE_FULL_SCREEN_DATA 5
#define SUBMODULE_FLOAT 6
#define SUBMODULE_BTRIEVE 7
// last one 
#define SUBMODULE_LAST_INDEX 8

int pushSubModule(int newSubModule);
int popSubModule(); 

#define NO_DEFAULT_INPUT_PROCESSING (1 << 0)

typedef struct _SubModule {
  // return 0 to exit the module
  int (*onEnter)();
  // called when the module is being pushed off the stack
  void (*onExit)();
  // return 0 to exit the module
  int (*onInput)();

  int flags;
} SubModule;

SubModule* getSubModule();

void registerSubModule(int index, SubModule *subModule);

#endif
