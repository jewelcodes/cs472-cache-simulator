/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#include <iostream>
#include <iomanip>
#include <cache.h>

/* CONSTRUCTORS */
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
    cout << " #  V  D  T  DATA" << endl;

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

    if(this->dirty) {
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
    this->dirty = true;
}

/*
 * hit(): determines whether this cache entry is a hit or miss
 * Parameters: address - absolute address to read
 * Returns: true/false
 */

bool CacheEntry::hit(uint32_t address) {
    uint32_t tag = (address >> TAG_SHIFT) & TAG_MASK;
    return (this->valid) && (this->tag == tag);
}

/*
 * isDirty(): returns whether this cache entry needs to be written to main memory
 * Parameters: none
 * Returns: true/false
 */

bool CacheEntry::isDirty() {
    return (this->valid) && (this->dirty);
}

/* Helper function after being recently fetched from main memory */
void CacheEntry::populate(uint32_t address) {
    uint32_t tag = (address >> TAG_SHIFT) & TAG_MASK;

    this->valid = true;
    this->dirty = false;
    this->tag = tag;
}

/*
 * read(): reads from memory
 * Parameters: address - address to read
 * Returns: value at said address
 */

uint8_t Cache::read(uint32_t address) {
    // first, let's check if we have a hit
    uint32_t block = (address >> BLOCK_SHIFT) & BLOCK_MASK;

    if(!this->entry[block].hit(address)) {
        // MISS -- WE NEED TO FETCH FROM MAIN MEMORY
        this->fetch(address);
    }

    // hit or recently fetched
    return this->entry[block].read(address & BLOCK_MASK);
}

/*
 * fetch(): fetches a block from main memory into the cache
 * Because we are implementing a writeback cache, this function is also where
 * main memory is updated - if a block is already in the cache entry we are
 * trying to access and is dirty, we update main memory before fetching the
 * new block.
 * 
 * Parameters: address - address to fetch
 * Returns: nothing
 */

void Cache::fetch(uint32_t address) {
    uint32_t block = (address >> BLOCK_SHIFT) & BLOCK_MASK;
    uint32_t blockStart = address & ~BLOCK_MASK;

    // TODO: Determine if we need to write back before reading

    for(int i = 0; i < BLOCK_SIZE; i++) {
        // read the entire block into the cache from main memory
        this->entry[block].write((blockStart+i) & BLOCK_MASK, this->memory->read(blockStart+i));
    }

    this->entry[block].populate(blockStart);
}
