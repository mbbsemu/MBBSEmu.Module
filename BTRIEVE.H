#ifndef __BTRIEVE_H_
#define __BTRIEVE_H_

void initBtrieveSubModule();

typedef struct {
  unsigned int version;      
  char userid[32];   
  long lvalue;       
  char svalue[32];   
  long autovalue;
} DBRECORD, *LPDBRECORD;

#endif
