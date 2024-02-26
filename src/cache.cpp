/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#include <iostream>
#include <iomanip>
#include <cache.h>

/* CONSTRUCTOR */
Cache::Cache(PhysicalMemory *memory, size_t entryCount) {
    this->memory = memory;
    this->entryCount = entryCount;
    this->entry.reserve(entryCount);
}

CacheEntry::CacheEntry() {
    this->valid = false;
    this->dirty = false;
    this->tag = 0;

    for(int i = 0; i < BLOCK_SIZE; i++) {
        this->block[i] = 0;
    }
}

/*
 * display(): displays the contents of the cache
 * Parameters: None
 * Returns: Nothing
 */

void Cache::display() {
    cout << " #  V  T  DATA" << endl;

    for(int i = 0; i < this->entryCount; i++) {
        cout << " " << hex << uppercase << setw(1) << i << "  ";
        this->entry[i].display();
    }
}

void CacheEntry::display() {
    if(this->valid) {
        cout << "1";
    } else {
        cout << "0";
    }

    cout << "  ";
    cout << hex << uppercase << setw(1) << this->tag << "  ";
    
    for(int i = 0; i < BLOCK_SIZE; i++) {
        cout << hex << uppercase << setw(2) << setfill('0') << (uint32_t)this->block[i] << " ";
    }

    cout << endl;
}

/* INDIVIDUAL READS/WRITES FROM A SINGLE CACHE ENTRY */
uint8_t CacheEntry::read(uint32_t offset) {
    return this->block[offset&BLOCK_MASK];
}

void CacheEntry::write(uint32_t offset, uint8_t data) {
    this->block[offset&BLOCK_MASK] = data;
}

bool CacheEntry::hit(uint32_t address) {
    uint32_t tag = (address >> TAG_SHIFT) & TAG_MASK;

    if(this->valid && this->tag == tag) {
        return true;
    } else {
        return false;
    }
}

