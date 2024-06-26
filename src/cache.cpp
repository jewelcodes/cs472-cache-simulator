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

/* INDIVIDUAL CACHE ENTRY FUNCTIONS */

/*
 * getTag(): returns the tag associated with this entry
 * Parameters: none
 * Returns: tag
 */

uint32_t CacheEntry::getTag() {
    return this->tag;
}

/*
 * display(): displays a single cache entry
 * Parameters: None
 * Returns: Nothing
 */

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

/* READS AND WRITES FROM/TO A SINGLE CACHE ENTRY */

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

/* MAIN CACHE FUNCTIONS, NOT FOR INDIVIDUAL ENTRIES */

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
        cout << "(miss) ";
    } else {
        cout << "(hit) ";
    }

    // hit or recently fetched
    uint8_t value = this->entry[block].read(address & BLOCK_MASK);
    cout << "read value 0x";
    cout << hex << uppercase << setw(2) << setfill('0') << (uint32_t)value;
    cout << " from address 0x";
    cout << hex << uppercase << setw(3) << setfill('0') << address << endl;
    
    return value;
}

/*
 * write(): writes to memory
 * Parameters: address - address to write to
 * Parameters: value - value to write
 * Returns: nothing
 */

void Cache::write(uint32_t address, uint8_t value) {
    // first, let's check if we have a hit
    uint32_t block = (address >> BLOCK_SHIFT) & BLOCK_MASK;

    if(!this->entry[block].hit(address)) {
        // MISS -- WE NEED TO FETCH FROM MAIN MEMORY
        cout << "(miss) ";
        this->fetch(address);
    } else {
        cout << "(hit) ";
    }

    // hit or recently fetched
    this->entry[block].write(address & BLOCK_MASK, value);
    cout << "wrote value 0x";
    cout << hex << uppercase << setw(2) << setfill('0') << (uint32_t)value;
    cout << " to address 0x";
    cout << hex << uppercase << setw(3) << setfill('0') << address << endl;
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

    // determine if we need to write back to main memory before reusing this cache entry
    if(this->entry[block].isDirty()) {
        // calculate address of the dirty block from the tag and slot number
        uint32_t dirtyBlock = this->entry[block].getTag() << TAG_SHIFT;
        dirtyBlock |= (block << BLOCK_SHIFT);

        cout << "(writeback) updating main memory block range 0x";
        cout << hex << uppercase << setw(3) << setfill('0') << dirtyBlock << "-0x";
        cout << hex << uppercase << setw(3) << setfill('0') << dirtyBlock+BLOCK_SIZE-1 << endl;

        // now write the contents of the dirty cache to main memory
        for(int i = 0; i < BLOCK_SIZE; i++) {
            this->memory->write(dirtyBlock+i, this->entry[block].read((dirtyBlock+i) & BLOCK_MASK));
        }
    }

    for(int i = 0; i < BLOCK_SIZE; i++) {
        // read the entire block into the cache from main memory
        this->entry[block].write((blockStart+i) & BLOCK_MASK, this->memory->read(blockStart+i));
    }

    this->entry[block].populate(blockStart);
}
