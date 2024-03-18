/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#include <iostream>
#include <fstream>
#include <cache.h>

// CONSTRUCTOR
Script::Script(Cache *cache) {
    this->cache = cache;
}

/*
 * execute(): executes a script containing read/write instructions
 * Parameters: filename - file name of the script
 * Returns: nothing
 */

void Script::execute(string filename) {
    ifstream file(filename, ios::in);
    string line;
    uint32_t address;

    while(getline(file, line)) {
        if(line[0] == 'd') {
            this->cache->display();
        } else if(line[0] == 'r') {
            address = stoi(&line[2], 0, 16);
            this->cache->read(address);
        }
    }
}