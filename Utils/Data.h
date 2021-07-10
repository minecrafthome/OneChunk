#ifndef DATA_H
#define DATA_H

#include <vector>
#include <iostream>

#include "BoundingBox.h"
#include "Random.h"

#define PIECE_START 0

#define STAIRS2_PIECE           0
#define CROSSING_PIECE          1
#define LEFTTURN_PIECE          2
#define STRAIGHT_PIECE          3
#define STAIRSSTRAIGHT_PIECE    4
#define STAIRS_PIECE            5
#define CORRIDOR_PIECE          6
#define PRISON_PIECE            7
#define RIGHTTURN_PIECE         8
#define ROOMCROSSING_PIECE      9
#define CHESTCORRIDOR_PIECE    10
#define LIBRARY_PIECE          11
#define PORTALROOM_PIECE       12

class PieceInfo {
public:
    BoundingBox box;
    int coordBaseMode;
    int componentType;
    int BFSlayer;
    int pieceID;
    bool field1;
    bool field2;
    bool field3;
    bool field4;
};

class PieceWeight {
public:
    int instancesLimit;
    int instancesSpawned;
    int componentType;
    int pieceWeight;
    bool ignore = false;
    
    void setData(int componentType, int pieceWeight, int instancesLimit) {
        this->componentType = componentType;
        this->pieceWeight = pieceWeight;
        this->instancesLimit = instancesLimit;
        this->instancesSpawned = 0;
        this->ignore = false;
    }
};

class Data {
public:
    int64_t seed;
    int StartChunkX;
    int StartChunkZ;
    JavaRnd* rng;
    
    PieceInfo pieces[1000];
    std::vector<PieceInfo> pieces_pending;
    PieceWeight weights[11];
    
    bool portalFound = false;
    int totalWeight = 145;
    int pieceCnt = 0;
    int pendingCnt = 0;
    int priorityComponentType = 0;
    int toIgnore = -1;
    int toProcess = 0;
    
    int availableWeights = 11;
    int portalCoordBaseMode = 0;
    BoundingBox* portalBox = NULL;
    
    Data() {
        rng = new JavaRnd(0);
        pieces_pending.reserve(1000);
    }
    
    void initWeights() {
        weights[ 0].setData( 3, 40, 0); //straight
        weights[ 1].setData( 7,  5, 5); //prison
        weights[ 2].setData( 2, 20, 0); //left_turn
        weights[ 3].setData( 8, 20, 0); //right_turn
        weights[ 4].setData( 9, 10, 6); //room_crossing
        weights[ 5].setData( 4,  5, 5); //stairs_straight
        weights[ 6].setData( 5,  5, 5); //stairs
        weights[ 7].setData( 1,  5, 4); //crossing
        weights[ 8].setData(10,  5, 4); //chest_corridor
        weights[ 9].setData(11, 10, 2); //library
        weights[10].setData(12, 20, 1); //portalroom
        availableWeights = 11;
    }
    
    void addPiece(int componentType, int coordBaseMode, int BFSlayer, BoundingBox box) {
        if(this->portalFound) return;
        
        setPiece(pieceCnt++, componentType, coordBaseMode, BFSlayer, box);
    }
    
    void addPiecePending(int componentType, int coordBaseMode, int BFSlayer, BoundingBox box, bool field1 = false, bool field2 = false, bool field3 = false, bool field4 = false) {
        if(this->portalFound) return;
        
        PieceInfo pieceInfo;
        
        pieceInfo.componentType = componentType;
        pieceInfo.coordBaseMode = coordBaseMode;
        pieceInfo.BFSlayer = BFSlayer;
        pieceInfo.box = box;
        
        pieceInfo.field1 = field1;
        pieceInfo.field2 = field2;
        pieceInfo.field3 = field3;
        pieceInfo.field4 = field4;
        
        pieces_pending.push_back(pieceInfo);
    }
    
    void setPiece(int pieceID, int componentType, int coordBaseMode, int BFSlayer, BoundingBox box) {
        pieces[pieceID].componentType = componentType;
        pieces[pieceID].coordBaseMode = coordBaseMode;
        pieces[pieceID].BFSlayer = BFSlayer;
        pieces[pieceID].box = box;
    }
    
    void addBox(BoundingBox box) {
        pieces[pieceCnt++].box = box;
    }
    
    void reset() {
        initWeights();
        totalWeight = 145;
        pieceCnt = 0;
        toIgnore = -1;
        priorityComponentType = 0;
        pendingCnt = 0;
        pieces_pending.clear();
        portalFound = false;
    }
};

#endif