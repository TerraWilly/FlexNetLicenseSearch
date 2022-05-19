//
// Created by willyterra on 1/13/22.
//
#include "pattern.h"

//create pattern struct
stSearchPattern fnSigPattern;

void SearchPatternInit(void)
{
    string patternTemp = "";

    //Search flexnet licensing signature
    patternTemp = "40 28 23 29 20 46 ?? ?? "; //@(#) F...
    fnSigPattern.pattern = DeleteSpace(patternTemp);
    fnSigPattern.pPatternData = new uint8_t [fnSigPattern.pattern.length()];
    fnSigPattern.lenSearchPattern = fnSigPattern.pattern.length()/2;
    fnSigPattern.posMarkCode = String2ByteArray(fnSigPattern.pattern,fnSigPattern.pPatternData);
}
