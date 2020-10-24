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

#define STATE_KEY_NUMBER 12
#define STATE_KEY_INT 13
#define STATE_KEY_STRING 14

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

static int mbbs_qeqbtv(char *rec, char *b, int c) { return qeqbtv(b, c); }
static int mbbs_qgtbtv(char *rec, char *b, int c) { return qgtbtv(b, c); }
static int mbbs_qgebtv(char *rec, char *b, int c) { return qgebtv(b, c); }
static int mbbs_qltbtv(char *rec, char *b, int c) { return qltbtv(b, c); }
static int mbbs_qlebtv(char *rec, char *b, int c) { return qlebtv(b, c); }
static int mbbs_qlobtv(char *rec, char *b, int c) { return qlobtv(c); }
static int mbbs_qhibtv(char *rec, char *b, int c) { return qhibtv(c); }

static void mbbs_gcrbtv(char *a, int b) { gcrbtv(a, b); }
static int mbbs_qnxbtv(char *rec) { return qnxbtv(); }

static int mbbs_slobtv(char *rec, char *a, int b) { return slobtv(rec); }
static int mbbs_snxbtv(char *rec) { return snxbtv(rec); }

typedef struct _KEY {
  int offset;
  int length;
} KEY;

static const KEY keys[] = { {2, 32}, {34, 4}, {38, 32}, {70, 4} };

static void dumpRecords()
{
  char value[16];
  char inc[16];
  BtrieveData *pData = &getUserData()->btrieveData;
  int keylen = keys[pData->keyNumber].length;
  DBRECORD dbRecord;
  char *keyInRecord = ((char*)&dbRecord) + keys[pData->keyNumber].offset;
 
  prfmsg(DBPRINT, pData->keyNumber, keylen);

  memset(&dbRecord, 0, sizeof(dbRecord));
  
  if (pData->queryFunc((char*)&dbRecord, pData->keyData, pData->keyNumber)) {
    do {
      if (pData->acquireFunc) {
        char buf[32];

        pData->acquireFunc((char*)&dbRecord, pData->keyNumber);
        pData->acquireFunc(NULL, pData->keyNumber);

        if (pData->verifyData && memcmp(&dbRecord, bdb->data, sizeof(dbRecord))) 
          catastro("bdb->data differs from dbRecord");

        memcpy(buf, keyInRecord, keylen);
        if (pData->verifyKey && memcmp(buf, bdb->key, keylen))
          catastro("bdb->key differs from dbRecord");
        if (pData->verifyKey && bdb->lastkn != pData->keyNumber)
          catastro("bdb->lastkn != keynum");
      }
    
      strcpy(value, ltoa(dbRecord.lvalue));
      strcpy(inc, ltoa(dbRecord.autovalue));

      prfmsg(DBVPRINT, dbRecord.userid, value, dbRecord.svalue, inc);

      memset(&dbRecord, 0, sizeof(dbRecord));
    } while (pData->nextFunc((char*)&dbRecord));
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

static void setQuery(int digit) {
  BtrieveData *pData = &getUserData()->btrieveData;
  pData->verifyData = pData->verifyKey = 1;
  
  switch (digit) {
    case 1:
    case '1': // physical
      pData->queryFunc = mbbs_slobtv;
      pData->acquireFunc = NULL;
      pData->nextFunc = mbbs_snxbtv;
      break;
    case 2:
    case '2': // equal 
      pData->queryFunc = mbbs_qeqbtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 3:
    case '3': // greater
      pData->queryFunc = mbbs_qgtbtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 4:
    case '4': // greater or equal
      pData->queryFunc = mbbs_qgebtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 5:
    case '5': // less than
      pData->queryFunc = mbbs_qltbtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 6:
    case '6': // less than or equal
      pData->queryFunc = mbbs_qlebtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 7:
    case '7': // lowest
      pData->queryFunc = mbbs_qlobtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
    case 8:
    case '8': // highest
      pData->queryFunc = mbbs_qhibtv;
      pData->acquireFunc = mbbs_gcrbtv;
      pData->nextFunc = mbbs_qnxbtv;
      break;
  }
}

static int btrieveEntryHandler() { 
  memset(&getUserData()->btrieveData, 0, sizeof(BtrieveData));

  setState(STATE_TOP);
  setQuery(1); // physical default
  return 1;
}

static void btrieveExitHandler() {
  usrptr->substt = 1;
}

static int btrieveInputHandler() {
  char c;
  long l;
  char *s;
  
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
        } else if (isdigit(c)) {
          setQuery(c);
          prfmsg(BMENU);
        } else if (c == 'K') {
          setState(STATE_KEY_NUMBER);
        } else if (c == 'I') {
          setState(STATE_KEY_INT);
        } else if (c == 'S') {
          setState(STATE_KEY_STRING);
        } else {
          prfmsg(BMENU);
        }
        break;
      case STATE_KEY_INT:
        l = cnclon();
        memset(getUserData()->btrieveData.keyData, 0, 32);
        memcpy(getUserData()->btrieveData.keyData, &l, sizeof(long));

        setState(STATE_TOP);
        break;
      case STATE_KEY_NUMBER:
        getUserData()->btrieveData.keyNumber = cncint();
        
        setState(STATE_TOP);
        break;
      case STATE_KEY_STRING:
        s = cncwrd();
        if (s) {
          memset(getUserData()->btrieveData.keyData, 0, 32);
          strcpy(getUserData()->btrieveData.keyData, s);
        }

        setState(STATE_TOP);
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
