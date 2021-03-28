#include "hexfunction.h"

hexFunction::hexFunction()
{

}

bool hexFunction::operator ==(hexFunction b)
{
    return (
                func16 == b.func16
                && varsFuncCount == b.varsFuncCount
                && typeMnf == b.typeMnf
                && canCalculateMdnf == b.canCalculateMdnf
                && canCalculateMknf == b.canCalculateMknf
                && varsCountMdnf == b.varsCountMdnf
                && varsCountMknf == b.varsCountMknf
                && skleykiCountMdnf == b.skleykiCountMdnf
                && skleykiCountMknf == b.skleykiCountMknf
                && maxtermsCount == b.maxtermsCount
                && mintermsCount == b.mintermsCount
                );
}

//void hexFunction::operator =(hexFunction b)
//{
//    func16 = b.func16;
//    varsFuncCount = b.varsFuncCount;
//    used = b.used;
//    typeMnf = b.typeMnf;
//    canCalculateMdnf = b.canCalculateMdnf;
//    canCalculateMknf = b.canCalculateMknf;
//    varsCountMdnf = b.varsCountMdnf;
//    varsCountMknf = b.varsCountMknf;
//    skleykiCountMdnf = b.skleykiCountMdnf;
//    skleykiCountMknf = b.skleykiCountMknf;
//    maxtermsCount = b.maxtermsCount;
//    mintermsCount = b.mintermsCount;
//}
