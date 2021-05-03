#ifndef PRISON_H
#define PRISON_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Prison : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 9, 5, 11, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->addPiece(PRISON_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            
            data->addPiecePending(PRISON_PIECE, coordBaseMode, BFSlayer, box);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 1, 1);
    }
};
#endif