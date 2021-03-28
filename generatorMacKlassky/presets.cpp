#include "presets.h"

presets::presets(QString name, int varsFuncCount, int varsCountMknfFrom, int varsCountMknfTo, int varsCountMdnfFrom, int varsCountMdnfTo, int skleykiCountMknf, int skleykiCountMdnf, int maxtermsCountFrom, int maxtermsCountTo, int mintermsCountFrom, int mintermsCountTo, int generateType, bool fullNoPovtor, bool noPovtorInGroup)
{
    setName(name);
    setVarsFuncCount(varsFuncCount);
    setVarsCountMknfFrom(varsCountMknfFrom);
    setVarsCountMknfTo(varsCountMknfTo);
    setVarsCountMdnfFrom(varsCountMdnfFrom);
    setVarsCountMdnfTo(varsCountMdnfTo);
    setSkleykiCountMknf(skleykiCountMknf);
    setSkleykiCountMdnf(skleykiCountMdnf);
    setMaxtermsCountFrom(maxtermsCountFrom);
    setMaxtermsCountTo(maxtermsCountTo);
    setMintermsCountFrom(mintermsCountFrom);
    setMintermsCountTo(mintermsCountTo);
    setGenerateType(generateType);
    setFullNoPovtor(fullNoPovtor);
    setNoPovtorInGroup(noPovtorInGroup);
}

int presets::getVarsFuncCount() const
{
    return varsFuncCount;
}

void presets::setVarsFuncCount(int value)
{
    varsFuncCount = value;
}

int presets::getSkleykiCountMknf() const
{
    return skleykiCountMknf;
}

void presets::setSkleykiCountMknf(int value)
{
    skleykiCountMknf = value;
}

int presets::getSkleykiCountMdnf() const
{
    return skleykiCountMdnf;
}

void presets::setSkleykiCountMdnf(int value)
{
    skleykiCountMdnf = value;
}

bool presets::operator ==(presets value)
{
    return (
                varsFuncCount == value.varsFuncCount &&
                varsCountMknfFrom == value.varsCountMknfFrom &&
                varsCountMknfTo== value.varsCountMknfTo &&
                varsCountMdnfFrom == value.varsCountMdnfFrom &&
                varsCountMdnfTo == value.varsCountMdnfTo &&
                skleykiCountMknf == value.skleykiCountMknf &&
                skleykiCountMdnf == value.skleykiCountMdnf &&
                maxtermsCountFrom == value.maxtermsCountFrom &&
                maxtermsCountTo == value.maxtermsCountTo &&
                mintermsCountFrom == value.mintermsCountFrom &&
                mintermsCountTo == value.mintermsCountTo &&
                generateType == value.generateType &&
                fullNoPovtor == value.fullNoPovtor &&
                noPovtorInGroup == value.noPovtorInGroup
                );
}

int presets::getVarsCountMknfFrom() const
{
    return varsCountMknfFrom;
}

void presets::setVarsCountMknfFrom(int value)
{
    varsCountMknfFrom = value;
}

int presets::getVarsCountMknfTo() const
{
    return varsCountMknfTo;
}

void presets::setVarsCountMknfTo(int value)
{
    varsCountMknfTo = value;
}

int presets::getVarsCountMdnfFrom() const
{
    return varsCountMdnfFrom;
}

void presets::setVarsCountMdnfFrom(int value)
{
    varsCountMdnfFrom = value;
}

int presets::getVarsCountMdnfTo() const
{
    return varsCountMdnfTo;
}

void presets::setVarsCountMdnfTo(int value)
{
    varsCountMdnfTo = value;
}

int presets::getMaxtermsCountFrom() const
{
    return maxtermsCountFrom;
}

void presets::setMaxtermsCountFrom(int value)
{
    maxtermsCountFrom = value;
}

int presets::getMaxtermsCountTo() const
{
    return maxtermsCountTo;
}

void presets::setMaxtermsCountTo(int value)
{
    maxtermsCountTo = value;
}

int presets::getMintermsCountFrom() const
{
    return mintermsCountFrom;
}

void presets::setMintermsCountFrom(int value)
{
    mintermsCountFrom = value;
}

int presets::getMintermsCountTo() const
{
    return mintermsCountTo;
}

void presets::setMintermsCountTo(int value)
{
    mintermsCountTo = value;
}

QString presets::getName() const
{
    return name;
}

void presets::setName(const QString &value)
{
    name = value;
}

int presets::getGenerateType() const
{
    return generateType;
}

void presets::setGenerateType(int value)
{
    generateType = value;
}

bool presets::getFullNoPovtor() const
{
    return fullNoPovtor;
}

void presets::setFullNoPovtor(bool value)
{
    fullNoPovtor = value;
}

bool presets::getNoPovtorInGroup() const
{
    return noPovtorInGroup;
}

void presets::setNoPovtorInGroup(bool value)
{
    noPovtorInGroup = value;
}
