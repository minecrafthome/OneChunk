#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Straight : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        //removed3
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, 7, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            //removed1
            box.start.y = -64;
        }
        else {
            //removed2
            
            data->addPiece(STRAIGHT_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5); //getRandomDoor
            bool field1 = data->rng->nextInt(2) == 0;
            bool field2 = data->rng->nextInt(2) == 0;
            
            data->addPiecePending(STRAIGHT_PIECE, coordBaseMode, BFSlayer, box, field1, field2);

            /*int x = (data->StartChunkX << 4) + 2;
            int z = (data->StartChunkZ << 4) + 2;
            
            box.setAll(x, 64, z, x + 4, 74, z + 4);
            
            data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);*/
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 1, 1);
        
        if(thisPiece.field1) StrongholdPiece::getNextComponentX(data, thisPiece, 1, 2);
        if(thisPiece.field2) StrongholdPiece::getNextComponentZ(data, thisPiece, 1, 2);
    }
};
#endif