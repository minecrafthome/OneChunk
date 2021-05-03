#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Pos.h"
#include <string>
#include <climits>
class BoundingBox {
public:
    Position start;
    Position end;

    static BoundingBox getNewBoundingBox() {
        return getBoundingBox(INT_MAX, INT_MAX, INT_MAX, INT_MIN, INT_MIN, INT_MIN);
    }
    
    static BoundingBox getBoundingBox(int minx, int miny, int minz, int maxx, int maxy, int maxz) {
        BoundingBox box;
        box.start.x = minx;
        box.start.y = miny;
        box.start.z = minz;
        box.end.x = maxx;
        box.end.y = maxy;
        box.end.z = maxz;
        return box;
    }
    
    std::string getStr() {
        return std::string("(") + std::to_string(start.x) + std::string(",") + std::to_string(start.y) + std::string(",") + std::to_string(start.z) + std::string(")(") + std::to_string(end.x) + std::string(",") + std::to_string(end.y) + std::string(",") + std::to_string(end.z) + std::string(")");
    }

    bool intersectsWith(BoundingBox* box)
    {
        return this->end.x >= box->start.x && this->start.x <= box->end.x && this->end.z >= box->start.z && this->start.z <= box->end.z && this->end.y >= box->start.y && this->start.y <= box->end.y;
    }

    void setStart(int x, int y, int z) {
        this->start.x = x;
        this->start.y = y;
        this->start.z = z;
    }
    
    void setEnd(int x, int y, int z) {
        this->end.x = x;
        this->end.y = y;
        this->end.z = z;
    }
    
    void setAll(int x1, int y1, int z1, int x2, int y2, int z2) {
        setStart(x1, y1, z1);
        setEnd(x2, y2, z2);
    }
    
    int getYSize()
    {
           return this->end.y - this->start.y + 1;
    }
    
    void expandTo(BoundingBox box)
    {
        this->start.x = std::min(this->start.x, box.start.x);
        this->start.y = std::min(this->start.y, box.start.y);
        this->start.z = std::min(this->start.z, box.start.z);
        this->end.x = std::max(this->end.x, box.end.x);
        this->end.y = std::max(this->end.y, box.end.y);
        this->end.z = std::max(this->end.z, box.end.z);
    }
    
    static BoundingBox getComponentToAddBoundingBox(int p_78889_0_, int p_78889_1_, int p_78889_2_, int p_78889_3_, int p_78889_4_, int p_78889_5_, int p_78889_6_, int p_78889_7_, int p_78889_8_, int p_78889_9_)
    {
        BoundingBox ret;
        ret.setAll(-133769, -2, -1, 1, 2, 3);
        switch (p_78889_9_)
        {
            case 0:
                ret.setAll(p_78889_0_ + p_78889_3_, p_78889_1_ + p_78889_4_, p_78889_2_ + p_78889_5_, p_78889_0_ + p_78889_6_ - 1 + p_78889_3_, p_78889_1_ + p_78889_7_ - 1 + p_78889_4_, p_78889_2_ + p_78889_8_ - 1 + p_78889_5_);
                return ret;

            case 1:
                ret.setAll(p_78889_0_ - p_78889_8_ + 1 + p_78889_5_, p_78889_1_ + p_78889_4_, p_78889_2_ + p_78889_3_, p_78889_0_ + p_78889_5_, p_78889_1_ + p_78889_7_ - 1 + p_78889_4_, p_78889_2_ + p_78889_6_ - 1 + p_78889_3_);
                return ret;

            case 2:
                ret.setAll(p_78889_0_ + p_78889_3_, p_78889_1_ + p_78889_4_, p_78889_2_ - p_78889_8_ + 1 + p_78889_5_, p_78889_0_ + p_78889_6_ - 1 + p_78889_3_, p_78889_1_ + p_78889_7_ - 1 + p_78889_4_, p_78889_2_ + p_78889_5_);
                return ret;

            case 3:
                ret.setAll(p_78889_0_ + p_78889_5_, p_78889_1_ + p_78889_4_, p_78889_2_ + p_78889_3_, p_78889_0_ + p_78889_8_ - 1 + p_78889_5_, p_78889_1_ + p_78889_7_ - 1 + p_78889_4_, p_78889_2_ + p_78889_6_ - 1 + p_78889_3_);
                return ret;

            default:
                return ret;
        }
    }
};
#endif
