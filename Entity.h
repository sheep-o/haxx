#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include "Graphics.h"
#include "Offsets.h"

class Entity
{
public:
    union
    {
        DEFINE_MEMBER_N(bool, IsDormant, 0xED);
        DEFINE_MEMBER_N(int, Team, 0xF4);
        DEFINE_MEMBER_N(int, Health, 0x100);
        DEFINE_MEMBER_N(int, Flags, 0x104);
        DEFINE_MEMBER_N(Graphics::Vec3, VecOrigin, 0x138);
        DEFINE_MEMBER_N(int, BoneMatrix, Offsets::m_dwBoneMatrix);
        DEFINE_MEMBER_N(int, Weapon, 0x2F08);
        DEFINE_MEMBER_N(int, GlowIndex, 0xA438);

    };

    bool IsValid();
};