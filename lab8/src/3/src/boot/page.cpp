#include "os_constant.h"
#include <stdint.h>

extern "C" void open_page_mechanism()
{
    // 使用 uintptr_t 处理地址，保证类型安全
    uintptr_t page_directory_base = PAGE_DIRECTORY;
    uintptr_t page_table_base = PAGE_DIRECTORY + PAGE_SIZE;

    // 将地址转换为指针
    int *directory = (int *)page_directory_base;
    int *page = (int *)page_table_base;

    // 页目录表和页表项的数量
    int entryNum = PAGE_SIZE / sizeof(int);

    // 初始化页目录表和页表
    for (int i = 0; i < entryNum; ++i) {
        directory[i] = 0;  // 初始化页目录表
        page[i] = 0;       // 初始化线性地址 0~4MB 对应的页表
    }

    int address = 0;

    // 将线性地址 0~1MB 恒等映射到物理地址 0~1MB
    for (int i = 0; i < 256; ++i) {
        // 设置页表项，U/S = 1, R/W = 1, P = 1
        page[i] = address | 0x7;
        address += PAGE_SIZE;
    }

    // 初始化页目录项
    // 映射 0~1MB
    directory[0] = (uintptr_t)page | 0x07;

    // 映射 3GB 的内核空间
    directory[768] = directory[0];

    // 设置页目录最后一项指向页目录表，用于递归映射
    directory[1023] = (uintptr_t)directory | 0x07;
}
