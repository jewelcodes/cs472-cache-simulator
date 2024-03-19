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

    // now make some kind of interactive prompt
    bool quit = false;
    string input;
    uint32_t address;
    uint8_t value;

    // if a filename is specified as an argument, use that as a script
    if(argc >= 2) {
        cout << "reading " << argv[1] << " as input..." << endl;
        Script script(&cache);
        script.execute(argv[1]);
        //return 0;     // on second thought there's no reason to quit here, give the prompt anyway
    }

    while(!quit) {
        cout << "(r)ead, (w)rite, (d)isplay cache, load (s)cript, or (q)uit? ";
        cin >> input;

        if(input == "r" || input == "R") {
            cout << "address to read? ";
            cin >> input;
            address = stoi(input, 0, 16);

            cache.read(address);
        } else if(input == "w" || input == "W") {
            cout << "address to write? ";
            cin >> input;
            address = stoi(input, 0, 16);

            cout << "value to write? ";
            cin >> input;
            value = stoi(input, 0, 16);

            cache.write(address, value);
        } else if(input == "d" || input == "D") {
            cache.display();
        } else if(input == "s" || input == "S") {
            cout << "file name of the script containing list of references? ";
            cin >> input;

            Script script(&cache);
            script.execute(input);
        } else if(input == "q" || input == "Q") {
            break;
        } else {
            cout << "undefined command: " << input << endl;
        }

        cout << endl;
    }

    return 0;
}
