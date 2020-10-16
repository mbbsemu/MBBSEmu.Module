#include "gcomm.h"
#include "majorbbs.h"
#include "btrieve.h"
#include "submod.h"
#include "mbbsemu.h"
#include "globals.h"

#define STATE_TOP 1

#define STATE_ADD 3

#define STATE_ID  4

#define STATE_STRING 5

#define STATE_DELETE 6

#define STATE_PRINT 7

#define STATE_ADD_ID 8
#define STATE_ADD_STRING 9

#define STATE_DELETE_ID 10
#define STATE_DELETE_STRING 11

static unsigned long hash(const char *value) {
  unsigned long hash = 7;
  const char *c;

  for (c = value; *c; ++c) {
    hash = hash * 31 + *c;    
  }
  return hash;
}

static void ladd(long value) {
  DBRECORD dbRecord;
  memset(&dbRecord, 0, sizeof(dbRecord));

  strcpy(dbRecord.userid, usaptr->userid);
  dbRecord.lvalue = value;
  strcpy(dbRecord.svalue, ltoa(value));

  if (!dinsbtv(&dbRecord)) {
    prfmsg(INSFAIL);
  }
}

static void sadd(const char *value) {
  DBRECORD dbRecord;
  memset(&dbRecord, 0, sizeof(dbRecord));

  strcpy(dbRecord.userid, usaptr->userid);
  dbRecord.lvalue = hash(value);
  strcpy(dbRecord.svalue, value);

  if (!dinsbtv(&dbRecord)) {
    prfmsg(INSFAIL);
  }
}

static void dumpRecords() {
  char value[16];
  char inc[16];
  DBRECORD dbRecord;

  prfmsg(DBPRINT);

  memset(&dbRecord, 0, sizeof(dbRecord));
  
  if (slobtv(&dbRecord)) {
    do {
      strcpy(value, ltoa(dbRecord.lvalue));
      strcpy(inc, ltoa(dbRecord.autovalue));

      prfmsg(DBVPRINT, dbRecord.userid, value, dbRecord.svalue, inc);

      memset(&dbRecord, 0, sizeof(dbRecord));
    } while (snxbtv((char*) &dbRecord));
  }
   
  prfmsg(DBPRINTE);
}

static void setState(int state) {
  usrptr->substt = state;
  switch (state) {
    case STATE_TOP:
      prfmsg(BMENU);
      break;
    case STATE_ADD:
      prfmsg(BADDMENU);
      break;
    case STATE_ADD_ID:
    case STATE_DELETE_ID:
      prfmsg(BID);
      break;
    case STATE_ADD_STRING:
    case STATE_DELETE_STRING:
      prfmsg(BSTR);
      break;
    case STATE_DELETE:
      prfmsg(BDELMENU);
      break;
    case STATE_PRINT:   
      dumpRecords();
      break;
  }
}

static int btrieveEntryHandler() { 
  setState(STATE_TOP);
  return 1;
}

static void btrieveExitHandler() {
  usrptr->substt = 1;
}

static int btrieveInputHandler() {
  char c;
  
  do {
    bgncnc();
    switch (usrptr->substt) {
      case STATE_TOP:
        c = cncchr();
        if (c == 'A') {
          setState(STATE_ADD);
        } else if (c == 'D') {
          setState(STATE_DELETE);
        } else if (c == 'P') {
          setState(STATE_PRINT);
        } else if (c == 'X') {
          cncall();
          popSubModule();
        } else {
          prfmsg(BMENU);
        }
        break;
      case STATE_ADD:
        c = cncchr();
        if (c == 'I') {
          setState(STATE_ADD_ID);
        } else if (c == 'S') {
          setState(STATE_ADD_STRING);
        } else if (c == 'X') {
          setState(STATE_TOP);
        } else {
          prfmsg(BADDMENU);
        }
        break;
      case STATE_ADD_ID:
        ladd(cnclon());
        setState(STATE_TOP);
        break;
      case STATE_ADD_STRING:
        sadd(cncwrd());
        setState(STATE_TOP);
        break;
      case STATE_DELETE:
        c = cncchr();
        if (c == 'I') {
          setState(STATE_DELETE_ID);
        } else if (c == 'S') {
          setState(STATE_DELETE_STRING);
        } else if (c == 'X') {
          setState(STATE_TOP);
        } else {
          prfmsg(BDELMENU);
        }
        break;
      case STATE_DELETE_ID:
        LOG1("You deleted %ld", cnclon());
        setState(STATE_TOP);
        break;
      case STATE_DELETE_STRING:
        LOG1("You deleted '%s'", cncwrd());
        setState(STATE_TOP);
        break;
      case STATE_PRINT:
        if (cncchr() == 'X') {
          setState(STATE_TOP);
        }
        break;
     }
  } while (!endcnc());
  
  return 1;
}

void initBtrieveSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));

  subModule.onEnter = btrieveEntryHandler;
  subModule.onExit = btrieveExitHandler;
  subModule.onInput = btrieveInputHandler;
  subModule.flags = NO_DEFAULT_INPUT_PROCESSING;

  registerSubModule(SUBMODULE_BTRIEVE, &subModule);
}
