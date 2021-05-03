#ifndef STAIRS2_H
#define STAIRS2_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"

class Stairs2 : public StrongholdPiece {
public:
    static void GeneratePiece(Data* data) {
        BoundingBox box;
        int x = (data->StartChunkX << 4) + 2;
        int z = (data->StartChunkZ << 4) + 2;
        
        box.setAll(x, 64, z, x + 4, 74, z + 4);
        
        data->addPiece(STAIRS2_PIECE, data->rng->nextInt(4), 0, box);
    }
    
    static void BuildComponent(Data* data) {
        StrongholdPiece::getNextComponentNormal(data, data->pieces[0], 1, 1);
    }
};
#endif