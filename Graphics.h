//
// Created by sheep on 11/26/2022.
//

#ifndef HAX_GRAPHICS_H
#define HAX_GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

namespace Graphics {
    class Vec4 {
    public:
        Vec4() {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }

        Vec4(float x, float y, float z, float w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        float x, y, z, w;
    };

    class Vec3 {
    public:
        Vec3() {
            x = 0;
            y = 0;
            z = 0;
        }

        Vec3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        float x, y, z;
    };

    class Vec2 {
    public:
        Vec2() {
            x = 0;
            y = 0;
        }

        Vec2(float x, float y) {
            this->x = x;
            this->y = y;
        }

        float x, y;
    };

    Vec3 WorldToScreen(float* ViewMatrix, int width, int height, Vec3 pos);
    void DrawLine(IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, D3DCOLOR color);
    bool DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message);
    float Get3dDistance(Vec3 f, Vec3 s);
};

#endif //HAX_GRAPHICS_H
