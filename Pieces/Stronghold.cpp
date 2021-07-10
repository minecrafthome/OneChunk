#include "Stronghold.h"
#include "Crossing.h"
#include "LeftTurn.h"
#include "RightTurn.h"
#include "Straight.h"
#include "RoomCrossing.h"
#include "StairsStraight.h"
#include "Stairs.h"
#include "Prison.h"
#include "Corridor.h"
#include "Library.h"
#include "PortalRoom.h"
#include "ChestCorridor.h"

#include <iostream>

bool getStrongholdComponentFromWeightedPiece(Data* data, int componentType, int x1, int y1, int z1, int coordBaseMode, int BFSlayer) {
    BoundingBox box;
    box.start.y = -64;
    
    if(componentType == CROSSING_PIECE) box = Crossing::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == LEFTTURN_PIECE) box = LeftTurn::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == STRAIGHT_PIECE) box = Straight::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == STAIRSSTRAIGHT_PIECE) box = StairsStraight::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == STAIRS_PIECE) box = Stairs::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == PRISON_PIECE) box = Prison::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == PORTALROOM_PIECE) box = PortalRoom::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == RIGHTTURN_PIECE) box = LeftTurn::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == CHESTCORRIDOR_PIECE) box = ChestCorridor::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == ROOMCROSSING_PIECE) box = RoomCrossing::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    else if(componentType == LIBRARY_PIECE) box = Library::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    
    if(box.start.y == -64) return false;
    
    return true;
}

bool getNextComponent(Data* data, int x1, int y1, int z1, int coordBaseMode, int componentType, int BFSlayer)
{
    if(data->availableWeights == 0)
        return false;
    
    if (data->priorityComponentType != 0)
    {
        bool var8 = getStrongholdComponentFromWeightedPiece(data, data->priorityComponentType, x1, y1, z1, coordBaseMode, BFSlayer);
        if (var8)
            return true;
    }
    
    int var13 = 0;

    bool prox = false;
    while (var13 < 5)
    {
        ++var13;
        int var9 = data->rng->nextInt(data->totalWeight);
        
        for(int i = 0; i < 11; i++) {
            if(data->weights[i].ignore) continue;
            
            var9 -= data->weights[i].pieceWeight;
            
            if(var9 < 0) {
                bool canSpawnPiece = data->weights[i].instancesLimit == 0 || data->weights[i].instancesSpawned < data->weights[i].instancesLimit;
                if(data->weights[i].componentType == LIBRARY_PIECE && BFSlayer <= 4)
                    canSpawnPiece = false;
                
                else if(data->weights[i].componentType == PORTALROOM_PIECE && BFSlayer <= 5)
                    canSpawnPiece = false;
                
                if(!canSpawnPiece || i == data->toIgnore) {
                    break;
                }
                
                bool spawned = getStrongholdComponentFromWeightedPiece(data, data->weights[i].componentType, x1, y1, z1, coordBaseMode, BFSlayer);
                
                if(spawned) {
                    data->weights[i].instancesSpawned += 1;
                    
                    bool canSpawnPiece = data->weights[i].instancesLimit == 0 || data->weights[i].instancesSpawned < data->weights[i].instancesLimit;
                    if(data->weights[i].componentType == LIBRARY_PIECE && BFSlayer <= 4)
                        canSpawnPiece = false;
                    else if(data->weights[i].componentType == PORTALROOM_PIECE && BFSlayer <= 5)
                        canSpawnPiece = false;

                    if(!canSpawnPiece) {
                        data->totalWeight -= data->weights[i].pieceWeight;
                        data->weights[i].ignore = true;
                        data->availableWeights--;
                    }
                
                    data->toIgnore = i;
                    return true;
                }
            }
        }
    }
    
    BoundingBox box = Corridor::GeneratePiece(data, x1, y1, z1, coordBaseMode, BFSlayer);
    if(box.start.y != -64)
        return true;
    
    return false;
}

bool Stronghold::getNextValidComponent(Data* data, int x1, int y1, int z1, int coordBaseMode, int componentType, int BFSlayer) {    
    if (BFSlayer > 50 || data->portalFound)
    {
        return false;
    }

    BoundingBox startPieceBox = data->pieces[0].box;
    if (std::abs(x1 - startPieceBox.start.x) <= 112 && std::abs(z1 - startPieceBox.start.z) <= 112)
    {
        getNextComponent(data, x1, y1, z1, coordBaseMode, componentType, BFSlayer + 1);
        return true;
    }
    return false;
}