/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 */

#include <cache.h>

/* CONSTRUCTOR */
PhysicalMemory::PhysicalMemory(size_t size) {
    this->memory.reserve(size);
    this->size = size;

    // initialize memory according to the instructions
    for(size_t i = 0; i < size; i++) {
        this->memory[i] = (uint8_t)i & 0xFF;
    }
}

/*
 * read(): reads a byte from physical memory
 * Parameters: address - address of the byte to read
 * Returns: value from said address
 */

uint8_t PhysicalMemory::read(uint32_t address) {
    if(address >= this->size) {
        // handle out of bounds the way real hardware is likely to do
        return 0xFF;
    } else {
        return this->memory[address];
    }
}

/*
 * write(): writes a byte to physical memory
 * Parameters: address - address of the byte to write
 * Parameters: data - the byte to be written
 * Returns: nothing
 */

void PhysicalMemory::write(uint32_t address, uint8_t data) {
    if(address < this->size) {
        this->memory[address] = data;
    }
}
