#ifndef PRESETS_H
#define PRESETS_H

#include <QString>


class presets
{
public:
    presets(){};
    presets(QString name,
            int varsFuncCount,
            int varsCountMknfFrom,
            int varsCountMknfTo,
            int varsCountMdnfFrom,
            int varsCountMdnfTo,
            int skleykiCountMknf, // количество склеек
            int skleykiCountMdnf, // количество склеек
            int maxtermsCountFrom , // количество макстермов
            int maxtermsCountTo,
            int mintermsCountFrom, // количество минтермов
            int mintermsCountTo,
            int generateType,
            bool fullNoPovtor,
            bool noPovtorInGroup
            );


    int getVarsFuncCount() const;
    void setVarsFuncCount(int value);

    int getSkleykiCountMknf() const;
    void setSkleykiCountMknf(int value);

    int getSkleykiCountMdnf() const;
    void setSkleykiCountMdnf(int value);

    bool operator == (presets value);

    int getVarsCountMknfFrom() const;
    void setVarsCountMknfFrom(int value);

    int getVarsCountMknfTo() const;
    void setVarsCountMknfTo(int value);

    int getVarsCountMdnfFrom() const;
    void setVarsCountMdnfFrom(int value);

    int getVarsCountMdnfTo() const;
    void setVarsCountMdnfTo(int value);

    int getMaxtermsCountFrom() const;
    void setMaxtermsCountFrom(int value);

    int getMaxtermsCountTo() const;
    void setMaxtermsCountTo(int value);

    int getMintermsCountFrom() const;
    void setMintermsCountFrom(int value);

    int getMintermsCountTo() const;
    void setMintermsCountTo(int value);

    QString getName() const;
    void setName(const QString &value);

    int getGenerateType() const;
    void setGenerateType(int value);

    bool getFullNoPovtor() const;
    void setFullNoPovtor(bool value);

    bool getNoPovtorInGroup() const;
    void setNoPovtorInGroup(bool value);

private:
    QString name = "";
    int varsFuncCount = 0; // количество переменных в векторе
    int varsCountMknfFrom = -1;
    int varsCountMknfTo = -1;
    int varsCountMdnfFrom = -1;
    int varsCountMdnfTo = -1;
    int skleykiCountMknf = -1; // количество склеек
    int skleykiCountMdnf = -1; // количество склеек
    int maxtermsCountFrom = -1; // количество макстермов
    int maxtermsCountTo = -1;
    int mintermsCountFrom = -1; // количество минтермов
    int mintermsCountTo = -1;
    int generateType = -1; // тип генерации (для кого - всех, группы, выбранных)
    bool fullNoPovtor = false;
    bool noPovtorInGroup = false;

};

#endif // PRESETS_H
