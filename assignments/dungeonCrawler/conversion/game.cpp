#ifndef GAME_H
  #define GAME_H
  #include "game.h"
#endif

Settings settings;

using namespace std;

int main(int argc, char* argv[]) {
  setSettings(argc, argv);
  cout<<"Using Seed: "<< settings.seed <<endl;
  // get the basic dungeon
  Dungeon dun = dungeonInit();

  dun.draw();

  if(settings.save) {
    dun.save(settings.loadSaveLoc);
  }
}

void setSettings(int argc, char* argv[]) {
  string home(getenv("HOME"));
  string loadLocSubPath("/.rlg327/dungeon");
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
  }

  cout<<"save = "<<settings.save<<"\tload = "<<settings.load<<endl;

  if(settings.seed == 0) {
    settings.seed = time(NULL);
    srand(settings.seed);
  } else {
    srand(settings.seed); 
  }
}

Dungeon dungeonInit() {
  Dungeon dun;
  if(settings.load) {
    dun = Dungeon(settings.loadSaveLoc);
  } else {
    dun = Dungeon();
  }
  return dun;
}
