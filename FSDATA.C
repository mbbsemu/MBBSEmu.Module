#include "gcomm.h"
#include "majorbbs.h"
#include "globals.h"
#include "submod.h"
#include "mbbsemu.h"
#include "fsdbbs.h"

static char FIELD_SPEC[]="\
USERID(MIN=3) \
NAME(MIN=5) \
COMPANY \
ADDRESS2(MIN=5) \
ADDRESS3(MIN=5) \
ADDRESS4 \
PHONE(MIN=7) \
COMPUTER(ALT=IBM-PC ALT=Macintosh ALT=Apple/non-Mac ALT=OTHER MULTICHOICE) \
ANSIMODE(ALT=AUTO ALT=ON ALT=OFF MULTICHOICE) \
SCREENWIDTH(MIN=40 MAX=136) \
SCREENLENGTH(MIN=3 MAX=254) \
EDITORPREF(ALT=FSE ALT=LINE MULTICHOICE) \
PAUSEOUT(ALT=PAUSE ALT=CONTINUOUS MULTICHOICE) \
BIRTHDAY(MIN=6) \
SEX(ALT=MALE ALT=FEMALE MULTICHOICE) \
PASSWORD(SECRET MIN=1) \
DONE(ALT=SAVE ALT=EDIT ALT=QUIT MULTICHOICE)"; 

static char DEFAULT_FORMATS[]="\
USERID=%s%c\
NAME=%s%c\
COMPANY=%s%c\
ADDRESS2=%s%c\
ADDRESS3=%s%c\
ADDRESS4=%s%c\
PHONE=%s%c\
COMPUTER=%s%c\
ANSIMODE=%s%c\
SCREENWIDTH=%d%c\
SCREENLENGTH=%d%c\
EDITORPREF=%s%c\
PAUSEOUT=%s%c\
BIRTHDAY=%s%c\
SEX=%s%c\
PASSWORD=%s%c\
DONE=%s%c";

static int onVerifyField(int fieldNumber, char *answer) {
  return vfyadn(fieldNumber, answer);
}

static void processResults(int saved) {
  //fsdord(0-n); // returns index of multiple choice option
  
  if (!saved) {
    return;
  }

  prf(
    "\nChange count: %d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s (%d)\n" 
    "%s (%d)\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
    fsdscb->chgcnt,
    fsdnan(0),
    fsdnan(1),
    fsdnan(2),
    fsdnan(3),
    fsdnan(4),
    fsdnan(5),
    fsdnan(6),
    fsdnan(7), fsdord(7),
    fsdnan(8), fsdord(8),
    fsdnan(9),
    fsdnan(10),
    fsdnan(11),
    fsdnan(12),
    fsdnan(13),
    fsdnan(14),
    fsdnan(15));

  outprf(usrnum);
}

static void onDoneEditing(int saved) {
  // full screen editing wipes our state, restore it
  usrptr->state = moduleState;
  usrptr->substt = 1;

  setmbk(mcv);
  
  processResults(saved);

  popSubModule();

  // deliberately not calling outprf here since popping the module will print
  // the menu, but due to the stack system the input handler will get called
  // with 0 args, which will cause the menu to print anyways. If we call
  // outprf here, it'll print the menu twice
}

static void loadDefaults(char *buffer) {
  sprintf(
    buffer, 
    DEFAULT_FORMATS, 
    "Fisty", 0,
    "Joe Blow", 0,
    "Cool Company", 0,
    "1234 Drive", 0,
    "4321 Drive", 0,
    "Grand Rapids, MI", 0,
    "616-111-2323", 0,
    "IBM-PC", 0,
    "AUTO", 0,
    80, 0,
    24, 0,
    "FSE", 0,
    "PAUSE", 0,
    "01/01/1979", 0,
    "MALE", 0,
    "secret", 0,
    "", 0);
}

static int fullScreenDataEntryHandler() {
  UserData *userData = getUserData();
  char answers[512];

  loadDefaults(answers);
        
  userData->fullScreenEditorData.text[0] = 0;

  // calculate some bytes required, but I ignore it with the assumption
  // that what I have allocated already is large enough. Running it confirms
  // this, this method returns approxiamately 820 bytes while my buffer is
  // approxiamately 1900 bytes.
  fsdroom(FSE, FIELD_SPEC, 0);
  
  fsdroom(FSE, FIELD_SPEC, 1);
  fsdapr(
    userData->fullScreenEditorData.text, 
    FSED_TEXT_LENGTH, 
    answers);
  fsdrhd("Say what?");
  fsdbkg(fsdrft());
  fsdego(onVerifyField, onDoneEditing);

  return 1;
}

static int fullScreenDataInputHandler() {
  return 1;
}

void initFullScreenDataSubModule() {
  SubModule subModule;
  memset(&subModule, 0, sizeof(subModule));
  
  subModule.onEnter = fullScreenDataEntryHandler;
  subModule.onInput = fullScreenDataInputHandler;
  
  registerSubModule(SUBMODULE_FULL_SCREEN_DATA, &subModule); 
};
