// OneChunk-CPU 'Filter9000', Minecraft@Home
// Follow workflows/main.yml for compilation
// and do not compile with -O3, only -O2

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <thread>

#include "Utils/Data.h"
#include "Utils/Random.h"

#include "Pieces/Stairs2.h"
#include "Pieces/Straight.h"
#include "Pieces/Crossing.h"
#include "Pieces/LeftTurn.h"
#include "Pieces/RightTurn.h"
#include "Pieces/StairsStraight.h"
#include "Pieces/Stairs.h"
#include "Pieces/Prison.h"
#include "Pieces/ChestCorridor.h"
#include "Pieces/RoomCrossing.h"

#include "cubiomes/layers.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

#include "boinc/boinc_api.h"
#include "boinc/filesys.h"

FILE *fp;
int outCount = 0;
time_t start;
int64_t total;
std::vector<std::string> arr;
time_t elapsed_chkpoint = 0;

struct checkpoint_vars {
    unsigned long long offset;
    time_t elapsed_chkpoint;
};

void setInitialRng(Data* data) {
    int64_t worldSeed = data->seed;
    int chunkX = data->StartChunkX;
    int chunkZ = data->StartChunkZ;

    data->rng->setSeed(worldSeed);

    int64_t var7 = data->rng->nextLong();
    int64_t var9 = data->rng->nextLong();
    int64_t var13 = (int64_t)chunkX * var7;
    int64_t var15 = (int64_t)chunkZ * var9;
    int64_t internalSeed = var13 ^ var15 ^ worldSeed;

    data->rng->setSeed(internalSeed);
    
    data->rng->Next(32);
}

void setFirstPiece(Data* data) {    
    Stairs2::GeneratePiece(data);
    data->priorityComponentType = 1;
    Stairs2::BuildComponent(data);
    data->priorityComponentType = 0;
}

void BuildComponent(Data* data, PieceInfo pieceInfo) {
    int componentType = pieceInfo.componentType;

    if(componentType == CROSSING_PIECE) Crossing::BuildComponent(data, pieceInfo);
    else if(componentType == LEFTTURN_PIECE) LeftTurn::BuildComponent(data, pieceInfo);
    else if(componentType == STRAIGHT_PIECE) Straight::BuildComponent(data, pieceInfo);
    else if(componentType == STAIRSSTRAIGHT_PIECE) StairsStraight::BuildComponent(data, pieceInfo);
    else if(componentType == ROOMCROSSING_PIECE) RoomCrossing::BuildComponent(data, pieceInfo);
    else if(componentType == PRISON_PIECE) Prison::BuildComponent(data, pieceInfo);
    else if(componentType == STAIRS_PIECE) Stairs::BuildComponent(data, pieceInfo);
    else if(componentType == RIGHTTURN_PIECE) LeftTurn::BuildComponent(data, pieceInfo);
    else if(componentType == CHESTCORRIDOR_PIECE) ChestCorridor::BuildComponent(data, pieceInfo);
    else if(componentType == CORRIDOR_PIECE || componentType == LIBRARY_PIECE) {}
    else
        std::cout << "COULD NOT BUILD COMPONENT TYPE " << componentType << std::endl;
}


void generateAllPieces(Data* threadData, int64_t seed, int startChunkX, int startChunkZ) {
    threadData->reset();    
    setFirstPiece(threadData);

    while(threadData->pieces_pending.size() != 0) {
        int randomPieceNumber = threadData->rng->nextInt(threadData->pieces_pending.size());
        PieceInfo pieceChosen = threadData->pieces_pending.at(randomPieceNumber);
        threadData->pieces_pending.erase(threadData->pieces_pending.begin() + randomPieceNumber);

        BuildComponent(threadData, pieceChosen);

        if(threadData->portalFound)
            break;
    }

    
    PieceInfo lastPiece = threadData->pieces[threadData->pieceCnt - 1];
    if(lastPiece.componentType != PORTALROOM_PIECE) {
        BoundingBox structureBox = BoundingBox::getNewBoundingBox();
        for(int i = 0; i < threadData->pieceCnt; i++) {
            structureBox.expandTo(threadData->pieces[i].box);
        }
        
        int ySize = structureBox.getYSize() + 1;
        if(ySize < 53)
            threadData->rng->nextInt(53 - ySize);
        
        generateAllPieces(threadData, seed, startChunkX, startChunkZ);
    }
}

PieceInfo getLastPiece(Data* threadData, int64_t seed, int startChunkX, int startChunkZ) {
    generateAllPieces(threadData, seed, startChunkX, startChunkZ);
    
    return threadData->pieces[threadData->pieceCnt - 1];
}

Position getCenterPos(BoundingBox box) {
    Position ret;
    ret.x = (box.end.x + box.start.x) / 2;
    ret.z = (box.end.z + box.start.z) / 2;
    return ret;
}

void getStrongholdPositions(LayerStack* g, int64_t* worldSeed, int SH, Data* data, int* cache, BoundingBox* boxCache, int desiredX, int desiredZ)
{
    static const char* isStrongholdBiome = getValidStrongholdBiomes();

    int64_t copy = *worldSeed;
    applySeed(g, *worldSeed);

    Layer *l = &g->layers[L_RIVER_MIX_4];

    setSeed(worldSeed);
    long double angle = nextDouble(worldSeed) * PI * 2.0;
    int var6 = 1;

    for (int var7 = 0; var7 < SH; ++var7)
    {
        long double distance = (1.25 * (double)var6 + nextDouble(worldSeed)) * 32.0 * (double)var6;
        int x = (int)round(cos(angle) * distance);
        int z = (int)round(sin(angle) * distance);

        Pos biomePos = findBiomePosition(MC_1_7, l, cache, (x << 4) + 8, (z << 4) + 8, 112, isStrongholdBiome, worldSeed, NULL);

        x = biomePos.x >> 4;
        z = biomePos.z >> 4;

        data->seed = copy;
        data->StartChunkX = x;
        data->StartChunkZ = z;
        setInitialRng(data);
        
        PieceInfo lastPiece = getLastPiece(data, copy, x, z);
        if(lastPiece.componentType == PORTALROOM_PIECE) {
            int pos1X, pos1Z, pos2X, pos2Z;

            if(lastPiece.coordBaseMode == 0) {
                pos1X = lastPiece.box.start.x + 3;
                pos1Z = lastPiece.box.start.z + 12;
                pos2X = lastPiece.box.start.x + 7;
                pos2Z = lastPiece.box.start.z + 8;
            }

            else if(lastPiece.coordBaseMode == 1) {
                pos1X = lastPiece.box.start.x + 7;
                pos1Z = lastPiece.box.start.z + 7;
                pos2X = lastPiece.box.start.x + 3;
                pos2Z = lastPiece.box.start.z + 3;
            }

            else if(lastPiece.coordBaseMode == 2) {
                pos1X = lastPiece.box.start.x + 3;
                pos1Z = lastPiece.box.start.z + 7;
                pos2X = lastPiece.box.start.x + 7;
                pos2Z = lastPiece.box.start.z + 3;
            }

            else if(lastPiece.coordBaseMode == 3) {
                pos1X = lastPiece.box.start.x + 12;
                pos1Z = lastPiece.box.start.z + 7;
                pos2X = lastPiece.box.start.x + 8;
                pos2Z = lastPiece.box.start.z + 3;
            }

            if((pos1X - 8) >> 4 == desiredX && (pos2X - 8) >> 4 == desiredX) {
                if((pos1Z - 8) >> 4 == desiredZ && (pos2Z - 8) >> 4 == desiredZ) {
                    Position center = getCenterPos(lastPiece.box);
                    fprintf(fp, "%lld %d %d\n", copy, center.x, center.z);
                    outCount++;
                }
            }
        }
        angle += 2 * PI / 3.0;
    }
}

void doSeed(int64_t seed, int x, int z, LayerStack g, int* cache, Data* threadData, BoundingBox* boxCache) {
    getStrongholdPositions(&g, &seed, 3, threadData, cache, boxCache, x, z); 
}

int main(int argc, char **argv) {
    fp = fopen("out.txt", "w+");
    char* filename = "ocinput.txt";
    initBiomes();

    int64_t checkpointOffset = 0;
    std::vector<std::thread> threads;

    #ifdef BOINC
        BOINC_OPTIONS options;
        boinc_options_defaults(options);
        options.normal_thread_priority = true;
        boinc_init_options(&options);
    #endif

    FILE *checkpoint_data = boinc_fopen("filter9000-checkpoint.txt", "rb");
    if(!checkpoint_data){
        fprintf(stderr, "No checkpoint to load\n");
    } else {
        #ifdef BOINC
            boinc_begin_critical_section();
        #endif

        struct checkpoint_vars data_store;
        fread(&data_store, sizeof(data_store), 1, checkpoint_data);
        checkpointOffset = data_store.offset;
        elapsed_chkpoint = data_store.elapsed_chkpoint;
        fprintf(stderr, "Checkpoint loaded, task time %d s, seed pos: %llu\n", elapsed_chkpoint, checkpointOffset);
        fclose(checkpoint_data);

        #ifdef BOINC
            boinc_end_critical_section();
        #endif
    }

    #ifdef BOINC
    APP_INIT_DATA aid;
    boinc_get_init_data(aid);
    #endif

    std::string line;
    std::ifstream infile(filename);

    while(std::getline(infile, line)){
        int ChunkX = 0;
        int ChunkZ = 0;
        int64_t structureSeed = 0;
        std::istringstream iss(line);
        if(!(iss >> structureSeed >> ChunkX >> ChunkZ)){break;}
        arr.push_back(line);
    }

    infile.close();

    total = arr.size();
    start = time(NULL);

    int64_t structureSeed;
    int ChunkX;
    int ChunkZ;

    int* cache = (int*)malloc(sizeof(int) * 16 * 256 * 256);
    Data* data = new Data();
    BoundingBox* boxCache = (BoundingBox*)malloc(sizeof(BoundingBox));

    LayerStack g;
    setupGenerator(&g, MC_1_7);

    for(int i = 0+checkpointOffset; i < total; i++){
        time_t elapsed = time(NULL) - start;
        std::string line = arr[i];
        std::istringstream iss(line);
        if(!(iss >> structureSeed >> ChunkX >> ChunkZ)){break;}

        for (int64_t upperBits = 0; upperBits < 1L << 16; upperBits++) {
            int64_t worldSeed = (upperBits << 48) | structureSeed;
            applySeed(&g, worldSeed);
            doSeed(worldSeed, ChunkX, ChunkZ, g, cache, data, boxCache);
        }

        if(i % 5 || boinc_time_to_checkpoint()){
            #ifdef BOINC
                boinc_begin_critical_section(); // Boinc should not interrupt this
            #endif
            // Checkpointing section below
            boinc_delete_file("filter9000-checkpoint.txt"); // Don't touch, same func as normal fdel

            FILE *checkpoint_data = boinc_fopen("filter9000-checkpoint.txt", "wb");
            struct checkpoint_vars data_store;
            data_store.offset = i;
            data_store.elapsed_chkpoint = elapsed_chkpoint + elapsed;
            fwrite(&data_store, sizeof(data_store), 1, checkpoint_data);
            fclose(checkpoint_data);
            #ifdef BOINC
                boinc_end_critical_section();
                boinc_checkpoint_completed(); // Checkpointing completed
            #endif
        }
    }

    #ifdef BOINC
        boinc_begin_critical_section();
    #endif
    
    time_t elapsed = (time(NULL) - start) + elapsed_chkpoint;
    double done = (double) total;
    double speed = (done / (double) elapsed) * 65535;

    fprintf(stderr, "\nSpeed: %.2lf world seeds/s\n", speed );
    fprintf(stderr, "Done.\n");
    fprintf(stderr, "Processed %llu world seeds in %.2lfs seconds.\n", total*65535, (double) elapsed );
    fprintf(stderr, "Have %llu output seeds.\n", outCount );
    fflush(stderr);
    fclose(fp);
    boinc_delete_file("filter9000-checkpoint.txt");
    #ifdef BOINC
        boinc_end_critical_section();
    #endif
    boinc_finish(0);
}
