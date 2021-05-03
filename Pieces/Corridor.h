#ifndef CORRIDOR_H
#define CORRIDOR_H

#include "../Utils/Data.h"
#include "StrongholdPiece.h"
#include <iostream>

class Corridor : public StrongholdPiece {
public:
    static BoundingBox GeneratePiece(Data* data, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
        //removed3
        BoundingBox var7 = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, 4, coordBaseMode);
        
        BoundingBox intersection = StrongholdPiece::findIntersecting(data, var7);
        if(intersection.start.x == -133769) {
            var7.start.y = -64;
        }
        else {
            if (intersection.start.y == var7.start.y)
            {
                for (int var9 = 3; var9 >= 1; --var9)
                {
                    var7 = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, var9 - 1, coordBaseMode);

                    if (!intersection.intersectsWith(&var7))
                    {
                        var7 = BoundingBox::getComponentToAddBoundingBox(x1, y1, z1, -1, -1, 0, 5, 5, var9, coordBaseMode);
                        //removed2
            
                        data->addPiece(CORRIDOR_PIECE, coordBaseMode, BFSlayer, var7);
                        data->addPiecePending(CORRIDOR_PIECE, coordBaseMode, BFSlayer, var7);
                        
                        return var7;
                    }
                }
            }

            var7.start.y = -64;
        }
        return var7;
    }
};
#endif