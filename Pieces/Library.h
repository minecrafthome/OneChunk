#ifndef LIBRARY_H
#define LIBRARY_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Library : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 14, 11, 15, coordBaseMode);
        if(box.start.x == -133769 || !canStrongholdGoDeeper(box) || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 14, 6, 15, coordBaseMode);
            
            if(box.start.x == -133769 || !canStrongholdGoDeeper(box) || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
                box.start.y = -64;
            }
        }
        
        if(box.start.y != -64) {
            data->addPiece(LIBRARY_PIECE, coordBaseMode, BFSlayer, box);
            
            data->rng->nextInt(5);
            
            data->addPiecePending(LIBRARY_PIECE, coordBaseMode, BFSlayer, box);
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
    }
};
#endif