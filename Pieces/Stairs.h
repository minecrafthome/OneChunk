#ifndef STAIRS_H
#define STAIRS_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Stairs : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -7, 0, 5, 11, 5, coordBaseMode);
        if(box.start.x == -133769 || !canStrongholdGoDeeper(box) || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->addPiece(STAIRS_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            
            data->addPiecePending(STAIRS_PIECE, coordBaseMode, BFSlayer, box);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 1, 1);
    }
};
#endif