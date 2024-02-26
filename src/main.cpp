/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#include <iostream>
#include <cache.h>

int main(int argc, char **argv) {
    PhysicalMemory memory(2048);
    Cache cache(&memory, BLOCK_COUNT);

    cache.display();
    return 0;
}
