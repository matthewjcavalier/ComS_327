#include "fileReading.h"

#define FIRST_LINE_MONST "RLG327 MONSTER DESCRIPTION 1"
#define START_LINE_MONST "BEGIN MONSTER"
#define START_LINE_OBJ "BEGIN OBJECT"
#define FIRST_LINE_OBJ "RLG327 OBJECT DESCRIPTION 1"
#define END_LINE "END"

monstBools::monstBools() {
  nameSet = false;
  descriptionSet = false;
  colorsSet = false;
  speedSet = false;
  abilitiesSet = false;
  hpSet = false;
  adSet = false;
  raritySet = false;
  symbolSet = false;
}

bool monstBools::isFilledOut() {
return nameSet && descriptionSet && 
        colorsSet && speedSet && 
        abilitiesSet && hpSet && 
        adSet && raritySet && 
        symbolSet;
}

objectBools::objectBools() {
  nameSet = false;
  descriptionSet = false;
  typeSet = false;
  colorsSet = false;
  hitBonusSet = false;
  damBonusSet = false;
  dodgeBonusSet = false;
  defencesBonusSet = false;
  weightSet = false;
  specialAttrSet = false;
  valueSet = false;
  isArtifactSet = false;
  raritySet = false;
  speedBuffSet = false;
}

bool objectBools::isFilledOut() {
  return nameSet && descriptionSet &&
          typeSet && colorsSet &&
          hitBonusSet && damBonusSet &&
          dodgeBonusSet && defencesBonusSet &&
          weightSet && specialAttrSet &&
          valueSet && isArtifactSet &&
          raritySet && speedBuffSet;
}

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

vector<objectDesc> parseObjectDescFile(string fileLoc) {
  vector<objectDesc> list;
  ifstream file(fileLoc);
  bool buildingObject = true;
  string line;
  string word;

  // read first line
  getline(file, line);
  if(line.compare(FIRST_LINE_OBJ) != 0) {
    exit(1);
  }

  while(buildingObject) {
    objectDesc desc;
    objectBools things;
    // read until it starts
    do {
      getline(file, line);
    } while(!file.eof() && line.compare(START_LINE_OBJ) != 0);

    if(!file.eof()) {
      do {
        file >> word;

        if(word.compare("NAME") == 0) {
          getline(file, desc.name);
          things.nameSet = true;
        }
        else if(word.compare("TYPE") == 0) {
          file >> desc.type;
          things.typeSet = true;
        }
        else if(word.compare("COLOR") == 0) {
          getWordsOnLine(desc.colors, file);
          things.colorsSet = true;
        }
        else if(word.compare("WEIGHT") == 0) {
          readDiceDesc(desc.weight, file);
          things.weightSet = true;
        }
        else if(word.compare("HIT") == 0) {
          readDiceDesc(desc.hitBonus, file);
          things.hitBonusSet = true;
        }
        else if(word.compare("DAM") == 0) {
          readDiceDesc(desc.damBonus, file);
          things.damBonusSet = true;
        }
        else if(word.compare("ATTR") == 0) {
          readDiceDesc(desc.specialAttr, file);
          things.specialAttrSet = true;
        }
        else if(word.compare("VAL") == 0) {
          readDiceDesc(desc.value, file);
          things.valueSet = true;
        }
        else if(word.compare("DODGE") == 0) {
          readDiceDesc(desc.dodgeBonus, file);
          things.dodgeBonusSet = true;
        }
        else if(word.compare("DEF") == 0) {
          readDiceDesc(desc.defenseBonus, file);
          things.defencesBonusSet = true;
        }
        else if(word.compare("SPEED") == 0) {
          readDiceDesc(desc.speedBuff, file);
          things.speedBuffSet = true;
        }
        else if(word.compare("ART") == 0) {
          file >> word;
          desc.isArtifact = word.compare("TRUE") == 0;
          things.isArtifactSet = true;
        }
        else if(word.compare("RRTY") == 0) {
          file >> word;
          desc.rarity = stoi(word);
          things.raritySet = true;
        }
        else if(word.compare("DESC") == 0) {
          while(file.peek() != '.') {
            getline(file, line);
            desc.description += line;
            desc.description += '\n';
          }
          things.descriptionSet = true;
        }
      } while(word.compare(START_LINE_OBJ) != 0 && word.compare(END_LINE) != 0 && !file.eof());
    }

    if(word.compare(END_LINE) == 0 && things.isFilledOut()) {
      cleanObjDesc(desc);
      list.push_back(desc);
    }

    if(file.eof()) {
      buildingObject = false;
    }
  }
  return list;
}

vector<monsterDesc> parseMonsterDescFile(string fileLoc) {
  vector<monsterDesc> list;
  ifstream file(fileLoc);
  bool buildingMonster = true;
  string line;
  string word;

  // read first line
  getline(file, line);
  if(line.compare(FIRST_LINE_MONST) != 0) {
    exit(1);
  }

  while(buildingMonster) {
    monsterDesc desc;
    monstBools things;
    // read until it starts
    do {
      getline(file, line);
    } while(!file.eof() && line.compare(START_LINE_MONST) != 0);

    if(!file.eof()) {
      do {
        file >> word;
        if(word.compare("NAME") == 0) {
          getline(file, desc.name);
          things.nameSet = true;
        }
        
        else if(word.compare("COLOR") == 0) {
          getWordsOnLine(desc.colors, file);
          things.colorsSet = true;
        }

        else if(word.compare("SPEED") == 0) {
          readDiceDesc(desc.speed, file);
          things.speedSet = true;
        }

        else if(word.compare("ABIL") == 0) {
          getWordsOnLine(desc.abilities, file);
          things.abilitiesSet = true;
        }
        
        else if(word.compare("HP") == 0) {
          readDiceDesc(desc.hp, file);
          things.hpSet = true;
        }

        else if(word.compare("DAM") == 0) {
          readDiceDesc(desc.ad, file);
          things.adSet = true;
        }

        else if(word.compare("RRTY") == 0) {
          file >> word;
          desc.rarity = stoi(word);
          things.raritySet = true;
        }

        else if(word.compare("SYMB") == 0) {
          file >> word;
          desc.symbol = word.c_str()[0];
          things.symbolSet = true;
        }

        else if(word.compare("DESC") == 0) {
          while(file.peek() != '.') {
            getline(file, line);
            desc.description += line;
            desc.description += '\n';
          }
          things.descriptionSet = true;
        }
      } while(word.compare(START_LINE_MONST) != 0 && word.compare(END_LINE) != 0 && !file.eof());
    }

    if(word.compare(END_LINE) == 0 && things.isFilledOut()) {
      cleanMonstDesc(desc);
      list.push_back(desc);
    }

    if(file.eof()) {
      buildingMonster = false;
    }
  }
  return list;
}

void cleanObjDesc(objectDesc& desc) {
  cleanString(desc.name);
  cleanString(desc.description);
}

void cleanMonstDesc(monsterDesc& desc) {
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