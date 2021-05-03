 #include "StrongholdPiece.h"
 #include "Stronghold.h"
 #include <iostream>
 
 BoundingBox StrongholdPiece::findIntersecting(Data* data, BoundingBox box) {
     BoundingBox ret;
     ret.start.x = -133769;
     
     //std::cout << "Searching for box " << box.getStr() << std::endl;
     for(int i = 0; i < data->pieceCnt; i++) {
         //std::cout << data->pieces[i].box.getStr() << std::endl;
         if(data->pieces[i].box.intersectsWith(&box)) return data->pieces[i].box;
     }
     return ret;
 }

 bool StrongholdPiece::getNextComponentNormal(Data* data, PieceInfo thisPiece, int offset1, int offset2)
 {
     //std::cout << "called from " << thisPiece.componentType << std::endl;
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
    //std::cout << "componentx coordBaseMode: " << thisPiece.coordBaseMode << std::endl;
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
    //std::cout << "componentz coordBaseMode: " << thisPiece.coordBaseMode << std::endl;
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
    //return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->start.x + offset2, this->box->start.y + offset1, this->box->end.z + 1, 0, this.getComponentType());

    default:
        return NULL;
    }
}

/*StrongholdPiece* StrongholdPiece::getNextComponentZ(std::vector<StrongholdPiece *>* pieces, std::vector<StrongholdPiece*>* pieces_pending, JavaRnd* rng, int offset1, int offset2, int BFSlayer, int* strongComponentType, int* totalWeight, std::vector<PieceWeight*>* piecesWeight, PieceWeight** strongholdPieceWeight)
{
  //std::cout << "componentz coordBaseMode: " << this->coordBaseMode << std::endl;
  switch (this->coordBaseMode)
  {
    case 0:
      return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->end.x + 1, this->box->start.y + offset1, this->box->start.z + offset2, 3, this->componentType, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);

    case 1:
      return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->start.x + offset2, this->box->start.y + offset1, this->box->end.z + 1, 0, this->componentType, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);

    case 2:
      return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->end.x + 1, this->box->start.y + offset1, this->box->start.z + offset2, 3, this->componentType, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);

    case 3:
      return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->start.x + offset2, this->box->start.y + offset1, this->box->end.z + 1, 0, this->componentType, this->BFSlayer, strongComponentType, totalWeight, piecesWeight, strongholdPieceWeight);
    //return Stronghold::getNextValidComponent(pieces, pieces_pending, rng, this->box->start.x + offset2, this->box->start.y + offset1, this->box->end.z + 1, 0, this.getComponentType());

    default:
      return NULL;
  }
}*/