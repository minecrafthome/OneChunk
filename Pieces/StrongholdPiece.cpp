 #include "StrongholdPiece.h"
 #include "Stronghold.h"
 #include <iostream>
 
 BoundingBox StrongholdPiece::findIntersecting(Data* data, BoundingBox box) {
     BoundingBox ret;
     ret.start.x = -133769;
     
     for(int i = 0; i < data->pieceCnt; i++) {
         if(data->pieces[i].box.intersectsWith(&box)) return data->pieces[i].box;
     }
     return ret;
 }

 bool StrongholdPiece::getNextComponentNormal(Data* data, PieceInfo thisPiece, int offset1, int offset2)
 {
    switch (thisPiece.coordBaseMode)
    {
        case 0:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset1, thisPiece.box.start.y + offset2, thisPiece.box.end.z + 1, thisPiece.coordBaseMode, thisPiece.componentType, thisPiece.BFSlayer);

        case 1:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x - 1, thisPiece.box.start.y + offset2, thisPiece.box.start.z + offset1, thisPiece.coordBaseMode, thisPiece.componentType, thisPiece.BFSlayer);

        case 2:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset1, thisPiece.box.start.y + offset2, thisPiece.box.start.z - 1, thisPiece.coordBaseMode, thisPiece.componentType, thisPiece.BFSlayer);

        case 3:
            return Stronghold::getNextValidComponent(data, thisPiece.box.end.x + 1, thisPiece.box.start.y + offset2, thisPiece.box.start.z + offset1, thisPiece.coordBaseMode, thisPiece.componentType, thisPiece.BFSlayer);

        default:
            return false;
    }
}

bool StrongholdPiece::getNextComponentX(Data* data, PieceInfo thisPiece, int offset1, int offset2)
 {
    switch (thisPiece.coordBaseMode)
    {
        case 0:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x - 1, thisPiece.box.start.y + offset1, thisPiece.box.start.z + offset2, 1, thisPiece.componentType, thisPiece.BFSlayer);

        case 1:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset2, thisPiece.box.start.y + offset1, thisPiece.box.start.z - 1, 2, thisPiece.componentType, thisPiece.BFSlayer);

        case 2:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x - 1, thisPiece.box.start.y + offset1, thisPiece.box.start.z + offset2, 1, thisPiece.componentType, thisPiece.BFSlayer);

        case 3:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset2, thisPiece.box.start.y + offset1, thisPiece.box.start.z - 1, 2, thisPiece.componentType, thisPiece.BFSlayer);
    
        default:
            return false;
    }
}

bool StrongholdPiece::getNextComponentZ(Data* data, PieceInfo thisPiece, int offset1, int offset2)
 {
    switch (thisPiece.coordBaseMode)
    {
        case 0:
            return Stronghold::getNextValidComponent(data, thisPiece.box.end.x + 1, thisPiece.box.start.y + offset1, thisPiece.box.start.z + offset2, 3, thisPiece.componentType, thisPiece.BFSlayer);

        case 1:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset2, thisPiece.box.start.y + offset1, thisPiece.box.end.z + 1, 0, thisPiece.componentType, thisPiece.BFSlayer);

        case 2:
            return Stronghold::getNextValidComponent(data, thisPiece.box.end.x + 1, thisPiece.box.start.y + offset1, thisPiece.box.start.z + offset2, 3, thisPiece.componentType, thisPiece.BFSlayer);

        case 3:
            return Stronghold::getNextValidComponent(data, thisPiece.box.start.x + offset2, thisPiece.box.start.y + offset1, thisPiece.box.end.z + 1, 0, thisPiece.componentType, thisPiece.BFSlayer);

        default:
            return NULL;
    }
}