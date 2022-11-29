//
// Created by sheep on 11/27/2022.
//

#ifndef HAX_MEMORY_H
#define HAX_MEMORY_H


class Memory {
public:
    static void Patch(unsigned char* dst, unsigned char* src, unsigned int len);
    static bool Detour32(unsigned char* src, unsigned char* dst, unsigned int len);
    static unsigned char* TrampHook32(unsigned char* src, unsigned char* dst, unsigned int len);
};


#endif //HAX_MEMORY_H
