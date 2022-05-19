//
// Created by willyterra on 1/13/22.
//

#ifndef PATTERNSEARCH_SEARCH_H
#define PATTERNSEARCH_SEARCH_H
#include <iostream>

using namespace std;

typedef struct PatchPattern
{
    string pattern;

    uint8_t* pPatternData;
    uint8_t posMarkCode;
    uint8_t  lenSearchPattern;

}stSearchPattern;

int PatchPatternSearch(uint8_t* pSrcBuffer,uint32_t SrcLength,uint8_t* pPatchPattern, uint8_t PatternLength, int MarkCodePos);
int String2ByteArray(string markCode, uint8_t* pMarkCode);
string DeleteSpace(string markCode);
int PatchPatternSearchInfo(uint8_t* pSrcBuffer,uint32_t SrcLength, stSearchPattern patData);

#endif //PATTERNSEARCH_SEARCH_H
