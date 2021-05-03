#ifndef RIGHTTURN_H
#define RIGHTTURN_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class RightTurn : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, 5, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->addPiece(RIGHTTURN_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            
            data->addPiecePending(RIGHTTURN_PIECE, coordBaseMode, BFSlayer, box);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        if (thisPiece.coordBaseMode != 2 && thisPiece.coordBaseMode != 3)
        {
            StrongholdPiece::getNextComponentX(data, thisPiece, 1, 1);
        }
        else
        {
            StrongholdPiece::getNextComponentZ(data, thisPiece, 1, 1);
        }
    }
};
#endif