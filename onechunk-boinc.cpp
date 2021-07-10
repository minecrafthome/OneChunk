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
#include <cstdio>
#include <unordered_map>

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
FILE *binary;
int outCount = 0;
time_t start;
int64_t total;
time_t elapsed_chkpoint = 0;
std::unordered_map<int, std::tuple<int, int>> goodStarts;

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

    if(!threadData->portalFound) {
        BoundingBox structureBox = BoundingBox::getNewBoundingBox();
        for(int i = 0; i < threadData->pieceCnt; i++)
            structureBox.expandTo(threadData->pieces[i].box);

        int ySize = structureBox.getYSize() + 1;
        if(ySize < 53)
            threadData->rng->nextInt(53 - ySize);

        generateAllPieces(threadData, seed, startChunkX, startChunkZ);
    }
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

        if(x < desiredX - 7 || x > desiredX + 7 || z < desiredZ - 7 || z > desiredZ + 7) {
            angle += 2 * PI / 3.0;
            continue;
        }
        
        std::unordered_map<int, std::tuple<int, int>>::iterator it;
        it = goodStarts.find(x * 1000 + z);
        
        if(it != goodStarts.end()) {
            fprintf(fp, "%lld %d %d\n", copy, std::get<0>(it->second), std::get<1>(it->second));
            fflush(fp);
            outCount++;
        }
        return;
    }
}

void generatePosCache(int64_t binarySeed, int16_t binaryX, int16_t binaryZ, Data* data) {
    int pos1X, pos1Z, pos2X, pos2Z;
    for(int offsetX = -7; offsetX <= 7; offsetX++) {
        for(int offsetZ = -7; offsetZ <= 7; offsetZ++) {
            data->seed = binarySeed;
            data->StartChunkX = binaryX + offsetX;
            data->StartChunkZ = binaryZ + offsetZ;
            setInitialRng(data);
            
            generateAllPieces(data, data->seed, data->StartChunkX, data->StartChunkZ);

            if(data->portalCoordBaseMode == 0) {
                pos1X = data->portalBox->start.x + 3;
                pos1Z = data->portalBox->start.z + 12;
                pos2X = data->portalBox->start.x + 7;
                pos2Z = data->portalBox->start.z + 8;
            }

            else if(data->portalCoordBaseMode == 1) {
                pos1X = data->portalBox->start.x + 7;
                pos1Z = data->portalBox->start.z + 7;
                pos2X = data->portalBox->start.x + 3;
                pos2Z = data->portalBox->start.z + 3;
            }

            else if(data->portalCoordBaseMode == 2) {
                pos1X = data->portalBox->start.x + 3;
                pos1Z = data->portalBox->start.z + 7;
                pos2X = data->portalBox->start.x + 7;
                pos2Z = data->portalBox->start.z + 3;
            }

            else {
                pos1X = data->portalBox->start.x + 12;
                pos1Z = data->portalBox->start.z + 7;
                pos2X = data->portalBox->start.x + 8;
                pos2Z = data->portalBox->start.z + 3;
            }

            if((pos1X - 8) >> 4 == binaryX && (pos2X - 8) >> 4 == binaryX) {
                if((pos1Z - 8) >> 4 == binaryZ && (pos2Z - 8) >> 4 == binaryZ) {
                    goodStarts.insert(
                        std::pair<int, std::tuple<int,int>>(
                            data->StartChunkX * 1000 + data->StartChunkZ,
                            std::tuple<int,int>((pos2X + pos1X) / 2, (pos2Z + pos1Z) / 2)       
                        )
                    );
                }
            }
        }
    }
}

void doSeed(int64_t seed, int x, int z, LayerStack g, int* cache, Data* threadData, BoundingBox* boxCache) {
    getStrongholdPositions(&g, &seed, 3, threadData, cache, boxCache, x, z);
}

int main(int argc, char **argv) {
    fp = fopen("out.txt", "w+");
    char* filename = "jf_MD5"; //Input seeds and pos go here
    for (int i = 1; i < argc; i += 2) {
        const char *param = argv[i];
        if (strcmp(param, "-f") == 0 || strcmp(param, "--file") == 0) {
            filename = argv[i + 1];
        }
        else {
            fprintf(stderr,"Unknown parameter: %s\n", param);
        }
    }
    initBiomes();

    int64_t checkpointOffset = 0;
    std::vector<std::thread> threads;

    #ifdef BOINC
        BOINC_OPTIONS options;
        boinc_options_defaults(options);
        options.normal_thread_priority = true;
        boinc_init_options(&options);
    #endif

    FILE *checkpoint_data = boinc_fopen("filter9000-checkpoint", "rb");
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

    start = time(NULL);

    int64_t structureSeed;
    int ChunkX;
    int ChunkZ;
	int64_t total = 0;
    int* cache = (int*)malloc(sizeof(int) * 16 * 256 * 256);
    Data* data = new Data();
    BoundingBox* boxCache = (BoundingBox*)malloc(sizeof(BoundingBox));

    LayerStack g;
    setupGenerator(&g, MC_1_7);
    binary = fopen(filename, "rb");
    if(!binary){
        fprintf(stderr, "ERROR: Could not load file %s for input.\n", filename);
    }
    fseek(binary, 0L, SEEK_END);
    total = (int64_t)((double)ftell(binary) / 8.0);
    fseek(binary, 0L, SEEK_SET);
	int16_t binaryX;
    int16_t binaryZ;
    int64_t* binarySeedPtr;
    int64_t binarySeed;
    binarySeedPtr = (int64_t*)malloc(8);
    fseek(binary, checkpointOffset*8, SEEK_SET);
    for(int i = 0+checkpointOffset; i < total; i++){
        time_t elapsed = time(NULL) - start;
        int result = 0;
        result = fread(binarySeedPtr, 8, 1, binary);
        binaryX = (uint8_t)(((*binarySeedPtr) >> 8) & 0x0FF) - 127;
        binaryZ = (uint8_t)(((*binarySeedPtr)) & 0x0FF) - 127; 
        binarySeed = (*binarySeedPtr >> 16) & 0x0FFFFFFFFFFFF;

        generatePosCache(binarySeed, binaryX, binaryZ, data);
        if(goodStarts.size() > 0) {
            for (int64_t upperBits = 0; upperBits < 1L << 16; upperBits++) {
                int64_t worldSeed = (upperBits << 48) | binarySeed;
                applySeed(&g, worldSeed);
                doSeed(worldSeed, binaryX, binaryZ, g, cache, data, boxCache);
            }
        }
        goodStarts.clear();

        if(i % 5 || boinc_time_to_checkpoint()){
            #ifdef BOINC
                boinc_begin_critical_section(); // Boinc should not interrupt this
            #endif
            // Checkpointing section below
            //boinc_delete_file("filter9000-checkpoint"); // Don't touch, same func as normal fdel

            FILE *checkpoint_data = boinc_fopen("filter9000-checkpoint", "wb");
            struct checkpoint_vars data_store;
            data_store.offset = i;
            data_store.elapsed_chkpoint = elapsed_chkpoint + elapsed;
            fwrite(&data_store, sizeof(data_store), 1, checkpoint_data);
            fflush(checkpoint_data);
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
    fflush(fp);
    fclose(fp);
    fclose(binary);
    boinc_delete_file("filter9000-checkpoint.txt");
    #ifdef BOINC
        boinc_end_critical_section();
    #endif
    boinc_finish(0);
}