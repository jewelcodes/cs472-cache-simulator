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

    // test
    /*cache.read(0x7A2);
    cache.read(0x2E);
    cache.read(0x2F);
    cache.read(0x3D5);
    cache.write(0x7A2, 0x88);*/

    //cache.display();

    cache.write(0x150, 0xF3);
    cache.write(0x155, 0x7D);
    cache.read(0x250);
    cache.read(0x150);
    cache.display();

    return 0;
}
