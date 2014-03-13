//
//  main.cpp
//  DiskStress
//
//  Created by rominet on 9/21/2013.
//  Copyright (c) 2013 syan.fr. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>

std::string filenameForInt(std::string folder, int number);
void writeRandomBytesToFile(long length, std::string filename);
long filelength(long min, long max);

#define USE_DEFAULTS 0

int main(int argc, const char * argv[])
{
    if(argc < 4 && !USE_DEFAULTS)
    {
        printf("DiskStress v1.0\n");
        printf("Usage: diskstress  folder  number_of_cycles  number_of_files_by_cycle  [file_max_size_mb]\n\n");
        return 0;
    }
    
    std::string folder = USE_DEFAULTS ? "/Users/rominet"  : argv[1];
    int numberOfCycles = USE_DEFAULTS ? 50                : atoi(argv[2]);
    int numberOfFiles  = USE_DEFAULTS ? 10                : atoi(argv[3]);
    int longFileSize   = (USE_DEFAULTS || argc != 5) ? 64 : atoi(argv[4]);
    
    
    FILE *pf = fopen((folder + "/rand_stat.txt").c_str(), "w");
    
    setbuf(stdout, NULL);
    setbuf(pf, NULL);
    
    int i,j;
    
    int nbS = 0, nbL = 0;
    long ttlLength = 0, ttlSecs = 0;
    
    for (i = 0; i < numberOfCycles; ++i)
    {
        time_t t = time(NULL);
        int longFiles = (arc4random() % 5 < 2 ? 1 : 0);
        
        if(longFiles) nbL += numberOfFiles;
        else          nbS += numberOfFiles;
        
        fprintf(pf, "Cycle %4d/%4d : ", i+1, numberOfCycles);
         printf(    "Cycle %4d/%4d : ", i+1, numberOfCycles);
        
        for (j = 0; j < numberOfFiles; ++j)
        {
            long l = filelength(1024, (longFiles*longFileSize+1)*1024*1024);
            ttlLength += l;
            
            fprintf(pf, "%4ld ", l / 1024 / 1024);
             printf(    "%4ld ", l / 1024 / 1024);
            
            writeRandomBytesToFile(l, filenameForInt(folder, j));
        }
        
        for (j = 0; j < numberOfFiles; ++j)
        {
            remove(filenameForInt(folder, j).c_str());
        }
        
        ttlSecs += difftime(time(NULL), t);
        
        fprintf(pf, "; %4ds\n", (int)difftime(time(NULL), t));
         printf(    "; %4ds\n", (int)difftime(time(NULL), t));
    }
    
    fclose(pf);
    
    fprintf(pf, "%d files : %d long files, %d short files, ratio %f\n", (nbL+nbS), nbL, nbS, (float)(nbL)/(float)(nbS+nbL));
     printf(    "%d files : %d long files, %d short files, ratio %f\n", (nbL+nbS), nbL, nbS, (float)(nbL)/(float)(nbS+nbL));
    
    fprintf(pf, "Total written size : %ld gb in %ldmin\n", ttlLength/1024/1024/1024, ttlSecs/60);
     printf(    "Total written size : %ld gb in %ldmin\n", ttlLength/1024/1024/1024, ttlSecs/60);
    
    return 0;
}

void writeRandomBytesToFile(long length, std::string filename)
{
    FILE* pf = fopen(filename.c_str(), "w");
    
    if(!pf) return;
    
    long totalLengthWritten = 0;
    while(totalLengthWritten * sizeof(long) < length)
    {
        long numbers = (long)arc4random();
        fwrite(&numbers, sizeof(long), sizeof(numbers), pf);
        totalLengthWritten += sizeof(numbers);
    }
    
    fclose(pf);
}

long filelength(long min, long max)
{
    long i = -1;
    while (i <= min) i = arc4random() % max;
    return i;
}

std::string filenameForInt(std::string folder, int number)
{
    std::ostringstream s;
    s << folder;
    s << "/random";
    s << number;
    return s.str();
}




