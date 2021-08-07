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
#include <unordered_set>
#include <vector>

#include "cubiomes/layers.h"
#include "cubiomes/finders.h"
#include "cubiomes/generator.h"

#include "boinc/boinc_api.h"
#include "boinc/filesys.h"

struct checkpoint_vars {
    unsigned long long seedCount;
    unsigned long long outCount;
    double timeElapsed;
};
checkpoint_vars curr_checkpoint;
int total;

using namespace std;

FILE* fp;
int count;
unordered_set<int32_t> mc1_7_positions;
unordered_set<int32_t> mc1_8_positions;

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void getStrongholdPositions(LayerStack* g, int64_t* worldSeed, int* cache)
{
    static const char* isStrongholdBiome = getValidStrongholdBiomes();

    int64_t copy = *worldSeed;
    Layer *l = &g->layers[L_RIVER_MIX_4];

    setSeed(worldSeed);
    long double angle = nextDouble(worldSeed) * PI * 2.0;
    int var6 = 1;

    for (int var7 = 0; var7 < 3; ++var7)
    {
        long double distance = (1.25 * (double)var6 + nextDouble(worldSeed)) * 32.0 * (double)var6;
        int x = (int)round(cos(angle) * distance);
        int z = (int)round(sin(angle) * distance);

        Pos biomePos = findBiomePosition(MC_1_7, l, cache, (x << 4) + 8, (z << 4) + 8, 112, isStrongholdBiome, worldSeed, NULL);

        x = biomePos.x >> 4;
        z = biomePos.z >> 4;
        
        if(mc1_7_positions.find(x * 1000 + z) != mc1_7_positions.end()) {
            fprintf(fp, "%lld %d %d 1\n", copy, x, z);
            fflush(fp);
            curr_checkpoint.outCount++;
        }
        
        if(mc1_8_positions.find(x * 1000 + z) != mc1_8_positions.end()) {
            fprintf(fp, "%lld %d %d 2\n", copy, x, z);
            fflush(fp);
            curr_checkpoint.outCount++;
        }
        angle += 2 * PI / 3.0;
    }
}

int countFileLines(char* file) {
    int ret = 0;
    std::ifstream infile(file);
    std::string line;
    while (std::getline(infile, line)) {
        ret++;
    }
    return ret;
}

void loadCheckpoint() {
    FILE *checkpoint_data = boinc_fopen("onechunk-checkpoint", "rb");
    if(!checkpoint_data){
        fprintf(stderr, "No checkpoint to load\n");
        curr_checkpoint.seedCount = 0;
        curr_checkpoint.outCount = 0;
        curr_checkpoint.timeElapsed = 0.0;
    }
    else {
        boinc_begin_critical_section();

        fread(&curr_checkpoint, sizeof(curr_checkpoint), 1, checkpoint_data);
        fprintf(stderr, "Checkpoint loaded, task time %.2f s, seed pos: %d\n", curr_checkpoint.timeElapsed, curr_checkpoint.seedCount);
        fclose(checkpoint_data);

        boinc_end_critical_section();
    }
}

void saveCheckpoint() {
    boinc_begin_critical_section(); // Boinc should not interrupt this

    FILE *checkpoint_data = boinc_fopen("onechunk-checkpoint", "wb");
    fwrite(&curr_checkpoint, sizeof(curr_checkpoint), 1, checkpoint_data);
    fflush(checkpoint_data);
    fclose(checkpoint_data);
    
    boinc_end_critical_section();
    boinc_checkpoint_completed(); // Checkpointing completed
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
    
    BOINC_OPTIONS options;
    boinc_options_defaults(options);
    options.normal_thread_priority = true;
    boinc_init_options(&options);
    
    loadCheckpoint();

    initBiomes();
    int* cache = (int*)malloc(sizeof(int) * 16 * 256 * 256);
    LayerStack g;
    setupGenerator(&g, MC_1_7);
    
    std::ifstream infile(filename);
    if(!infile) {
        fprintf(stderr, "Could not load input file!\n");
        boinc_finish(1);
        return 1;
    }
    
    total = countFileLines(filename);
    std::string line;
    int currLine = 0;
    
    clock_t elapsedAux = clock();
    while (std::getline(infile, line)) {
        currLine++;
        
        if(curr_checkpoint.seedCount > currLine)
            continue;
        
        vector<string> values = split(line, string(" "));
        if((values.size() - 1) % 3 != 0) {
            fprintf(stderr, "invalid line detected: %s\n", line.c_str());
            fflush(stderr);
            boinc_finish(1);
        }
        
        int64_t structureSeed = stoll(values.at(0), NULL, 10);
        for(int i = 1; i < values.size(); i += 3) {
            int mc_version = stoi(values.at(i));
            int chunkX = stoi(values.at(i + 1));
            int chunkZ = stoi(values.at(i + 2));
            if(chunkX < -100 || chunkX > 100 || chunkZ < -100 || chunkZ > 100) {
                fprintf(stderr, "invalid position detected! %s\n", line.c_str());
                fflush(stderr);
                boinc_finish(1);
            }
            
            if(mc_version == 1)
                mc1_7_positions.insert(chunkX * 1000 + chunkZ);
            else if(mc_version == 2)
                mc1_8_positions.insert(chunkX * 1000 + chunkZ);
            else {
                fprintf(stderr, "invalid version detected! %s\n", line.c_str());
                fflush(stderr);
                boinc_finish(1);
            }
        }
        
        for (int64_t upperBits = 0; upperBits < 1L << 16; upperBits++) {
            int64_t worldSeed = (upperBits << 48) | structureSeed;
            applySeed(&g, worldSeed);
            getStrongholdPositions(&g, &worldSeed, cache);
        }
        
        mc1_7_positions.clear();
        mc1_8_positions.clear();
        
        clock_t elapsed = clock() - elapsedAux;
        elapsedAux = clock();
        
        curr_checkpoint.timeElapsed += (double)elapsed / CLOCKS_PER_SEC;
        curr_checkpoint.seedCount++;
        
        boinc_fraction_done((double)curr_checkpoint.seedCount / total);
        if((curr_checkpoint.seedCount % 2) == 0 || boinc_time_to_checkpoint())
            saveCheckpoint();
    }
    
    fflush(fp);
    fclose(fp);
    free(cache);
    
    #ifdef BOINC
        boinc_begin_critical_section();
    #endif
    
    double done = (double)total;
    double speed = (done / curr_checkpoint.timeElapsed) * 65536;

    fprintf(stderr, "\nSpeed: %.2lf world seeds/s\n", speed );
    fprintf(stderr, "Done.\n");
    fprintf(stderr, "Processed %llu world seeds in %.2lfs seconds.\n", total * 65536, curr_checkpoint.timeElapsed );
    fprintf(stderr, "Have %llu output seeds.\n", curr_checkpoint.outCount );
    fflush(stderr);
    
    boinc_delete_file("onechunk-checkpoint");
    
    boinc_end_critical_section();
    boinc_finish(0);
    return 0;
}
