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
    uint8_t value;
    bool pause = false;     // whether we will pause after every instruction

    while(getline(file, line)) {
        if(line[0] == 'd') {
            this->cache->display();
        } else if(line[0] == 'r') {
            address = stoi(&line[2], 0, 16);
            this->cache->read(address);
        } else if(line[0] == 'w') {
            address = stoi(&line[2], 0, 16);

            if(!getline(file, line)) {
                cout << "unexpected end of file" << endl;
                return;
            }

            value = stoi(line, 0, 16);
            this->cache->write(address, value);
        } else if(line[0] == 'p') {
            // this isn't part of the assignment but will make a recording of the submission easier!
            pause = !pause;
            if(pause) cout << "press enter after each operation to proceed to the next" << endl;
        }

        if(pause && line[0] != 'p') {
            //cout << " (press enter to continue) ";
            cin.get();
        }
    }
}