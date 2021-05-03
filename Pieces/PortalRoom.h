#ifndef PORTALROOM_H
#define PORTALROOM_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class PortalRoom : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        //removed3
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 11, 8, 16, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            //removed1
            box.start.y = -64;
        }
        else {
            //std::cout << "Found it! boom. Box: " << box.getStr() << std::endl;
            
            
            data->addPiece(PORTALROOM_PIECE, coordBaseMode, BFSlayer, box);
            
            data->portalFound = true;
            //data->addPiecePending(PORTALROOM_PIECE, coordBaseMode, BFSlayer, box);

            /*int x = (data->StartChunkX << 4) + 2;
            int z = (data->StartChunkZ << 4) + 2;
            
            box.setAll(x, 64, z, x + 4, 74, z + 4);
            
            data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);*/
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
    }
};
#endif