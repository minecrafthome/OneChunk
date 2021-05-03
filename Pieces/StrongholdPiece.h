#ifndef STRONGHOLD_PIECE_H
#define STRONGHOLD_PIECE_H

#include "../Utils/Random.h"
#include "../Utils/BoundingBox.h"
#include "../Utils/Data.h"
#include <vector>
//#include "PieceWeight.h"

class StrongholdPiece {
public:
    static bool getNextComponentNormal(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static bool getNextComponentZ(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static bool getNextComponentX(Data* data, PieceInfo thisPiece, int offset1, int offset2);
    static BoundingBox findIntersecting(Data* data, BoundingBox box);
    //static StrongholdPiece* findIntersecting(std::vector<StrongholdPiece *> pieces, BoundingBox* box);

    /*StrongholdPiece* getNextComponentNormal(std::vector<StrongholdPiece*>* pieces, std::vector<StrongholdPiece*>* pieces_pending, JavaRnd* rng, int offsetX, int offsetY, int BFSlayer, int* strongComponentType, int* totalWeight, std::vector<PieceWeight*>* piecesWeight, PieceWeight** strongholdPieceWeight);

    StrongholdPiece* getNextComponentZ(std::vector<StrongholdPiece *>* pieces, std::vector<StrongholdPiece*>* pieces_pending, JavaRnd* rnd, int offset1, int offset2, int BFSlayer, int* strongComponentType, int* totalWeight, std::vector<PieceWeight*>* piecesWeight, PieceWeight** strongholdPieceWeight);

    StrongholdPiece* getNextComponentX(std::vector<StrongholdPiece *>* pieces, std::vector<StrongholdPiece*>* pieces_pending, JavaRnd* rng, int offset1, int offset2, int BFSlayer, int* strongComponentType, int* totalWeight, std::vector<PieceWeight*>* piecesWeight, PieceWeight** strongholdPieceWeight);

    virtual void buildComponent(std::vector<StrongholdPiece*>* pieces, std::vector<StrongholdPiece*>* pieces_pending, JavaRnd* rng, int* strongComponentType, int* totalWeight, std::vector<PieceWeight*>* piecesWeight, PieceWeight** strongholdPieceWeight) {}*/
};
#endif