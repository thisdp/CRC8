#pragma once
#include "Arduino.h"

class CRC8Model {
public:
    uint8_t xorIn;
    uint8_t xorOut;
    uint8_t polynomial;
    uint8_t reflectIn;
    uint8_t reflectOut;
};

extern CRC8Model CRC8DEFAULT;
extern CRC8Model CRC8ITU;
extern CRC8Model CRC8ROHC;
extern CRC8Model CRC8MAXIM;

class CRC8 {
public:
    CRC8(CRC8Model cfg);
    CRC8(uint8_t reflectIn, uint8_t reflectOut, uint8_t polynomial, uint8_t xorIn, uint8_t xorOut);
    void setModel(CRC8Model cfg);
    void updateRaw(uint8_t data);
    void updateRaw(uint8_t* data, uint32_t len);
    void update(uint8_t data);
    void update(uint8_t* data, uint32_t len);
    uint8_t get();
    void clear();
private:
    uint8_t reflectTable[256];
    uint8_t reflect(uint8_t data);
    uint8_t reflectRaw(uint8_t data);
    uint8_t calc(uint8_t data);
    void genCRCTable();
    union {
        CRC8Model config;
        struct {
            uint8_t xorIn;
            uint8_t xorOut;
            uint8_t polynomial;
            uint8_t reflectIn;
            uint8_t reflectOut;
        };
    };
    uint8_t crcTable[256];
    uint8_t crc;
};