/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 */

#pragma once

#include <cstdint>      // integer sizes
#include <cstddef>      // size_t
#include <vector>

using namespace std;

#define BLOCK_SIZE      16      // constant
#define BLOCK_COUNT     16

// PhysicalMemory: we will use this class to track "physical" memory
class PhysicalMemory {
private:
    uint8_t *memory;    // array where we'll store the memory
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
    bool hit(size_t);
    uint8_t read(uint32_t);
    void write(uint32_t, uint8_t);
};

class Cache {
private:
    vector<CacheEntry> entry;
    PhysicalMemory memory;
    size_t entryCount;

public:
    Cache(size_t);
    uint8_t read(uint32_t);
    void write(uint32_t, uint8_t);
};
