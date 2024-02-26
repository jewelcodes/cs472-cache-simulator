/*
 * MET CS 472 - Computer Architecture
 * Project 2: Cache Simulator
 * Omar Elghoul
 */

#include <cache.h>

/* CONSTRUCTOR */
Cache::Cache(PhysicalMemory *memory, size_t entryCount) {
    this->memory = memory;
    this->entryCount = entryCount;
    this->entry.reserve(entryCount);
}

CacheEntry::CacheEntry() {
    this->valid = false;    // none of the other fields matter if we start with valid=false
                            // so we don't need to manually initialize them
}

