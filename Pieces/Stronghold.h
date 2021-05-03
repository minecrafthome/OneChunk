#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include "../Utils/Data.h"

class Stronghold {
public:
    static bool getNextValidComponent(Data* data, int x1, int y1, int z1, int coordBaseMode, int componentType, int BFSlayer);
};
#endif