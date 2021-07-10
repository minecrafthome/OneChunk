#ifndef CROSSING_H
#define CROSSING_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Crossing : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -3, 0, 10, 9, 11, coordBaseMode);
        if(box.start.x == -133769 || !canStrongholdGoDeeper(box) || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->pieces_pending[data->pendingCnt].pieceID = data->pieceCnt;
            data->addPiece(CROSSING_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            
            bool field1 = data->rng->nextBoolean();
            bool field2 = data->rng->nextBoolean();
            bool field3 = data->rng->nextBoolean();
            bool field4 = data->rng->nextInt(3) > 0;
            
            data->addPiecePending(CROSSING_PIECE, coordBaseMode, BFSlayer, box, field1, field2, field3, field4);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        int var4 = 3;
        int var5 = 5;

        if (thisPiece.coordBaseMode == 1 || thisPiece.coordBaseMode == 2)
        {
          var4 = 8 - var4;
          var5 = 8 - var5;
        }
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 5, 1);
        
        if(thisPiece.field1) {
            StrongholdPiece::getNextComponentX(data, thisPiece, var4, 1);
        }
        
        if(thisPiece.field2) {
            StrongholdPiece::getNextComponentX(data, thisPiece, var5, 7);
        }
        
        if(thisPiece.field3) {
            StrongholdPiece::getNextComponentZ(data, thisPiece, var4, 1);
        }
        
        if(thisPiece.field4) {
            StrongholdPiece::getNextComponentZ(data, thisPiece, var5, 7);
        }
    }
};
#endif