#include "fileReading.h"

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
  num.clear();

  while(file.peek() != 'd') {
    num += file.get();
  }
  // get the d
  file.get();

  desc.diceCount = stoi(num);
  num.clear(); 

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
    } while(!file.eof() && line.compare(START_LINE) != 0);

    if(!file.eof()) {
      do {
        file >> word;
        if(word.compare("NAME") == 0) {
          getline(file, desc.name);
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
          file >> word;
          desc.rarity = stoi(word);
        }

        else if(word.compare("SYMB") == 0) {
          file >> word;
          desc.symbol = word.c_str()[0];
        }

        else if(word.compare("DESC") == 0) {
          while(file.peek() != '.') {
            getline(file, line);
            desc.description += line;
            desc.description += '\n';
          }
        }
      } while(word.compare(START_LINE) != 0 && word.compare(END_LINE) != 0 && !file.eof());
    }

    if(word.compare(END_LINE) == 0 && !file.eof()) {
      cleanDesc(desc);
      list.push_back(desc);
    }

    if(file.eof()) {
      buildingMonster = false;
    }
  }
  return list;
}

void cleanDesc(monsterDesc& desc) {
  cleanString(desc.name);
  cleanString(desc.description);
}

void cleanString(string& toClean) {
  while(getFirstChar(toClean) == ' ' || getFirstChar(toClean) == '\n') {
    toClean.erase(0,1);
  }
  while(getLastChar(toClean) == ' ' || getLastChar(toClean) == '\n') {
    toClean.resize(toClean.length() - 1);
  }
}

char getLastChar(string str) {
  return str[str.length() - 1];
}

char getFirstChar(string str) {
  return str[0];
}