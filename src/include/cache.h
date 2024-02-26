/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#pragma once

#include <cstdint>      // integer sizes
#include <cstddef>      // size_t
#include <vector>

using namespace std;

#define BLOCK_SIZE      (1<<BLOCK_SHIFT)    // constant
#define BLOCK_COUNT     16                  // also constant

#define BLOCK_SHIFT     4                   // because 2^4 = 16
#define BLOCK_MASK      (BLOCK_SIZE-1)      // 0X0F

#define TAG_SHIFT       8           // 4 bits for offset, 4 bits for block
#define TAG_MASK        0xFFFFFF    // 2^24 as in 32-8=24 --
                // this also allows this program to be extended for larger memory size simulation

// PhysicalMemory: we will use this class to track "physical" memory
class PhysicalMemory {
private:
    vector<uint8_t> memory;    // where we'll store the memory
    size_t size;

public:
    PhysicalMemory(size_t);
    uint8_t read(uint32_t);
    void write(uint32_t, uint8_t);
};

// individual cache entries and then the actual cache itself
class CacheEntry {
private:
    uint32_t tag;
    bool valid, dirty;
    uint8_t block[BLOCK_SIZE];

public:
    CacheEntry();
    bool hit(uint32_t);
    uint8_t read(uint32_t);
    void write(uint32_t, uint8_t);
    void display();
};

class Cache {
private:
    vector<CacheEntry> entry;
    PhysicalMemory *memory;
    size_t entryCount;
    void fetch(uint32_t);

public:
    Cache(PhysicalMemory *, size_t);
    uint8_t read(uint32_t);
    void write(uint32_t, uint8_t);
    void display();
};
