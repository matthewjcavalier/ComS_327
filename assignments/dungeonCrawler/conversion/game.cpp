#define GAME_H
#include "game.h"

Settings settings;

using namespace std;

/**
 * @brief Comparitor used in the priority queue
 * 
 */
struct Compare {
  bool operator()(Character* a, Character* b) {
    return a->nextEventTime > b->nextEventTime;
  }
};

/**
 * @brief Prints the description for an input rollUp attribute
 * 
 * TODO: Remove this function before final game shipment
 * 
 * @param fruit rollUp
 */
void printDice(rollUp fruit) {
  cout<<fruit.base<<"+"<<fruit.diceCount<<"d"<<fruit.diceType<<endl;
}

int main(int argc, char* argv[]) {
  setSettings(argc, argv);
  cout<<"Using Seed: "<< settings.seed <<endl;
  // get the basic dungeon
  Dungeon dun = dungeonInit();

  vector<monsterDesc> monsterDescs = parseMonsterDescFile(settings.monsterDescLoc);
  /**
   * DEBUGGING print out monster descriptions
   * TODO : remove this in release version
   */
  cout<<endl<<endl<<"/**"<<endl<<" * Monster descriptions" <<endl<<" */"<<endl<<endl;
  for(monsterDesc monster: monsterDescs) {
    cout<<monster.name<< endl;
    cout<<monster.description<<endl;
    cout<<monster.symbol<<endl;
    for(string color : monster.colors) {
      cout<<color<<" ";
    }
    cout<<endl;
    printDice(monster.speed);
    for(string attr: monster.abilities) {
      cout<<attr;
    }
    cout<<endl;
    printDice(monster.hp);
    printDice(monster.ad);
    cout<<monster.rarity<<endl;
    cout<<endl;
  }

  vector<objectDesc> objectDescs = parseObjectDescFile(settings.objectDescLoc);

  cout<<endl<<endl<<"/**"<<endl<<" * Object descriptions" <<endl<<" */"<<endl<<endl;
  for(objectDesc obj : objectDescs) {
    cout<<obj.name<<endl;
    cout<<obj.description<<endl;
    cout<<obj.type<<endl;
    for(string color: obj.colors) {
      cout<<color;
    }
    cout<<endl;
    printDice(obj.hitBonus);
    printDice(obj.damBonus);
    printDice(obj.dodgeBonus);
    printDice(obj.defenseBonus);
    printDice(obj.weight);
    printDice(obj.speedBuff);
    printDice(obj.specialAttr);
    printDice(obj.value);
    cout<<((obj.isArtifact) ? "TRUE" : "FALSE") <<endl;
    cout<<obj.rarity<<endl<<endl;
  }
  /*
  scrStartup();

  runGame(dun);
  */
  if(settings.save) {
    dun.save(settings.loadSaveLoc);
  }
}


/**
 * @brief Main game procedure
 * 
 * @param dun 
 */
void runGame(Dungeon& dun) {
  priority_queue<Character, vector<Character*>, Compare> turnQueue;
  bool gameOver = false;
  Character curr;
  int id = 1;
  int resultId;
  PC* pc = new PC(id++, dun.getEmptySpace(), 10, &dun, 1000/10);
  
  dun.setPC(pc);
  turnQueue.push(pc);

  for(int i = 0; i < settings.nummon; i++) {
    int speed = rand() % 15 + 5;
    turnQueue.push(new NPC(id++, dun.getEmptySpace(), speed, &dun, 1000/speed, genCharacterType(), pc));
  }

  while(!gameOver) {
    resultId = turnQueue.top()->takeTurn();
    turnQueue.top()->nextEventTime += turnQueue.top()->speed;
    turnQueue.push(turnQueue.top());
    turnQueue.pop();
    usleep(10000);
    if(resultId== pc->id) {
      scrTearDown();
      gameOver = true;
      cout << "YOU LOSE" << endl;
    } else if(resultId > 0) {
      vector<Character*> tempList;
      while(turnQueue.size() > 0) {
        if(turnQueue.top()->id != resultId) {
          tempList.push_back(turnQueue.top());
          turnQueue.pop();
        } else {
          NPC* removing = (NPC*)turnQueue.top();
          turnQueue.pop();
          delete removing;
        }
      }
      for(Character* ptr : tempList) {
        turnQueue.push(ptr);
      }
    } else if (resultId == MOVE_BETWEEN_FLOORS) {
      // clear out turn queue
      while(turnQueue.size() > 0) {
        if(turnQueue.top()->symbol != '@') {
          delete turnQueue.top();
        }
        turnQueue.pop();
      }

      // get a new dungeon
      dun.rebuild();
      pc->coord = dun.getEmptySpace();
      dun.setPC(pc);
      dun.updateSpace(pc->coord, pc);
      turnQueue.push(pc);

      for(int i = 0; i < settings.nummon; i++) {
        int speed = rand() % 15 + 5;
        turnQueue.push(new NPC(id++, dun.getEmptySpace(), speed, &dun, 1000/speed + pc->nextEventTime, genCharacterType(), pc));
      }
    }
    if(turnQueue.size() == 1) {
      gameOver = true;
      scrTearDown();
      cout << "YOU WIN" << endl;
    }
  }
}

/**
 * @brief Generate a type for a monster in a random fashion
 * 
 * @return char 
 */
char genCharacterType() {
  char type = 0;
  if(rand() % 2 == 0) {
    type |= INTELLIGENCE_BIT;
  }
  if(rand() % 2 == 0) {
    type |= TELEPATHY_BIT;
  }
  if(rand() % 2 == 0) {
    type |= TUNNELING_BIT;
  }
  if(rand() % 2 == 0) {
    type |= ERRATIC_BIT;
  }
  return type;
}

/**
 * @brief Set the settings global based upon the cmd line args
 * 
 * @param argc 
 * @param argv 
 */
void setSettings(int argc, char* argv[]) {
  string home(getenv("HOME"));
  string loadLocSubPath("/.rlg327/dungeon");
  settings.nummon = 0;
  settings.monsterDescLoc = home;
  settings.monsterDescLoc += "/.rlg327/monster_desc.txt";
  settings.objectDescLoc = home;
  settings.objectDescLoc += "/.rlg327/object_desc.txt";
  for(int i = 0; i < argc; i++) {
    if(strcmp("--s", argv[i]) == 0 && i != argc -1) {
      cout << argv[i+1] << endl;
      settings.seed = atoi(argv[i+1]);
    }
    if(strcmp("--save", argv[i]) == 0) {
      settings.save = true;
      settings.loadSaveLoc = home + loadLocSubPath;
    }
    if(strcmp("--load", argv[i]) == 0) {
      settings.load = true;
      settings.loadSaveLoc = home + loadLocSubPath;
    }
    if(strcmp("--nummon", argv[i]) == 0 && i != argc - 1) {
      settings.nummon = atoi(argv[i+1]);
    }
  }

  cout<<"save = "<<settings.save<<"\tload = "<<settings.load<<endl;

  if(settings.seed == 0) {
    settings.seed = time(NULL);
    srand(settings.seed);
  } else {
    srand(settings.seed); 
  }

  if(settings.nummon == 0) {
    settings.nummon = rand() % MAX_RAND_MONST_COUNT + 1;
  }
}

/**
 * @brief set up the dungeon that will be used
 * 
 * @return Dungeon 
 */
Dungeon dungeonInit() {
  Dungeon dun;
  if(settings.load) {
    dun = Dungeon(settings.loadSaveLoc);
  } else {
    dun = Dungeon();
  }
  return dun;
}
