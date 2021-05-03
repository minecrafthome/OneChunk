#ifndef ROOMCROSSING_H
#define ROOMCROSSING_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class RoomCrossing : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 11, 7, 11, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->addPiece(ROOMCROSSING_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            data->rng->nextInt(5);
            
            data->addPiecePending(ROOMCROSSING_PIECE, coordBaseMode, BFSlayer, box);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 4, 1);
        StrongholdPiece::getNextComponentX(data, thisPiece, 1, 4);
        StrongholdPiece::getNextComponentZ(data, thisPiece, 1, 4);
    }
};
#endif