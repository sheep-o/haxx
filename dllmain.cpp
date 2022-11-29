#include <Windows.h>
#include <iostream>
#include <gdiplus.h>
#include "Offsets.h"
#include "Graphics.h"
#include "Memory.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "IClientEntityList.h"
#include "Entity.h"

float ViewMatrix[16];

int WIDTH = 1920;
int HEIGHT = 1080;

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
endScene pEndScene;

auto moduleBase = (DWORD)GetModuleHandle("client.dll");

LPD3DXFONT m_font = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 0, 0);
int pPadding = 1;

void Aimbot(IDirect3DDevice9* pDevice) {

}

void ESP(IDirect3DDevice9* pDevice) {
    if (m_font == nullptr)
        D3DXCreateFont( pDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );

    DWORD localPlr = *(DWORD*)(moduleBase + Offsets::dwLocalPlayer);
    int localTeam = *(int*)(localPlr + Offsets::m_iTeamNum);

    auto pViewMatrix = (float*)(moduleBase + Offsets::dwViewMatrix);
    memcpy(ViewMatrix, pViewMatrix, 64);

    for (int i = 0; i < 32; i++) {
        DWORD entity = *(DWORD*)(moduleBase + Offsets::dwEntityList + i * 0x10);
        if (entity) {
            int team = *(int*)(entity + Offsets::m_iTeamNum);
            int health = *(int*)(entity + Offsets::m_iHealth);
            bool isDormant = *(bool*)(entity + Offsets::m_bDormant);
            if (team != localTeam && health > 0 && !isDormant) {
                Graphics::Vec3 o = *(Graphics::Vec3*)(entity + 0x138);

                Graphics::Vec3 sPos = Graphics::WorldToScreen(ViewMatrix, 1920, 1080, o);

                if (sPos.x >= 0 && sPos.y >= 0 && sPos.z >= 0 && sPos.x <= 1920 && sPos.y <= 1080) {
                    DWORD boneMatrix = *(DWORD*)(entity + Offsets::m_dwBoneMatrix);
//                    Graphics::Vec3 wOldPos;
//                    Graphics::Vec3 bonePos;
//                    wOldPos.x = *(float*)(boneMatrix + 0x30 * 0 + 0x0C);
//                    wOldPos.y = *(float*)(boneMatrix + 0x30 * 0 + 0x1C);
//                    wOldPos.z = *(float*)(boneMatrix + 0x30 * 0 + 0x2C);
//                    Graphics::Vec3 oldPos = Graphics::WorldToScreen(ViewMatrix, 1920, 1080, wOldPos);
//                    for (int j = 1; j < 85; j++) {
//                        if (j == 1 || j == 2) continue;
//                        else {
//                            bonePos.x = *(float*)(boneMatrix + 0x30 * j + 0x0C);
//                            bonePos.y = *(float*)(boneMatrix + 0x30 * j + 0x1C);
//                            bonePos.z = *(float*)(boneMatrix + 0x30 * j + 0x2C);
//                            Graphics::Vec3 sBonePos = Graphics::WorldToScreen(ViewMatrix, 1920, 1080, bonePos);
//                            Graphics::DrawLine(pDevice, oldPos.x, oldPos.y, sBonePos.x, sBonePos.y);
//                            oldPos = sBonePos;
//                            //DrawMessage(m_font, sBonePos.x, sBonePos.y, 255, 255, 0, 0, "x");
//                        }
//                    }
//---------------------------------------------------------------------------------------------------------------------------
                    Graphics::Vec3 bonePos;
                    bonePos.x = *(float*)(boneMatrix + 0x30 * 8 + 0x0C);
                    bonePos.y = *(float*)(boneMatrix + 0x30 * 8 + 0x1C);
                    bonePos.z = *(float*)(boneMatrix + 0x30 * 8 + 0x2C);
                    Graphics::Vec3 sHead = Graphics::WorldToScreen(ViewMatrix, 1920, 1080, bonePos);

                    //DrawMessage(m_font, sBonePos.x, sBonePos.y, 255, 255, 0, 0, "x");

                    float height = std::abs(sHead.y - sPos.y);
                    float width = height / 2 + pPadding;
                    float wh = width / 2 + pPadding;
                    // draw box
                    Graphics::DrawLine(pDevice,sPos.x - wh, sPos.y, sPos.x + wh, sPos.y, color); //bottom
                    Graphics::DrawLine(pDevice,sPos.x - wh, sHead.y, sPos.x + wh, sHead.y, color); //top
                    Graphics::DrawLine(pDevice,sPos.x - wh, sPos.y, sPos.x - wh, sHead.y, color); //left
                    Graphics::DrawLine(pDevice,sPos.x + wh, sPos.y, sPos.x + wh, sHead.y, color); //right

                    //printf("[%f, %f, %f]\n", sPos.x, sPos.y, sPos.z);
                }
            }
        }
    }
}

typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
void* GetInterface(const char* dllName, const char* interfaceName)
{
    tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllName), "CreateInterface");

    void* newInterface = CreateInterface(interfaceName, 0);

    return newInterface;
}

auto entityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
void ESP2(IDirect3DDevice9* pDevice) {
    Entity* localPlr = (Entity*)(moduleBase + Offsets::dwLocalPlayer);

    auto pViewMatrix = (float*)(moduleBase + Offsets::dwViewMatrix);
    memcpy(ViewMatrix, pViewMatrix, 64);
`
    for (int i = 0; i < 32; i++) {
        Entity* ent = (Entity*)entityList->GetClientEntity(i);

        if (ent->IsValid() && ent != localPlr && ent->Team != localPlr->Team) {
            Graphics::Vec3 origin = ent->VecOrigin;
            Graphics::Vec3 screen = Graphics::WorldToScreen(ViewMatrix, WIDTH, HEIGHT, origin);

            DWORD boneMatrix = ent->BoneMatrix;
            Graphics::Vec3 headPos;
            headPos.x = *(float*)(boneMatrix + 0x30 * 8 + 0x0C);
            headPos.y = *(float*)(boneMatrix + 0x30 * 8 + 0x1C);
            headPos.z = *(float*)(boneMatrix + 0x30 * 8 + 0x2C);
            Graphics::Vec3 sHead = Graphics::WorldToScreen(ViewMatrix, WIDTH, HEIGHT, headPos);

            float height = std::abs(sHead.y - screen.y);
            float width = height / 2;
            float wh = width / 2;

            // draw box
            Graphics::DrawLine(pDevice,screen.x - wh, screen.y, screen.x + wh, screen.y, color); //bottom
            Graphics::DrawLine(pDevice,screen.x - wh, sHead.y, screen.x + wh, sHead.y, color); //top
            Graphics::DrawLine(pDevice,screen.x - wh, screen.y, screen.x - wh, sHead.y, color); //left
            Graphics::DrawLine(pDevice,screen.x + wh, screen.y, screen.x + wh, sHead.y, color); //right
        }
    }
}

bool bMenu = true;
bool bESP = false;
bool bAimbot = false;

bool imInit = false;

int pColorR = 255;
int pColorG = 0;
int pColorB = 0;

HRESULT __stdcall hEndScene(IDirect3DDevice9* pDevice)
{
    //ESP(pDevice);
    if (!imInit) {
        oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9"), GWL_WNDPROC, (LONG_PTR)WndProc);
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
        ImGui_ImplWin32_Init(FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9"));
        ImGui_ImplDX9_Init(pDevice);
        imInit = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::GetStyle().WindowRounding = 10.f;

    ImGui::Begin("LOSE");

    ImGui::SetWindowSize({215, 200}, 0);
    ImGui::Checkbox("ESP", &bESP);
    ImGui::Separator();
    ImGui::SliderInt("ESP R", &pColorR, 0, 255);
    ImGui::SliderInt("ESP G", &pColorG, 0, 255);
    ImGui::SliderInt("ESP B", &pColorB, 0, 255);
    color = D3DCOLOR_ARGB(255, pColorR, pColorG, pColorB);
    ImGui::Separator();
    ImGui::Checkbox("Aimbot", &bAimbot);
    ImGui::End();

    ImGui::EndFrame();

    if (bMenu) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    if (bESP)
        ESP2(pDevice);
    if (bAimbot)
        Aimbot(pDevice);

    return pEndScene(pDevice);
}

BYTE EndSceneBytes[7];

VOID WINAPI CheatMain(HMODULE hModule) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    // endscene hook
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!pD3D)
        return;

    D3DPRESENT_PARAMETERS d3dParams = { 0 };
    d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dParams.hDeviceWindow = GetForegroundWindow();
    d3dParams.Windowed = true;

    IDirect3DDevice9* pDevice = nullptr;

    HRESULT res = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dParams.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParams, &pDevice);

    if (FAILED(res) || !pDevice)
    {
        pD3D->Release();
        return;
    }

    void** vTable = *reinterpret_cast<void***>(pDevice);

    memcpy(EndSceneBytes, (PBYTE)vTable[42], 7);

    pEndScene = (endScene)Memory::TrampHook32((PBYTE)vTable[42], (PBYTE)hEndScene, 7);
    // endscene hook

    while (!GetAsyncKeyState(VK_END)) {
        if (GetAsyncKeyState(VK_INSERT) & 0x8000)
            bMenu = !bMenu;

        Sleep(75);
    }

    SetWindowLongPtr(FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9"), GWL_WNDPROC, (LONG_PTR)oWndProc);
    Memory::Patch((BYTE*)vTable[42], EndSceneBytes, 7);

    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatMain, hModule, 0, 0);
    }

    return TRUE;
}