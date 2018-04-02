#include "monster.h"

monsterFactory::monsterFactory(Dungeon* dun){
    this->dun = dun;
}

vector<NPC*> monsterFactory::buildMonsters(vector<monsterDesc> descList, int numToBuild, int currentEventTime, PC* pc) {
    int builtCount = 0;
    bool noDescSelected = true;
    monsterDesc desc;
    vector<NPC*> monstList;

    while(builtCount < numToBuild) {
        while(noDescSelected) {
            int index = rand() % descList.size();
            int randInt = rand() % 100;
            if(descList[index].rarity > randInt) {
                noDescSelected = false;
                desc = descList[index];
            }
        }
        NPC* npc = new NPC();
        buildMonster(desc, npc, currentEventTime, pc);
        monstList.push_back(npc);
        noDescSelected = true;
        builtCount++;
    }
    return monstList;
}
void monsterFactory::buildMonster(monsterDesc desc, NPC* monster, int currentEventTime, PC* pc) {
    static int id = 1;
    monster->name = desc.name;
    monster->description = desc.description;
    monster->colors = desc.colors;
    monster->hp = rollDice(desc.hp);
    monster->ad = desc.ad;
    monster->symbol = desc.symbol;
    monster->speed = rollDice(desc.speed);
    monster->nextEventTime = (1000/ monster->speed) + currentEventTime;
    monster->dun = this->dun;
    monster->id = id++;
    monster->lastSeenPCLoc = {0,0};
    monster->coord = dun->getEmptySpace();
    monster->pc = pc;
    monster->type = genType(desc.abilities);
    monster->setTurnLogic();

    dun->updateSpace(monster->coord, monster);
}

char genType(vector<string> abilities) {
    char type = 0b0000;
    for(string ability : abilities) {
        if(ability.compare("SMART") == 0) {
            type |= 0b0001;
        } else if(ability.compare("TELE") == 0) {
            type |= 0b0010;
        } else if(ability.compare("TUNNEL") == 0) {
            type |= 0b0100;
        } else if(ability.compare("ERRATIC") == 0) {
            type |= 0b1000;
        }
    }
    return type;
}