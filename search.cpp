//
// Created by willyterra on 1/13/22.
//
#include <iostream>
#include "search.h"
using namespace std;

string DeleteSpace(string markCode)
{
    if (!markCode.empty()) {
        int index = 0;

        //Delete all space
        while ((index = markCode.find(' ', index)) >= 0) {
            markCode.erase(index, 1);
        }

        index = 0;

        //Delete the header match code
        while (true) {
            index = markCode.find("??", index);

            if (index == 0)
                markCode.erase(index, 2);
            else
                break;
        }
    }
    if (markCode.length() % 2 != 0) return 0;

    return markCode;
}


int String2ByteArray(string markCode, uint8_t* pMarkCode)
{
    int markCodeLength = markCode.length()/2;
    int markCodePosition = 0;

    for (int i = 0; i < markCodeLength; i++)
    {
        string tempStr = markCode.substr(i * 2,2);

        if (tempStr == "??")
        {
            pMarkCode[i] = 0x3F;

            if (markCodePosition == 0) markCodePosition = i;
        }
        else
        {
            pMarkCode[i] = strtoul(tempStr.c_str(),0,16);
        }
    }

    return markCodePosition;
}

int PatchPatternSearch(uint8_t* pSrcBuffer,uint32_t SrcLength,uint8_t* pPatchPattern, uint8_t PatternLength, int MarkCodePos)
{
    uint32_t nOffset = 0;

    uint32_t i = 0, j = 0, nCount = 0;

    uint8_t aSunday[0xFF + 1] = {0};

    for (int i = 0; i < MarkCodePos; i++) {
        aSunday[pPatchPattern[i]] = i + 1;
    }

    while (j < PatternLength) {
        if (pSrcBuffer[i] == pPatchPattern[j] || pPatchPattern[j] == 0x3F) {
            i++;
            j++;
        }
        else {
            nOffset = i - j + MarkCodePos;

            if (nOffset > SrcLength - PatternLength) break;

            if (aSunday[pSrcBuffer[nOffset]]) {
                i = nOffset - aSunday[pSrcBuffer[nOffset]] + 1;
                j = 0;
            }
            else {
                i = nOffset + 1;
                j = 0;
            }
        }
    }

    if (j == PatternLength) {
        return i - PatternLength;
    }

    return -1;
}

int PatchPatternSearchInfo(uint8_t* pSrcBuffer,uint32_t SrcLength, stSearchPattern patData)
{
    uint32_t nOffset = 0;

    uint32_t i = 0, j = 0, nCount = 0;

    uint8_t aSunday[0xFF + 1] = {0};

    for (int i = 0; i < patData.posMarkCode; i++) {
        aSunday[patData.pPatternData[i]] = i + 1;
    }

    while (j < patData.lenSearchPattern) {
        if (pSrcBuffer[i] == patData.pPatternData[j] || patData.pPatternData[j] == 0x3F) {
            i++;
            j++;
        }
        else {
            nOffset = i - j + patData.posMarkCode;

            if (nOffset > SrcLength - patData.lenSearchPattern) break;

            if (aSunday[pSrcBuffer[nOffset]]) {
                i = nOffset - aSunday[pSrcBuffer[nOffset]] + 1;
                j = 0;
            }
            else {
                i = nOffset + 1;
                j = 0;
            }
        }
    }

    if (j == patData.lenSearchPattern) {
        return i - patData.lenSearchPattern;
    }

    return -1;
}
