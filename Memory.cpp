//
// Created by sheep on 11/27/2022.
//

#include "Memory.h"
#include <Windows.h>
#include <iostream>

void Memory::Patch(unsigned char *dst, unsigned char *src, unsigned int len) {
    DWORD old;
    VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &old);
    memcpy(dst, src, len);
    VirtualProtect(dst, len, old, 0);
}

bool Memory::Detour32(unsigned char *src, unsigned char *dst, unsigned int len) {
    if (len < 5) return false;

    DWORD old;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &old);
    memset(src, 0x90, len);
    uintptr_t relAddy = (uintptr_t)(dst - src - 5);
    *src = (char)0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
    VirtualProtect(src, len, old, 0);

    return true;
}

unsigned char* Memory::TrampHook32(unsigned char *src, unsigned char *dst, unsigned int len) {
    if (len < 5) return 0;
    auto gateway = (PBYTE) VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);
    uintptr_t jmpAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (char)0xE9;
    *(uintptr_t*)(gateway + len + 1) = jmpAddy;
    if (Detour32(src, dst, len)) {
        return gateway;
    } else return 0;
}
