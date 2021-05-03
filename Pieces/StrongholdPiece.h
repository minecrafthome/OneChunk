#ifndef STRONGHOLD_PIECE_H
#define STRONGHOLD_PIECE_H

#include "../Utils/Random.h"
#include "../Utils/BoundingBox.h"
#include "../Utils/Data.h"
#include <vector>

class StrongholdPiece {
public:
    static bool getNextComponentNormal(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static bool getNextComponentZ(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static bool getNextComponentX(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static BoundingBox findIntersecting(Data* data, BoundingBox box);
};
#endif