#ifndef RIGHTTURN_H
#define RIGHTTURN_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class RightTurn : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        //removed3
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, 5, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            //removed1
            box.start.y = -64;
        }
        else {
            //removed2
            
            data->addPiece(RIGHTTURN_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5); //getRandomDoor
            
            data->addPiecePending(RIGHTTURN_PIECE, coordBaseMode, BFSlayer, box);

            /*int x = (data->StartChunkX << 4) + 2;
            int z = (data->StartChunkZ << 4) + 2;
            
            box.setAll(x, 64, z, x + 4, 74, z + 4);
            
            data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);*/
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
        //std::cout << " MY NAME JEFF2 - " << thisPiece.coordBaseMode << std::endl;

        if (thisPiece.coordBaseMode != 2 && thisPiece.coordBaseMode != 3)
        {
            StrongholdPiece::getNextComponentX(data, thisPiece, 1, 1);
            //this->getNextComponentX(pieces, pieces_pending, rng, 1, 1, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);
        }
        else
        {
            StrongholdPiece::getNextComponentZ(data, thisPiece, 1, 1);
          //std::cout << "here." << std::endl;
            //this->getNextComponentZ(pieces, pieces_pending, rng, 1, 1, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);
        }
    }
};
#endif