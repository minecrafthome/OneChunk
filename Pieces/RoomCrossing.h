#ifndef ROOMCROSSING_H
#define ROOMCROSSING_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class RoomCrossing : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        //removed3
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 11, 7, 11, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            //removed1
            box.start.y = -64;
        }
        else {
            //removed2
            
            data->addPiece(ROOMCROSSING_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5); //getRandomDoor
            data->rng->nextInt(5); //room type (?)
            
            data->addPiecePending(ROOMCROSSING_PIECE, coordBaseMode, BFSlayer, box);

            /*int x = (data->StartChunkX << 4) + 2;
            int z = (data->StartChunkZ << 4) + 2;
            
            box.setAll(x, 64, z, x + 4, 74, z + 4);
            
            data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);*/
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