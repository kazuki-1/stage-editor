#pragma once
#include <memory>;
#include <vector>
#include <filesystem>
#include <fstream>
#include "CerealHeaders.h"
#include "DXMath.h"
#include "OBJECT_DATA.h"
using namespace Math;

enum class ENEMY_TYPE
{
    MELEE, 
    RANGE, 
    BOSS
};

struct ENEMY_DATA
{
    VECTOR3 spawn_position;
    VECTOR3 direction;
    ENEMY_TYPE type;
    template<class T>
    void serialize(T* t)
    {
        t(spawn_position, direction, type);
    }
};





class STAGE_UI
{
    std::vector<OBJECT_DATA>objectDataset;
    std::vector<ENEMY_DATA>enemyDataset;
    VECTOR3 start_position;

    void RenderUI();
    void ReadFromFile(std::string f);
    void OutputFile(std::string f);


};