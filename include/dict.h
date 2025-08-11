#pragma once

#include <cstring>

#define MAX_ENTRIES 8

class Dict {
private:
    const char* keys[MAX_ENTRIES];
    uint8_t values[MAX_ENTRIES];
    int count = 0;

public:
    void set(const char* key, int value) {
        for (int i = 0; i < count; ++i) {
            if (strcmp(keys[i], key) == 0) {
                values[i] = value;
                return;
            }
        }
        if (count < MAX_ENTRIES) {
            keys[count] = key;
            values[count] = value;
            ++count;
        }
    }

    uint8_t get(const char* key, uint8_t default_val) {
        for (int i = 0; i < count; ++i) {
            if (strcmp(keys[i], key) == 0) {
                return values[i];
            }
        }
        return default_val;
    }

    bool has(const char* key) {
        for (int i = 0; i < count; ++i) {
            if (strcmp(keys[i], key) == 0) {
                return true;
            }
        }
        return false;
    }

    void remove(const char* key) {
        for (int i = 0; i < count; ++i) {
            if (strcmp(keys[i], key) == 0) {
                for (int j = i; j < count - 1; ++j) {
                    keys[j] = keys[j + 1];
                    values[j] = values[j + 1];
                }
                --count;
                return;
            }
        }
    }
};