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

#define FIRST_LINE "RLG327 MONSTER DESCRIPTION 1"
#define START_LINE "BEGIN MONSTER"
#define END_LINE "END"

void readDiceDesc(rollUp& desc, ifstream& file) {
  string num;
  while(file.peek() != '+') {
    num += file.get();
  }
  // get the +
  file.get();

  desc.base = stoi(num);
  num = "";

  while(file.peek() != 'd') {
    num += file.get();
  }
  // get the d
  file.get();

  desc.diceCount = stoi(num);
  num == "";

  while(file.peek() != '\n') {
    num += file.get();
  }
  desc.diceType = stoi(num);
}

void getWordsOnLine(vector<string> &storage, ifstream& file) {
  string word;
  do {
    file >> word;
    storage.push_back(word);
  } while (file.peek() != '\n');

}

vector<monsterDesc> parseMonsterDescFile(string fileLoc) {
  vector<monsterDesc> list;
  ifstream file(fileLoc);
  bool buildingMonster = true;
  string line;
  string word;

  // read first line
  getline(file, line);
  if(line.compare(FIRST_LINE) != 0) {
    exit(1);
  }

  while(buildingMonster) {
    monsterDesc desc;
    // read until it starts
    do {
      getline(file, line);
    } while(line.compare(START_LINE) != 0);

    do {
      file >> word;
      if(word.compare("NAME") == 0) {
        desc.name = word;
      }
      
      else if(word.compare("COLOR") == 0) {
        getWordsOnLine(desc.colors, file);
      }

      else if(word.compare("SPEED") == 0) {
        readDiceDesc(desc.speed, file);
      }

      else if(word.compare("ABIL") == 0) {
        getWordsOnLine(desc.abilities, file);
      }
      
      else if(word.compare("HP") == 0) {
        readDiceDesc(desc.hp, file);
      }

      else if(word.compare("DAM") == 0) {
        readDiceDesc(desc.ad, file);
      }

      else if(word.compare("RRTY") == 0) {
        readDiceDesc(desc.rarity, file);
      }

      else if(word.compare("SYMB") == 0) {
        file >> word;
        desc.symbol = word.c_str()[0];
      }
    } while(word.compare(START_LINE) != 0 && word.compare(END_LINE) != 0 && !file.eof());

    if(word.compare(END_LINE) == 0) {
      list.push_back(desc);
    }

    if(file.eof()) {
      buildingMonster = false;
    }
  }
  return list;
}

