#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif
#ifndef DESC_H
  #define DESC_H
  #include "desc.h"
#endif
#ifndef FSTREAM
  #define FSTREAM
  #include <fstream>
#endif

void readDiceDesc(rollUp& desc, ifstream& file);

void getWordsOnLine(vector<string> &storage, ifstream& file);

vector<objectDesc> parseObjectDescFile(string fileLoc);

vector<monsterDesc> parseMonsterDescFile(string fileLoc);

void cleanMonstDesc(monsterDesc& desc);

void cleanObjDesc(objectDesc& desc);

char getLastChar(string str);

char getFirstChar(string str);

void cleanString(string& toClean);