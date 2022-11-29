//
// Created by sheep on 11/26/2022.
//

#include "Graphics.h"

Graphics::Vec3 Graphics::WorldToScreen(float* ViewMatrix, int width, int height, Graphics::Vec3 pos){
    Vec3 out;
    float _x = ViewMatrix[0] * pos.x + ViewMatrix[1] * pos.y + ViewMatrix[2] * pos.z + ViewMatrix[3];
    float _y = ViewMatrix[4] * pos.x + ViewMatrix[5] * pos.y + ViewMatrix[6] * pos.z + ViewMatrix[7];
    out.z = ViewMatrix[12] * pos.x + ViewMatrix[13] * pos.y + ViewMatrix[14] * pos.z + ViewMatrix[15];

    _x *= 1.f / out.z;
    _y *= 1.f / out.z;

    out.x = width * .5f;
    out.y = height * .5f;

    out.x += 0.5f * _x * 1920 + 0.5f;
    out.y -= 0.5f * _y * 1080 + 0.5f;

    return out;
}

void Graphics::DrawLine(IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, D3DCOLOR color) {
    ID3DXLine* line;
    D3DXCreateLine(pDevice, &line);

    D3DXVECTOR2 vLine[2];
    vLine[0] = D3DXVECTOR2(x1, y1);
    vLine[1] = D3DXVECTOR2(x2, y2);

    line->SetWidth(1.5);
    line->Draw(vLine, 2, color);
    line->Release();
}

bool Graphics::DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message)
{
    D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, r, g, b);
    RECT rct; //Font
    rct.left=x;
    rct.right=1680;
    rct.top=y;
    rct.bottom=rct.top+200;
    font->DrawTextA(NULL, Message, -1, &rct, 0, fontColor);
    return true;
}

float Graphics::Get3dDistance(Vec3 f, Vec3 s)
{
    float calc1 = (s.x - f.x) * (s.x - f.x);
    float calc2 = (s.y - f.y) * (s.y - f.y);
    float calc3 = (s.z - f.z) * (s.z - f.z);

    return sqrt((calc1 + calc2 + calc3));
}