#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdio.h>

// The module state returned from register_module, stored in usrptr->state
extern int moduleState;

// Pointer to our MCV file
extern FILE *mcv;

extern BTVFILE *bdb;

#define ACCOUNT_LOG "MBBSEmu"
#define LOG(s) shocst(ACCOUNT_LOG, s)
#define LOG1(s, a) do { char buf[128]; sprintf(buf, s, a); LOG(buf); } while (0)
#define LOG2(s, a, b) do { char buf[128]; sprintf(buf, s, a, b); LOG(buf); } while (0)
#define USR_LOG(s) do { prf(s); outprf(usrnum); } while (0)
#define USR_LOG1(fmt, a) do { prf(fmt, a); outprf(usrnum); } while (0)
#define USR_LOG2(fmt, a, b) do { prf(fmt, a, b); outprf(usrnum); } while (0)

#define FSED_TOPIC_LENGTH 51
#define FSED_TEXT_LENGTH (24 * 80 + 1)

typedef struct _FullScreenEditorData {
  char topic[FSED_TOPIC_LENGTH];
  char text[FSED_TEXT_LENGTH];
} FullScreenEditorData;

typedef struct _StateData {
  int numItems;
  int stack[8];
} StateData;

// This data is stored inside vda memory on a per-user basis
typedef struct _UserData {
  StateData stateData;
  FullScreenEditorData fullScreenEditorData;
} UserData;

#define getUserData() ((UserData*) vdaptr)

#endif
