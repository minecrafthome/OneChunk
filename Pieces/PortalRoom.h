#ifndef PORTALROOM_H
#define PORTALROOM_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class PortalRoom : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        BoundingBox box = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -4, -1, 0, 11, 8, 16, coordBaseMode);
        if(box.start.x == -133769 || StrongholdPiece::findIntersecting(data, box).start.x != -133769) {
            box.start.y = -64;
        }
        else {
            data->addPiece(PORTALROOM_PIECE, coordBaseMode, BFSlayer, box);
            data->portalFound = true;
        }
        return box;
    }
    
    static void BuildComponent(Data* data, PieceInfo thisPiece) {
    }
};
#endif