#ifndef CROSSING_H
#define CROSSING_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Crossing : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -3, 0, 10, 9, 11, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            //removed1
            box.start.y = -64;
        }
        else {
            //removed2
            
            data->pieces_pending[data->pendingCnt].pieceID = data->pieceCnt;
            data->addPiece(CROSSING_PIECE, coordBaseMode, BFSlayer, box);
            
            //std::cout << "randomDoor: " << data->rng->nextInt(5) << std::endl; //getRandomDoor
            data->rng->nextInt(5);
            
            bool field1 = data->rng->nextBoolean();
            bool field2 = data->rng->nextBoolean();
            bool field3 = data->rng->nextBoolean();
            bool field4 = data->rng->nextInt(3) > 0;
            
            data->addPiecePending(CROSSING_PIECE, coordBaseMode, BFSlayer, box, field1, field2, field3, field4);

            /*int x = (data->StartChunkX << 4) + 2;
            int z = (data->StartChunkZ << 4) + 2;
            
            box.setAll(x, 64, z, x + 4, 74, z + 4);
            
            data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);*/
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        //std::cout << " MY NAME JEFF " << std::endl;
        int var4 = 3;
        int var5 = 5;

        if (thisPiece.coordBaseMode == 1 || thisPiece.coordBaseMode == 2)
        {
          var4 = 8 - var4;
          var5 = 8 - var5;
        }
        StrongholdPiece::getNextComponentNormal(data, thisPiece, 5, 1);
        
        if(thisPiece.field1) {
            //std::cout << "special 3" << std::endl;
            StrongholdPiece::getNextComponentX(data, thisPiece, var4, 1);
        }
        
        if(thisPiece.field2) {
            //std::cout << "special 4" << std::endl;
            StrongholdPiece::getNextComponentX(data, thisPiece, var5, 7);
        }
        
        if(thisPiece.field3) {
            //std::cout << "special 5" << std::endl;
            StrongholdPiece::getNextComponentZ(data, thisPiece, var4, 1);
        }
        
        if(thisPiece.field4) {
            //std::cout << "special 6" << std::endl;
            StrongholdPiece::getNextComponentZ(data, thisPiece, var5, 7);
        }
    }
};
#endif