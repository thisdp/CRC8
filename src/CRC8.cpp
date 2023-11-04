#include "CRC8.h"

/*Reflect Calculations*/
uint8_t reflectLookupTable[] = {
  0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240,
  8,136,72,200,40,168,104,232,24,152,88,216,56,184,120,248,
  4,132,68,196,36,164,100,228,20,148,84,212,52,180,116,244,
  12,140,76,204,44,172,108,236,28,156,92,220,60,188,124,252,
  2,130,66,194,34,162,98,226,18,146,82,210,50,178,114,242,
  10,138,74,202,42,170,106,234,26,154,90,218,58,186,122,250,
  6,134,70,198,38,166,102,230,22,150,86,214,54,182,118,246,
  14,142,78,206,46,174,110,238,30,158,94,222,62,190,126,254,
  1,129,65,193,33,161,97,225,17,145,81,209,49,177,113,241,
  9,137,73,201,41,169,105,233,25,153,89,217,57,185,121,249,
  5,133,69,197,37,165,101,229,21,149,85,213,53,181,117,245,
  13,141,77,205,45,173,109,237,29,157,93,221,61,189,125,253,
  3,131,67,195,35,163,99,227,19,147,83,211,51,179,115,243,
  11,139,75,203,43,171,107,235,27,155,91,219,59,187,123,251,
  7,135,71,199,39,167,103,231,23,151,87,215,55,183,119,
  247,15,143,79,207,47,175,111,239,31,159,95,223,63,191,127,255,
};

uint8_t CRC8::reflectRaw(uint8_t data) {
    uint8_t reflection = 0x00;
    for (int8_t bit = 7; bit >= 0; bit--) {
        reflection |= (data & 0x01) << bit;
        data >>= 1;
    }
    return reflection;
}

uint8_t CRC8::reflect(uint8_t data) {
    return reflectLookupTable[data];
}

/*CRC Calculations*/
uint8_t CRC8::calc(uint8_t data) {
    uint8_t tCRC = data;
    for (int i = 0; i < 8; i++)
        tCRC = (tCRC << 1) ^ ((tCRC & 0x80) ? polynomial : 0);
    return tCRC;
}

void CRC8::updateRaw(uint8_t data) {
    crc ^= (reflectIn ? reflect(data) : data);
    for (int i = 0; i < 8; i++)
        crc = (crc << 1) ^ ((crc & 0x80) ? polynomial : 0);
}

void CRC8::updateRaw(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        updateRaw(data[i]);
}

void CRC8::update(uint8_t data) {
    data = reflectIn ? reflect(data) : data;
    crc = (crc << 8) ^ crcTable[crc ^ data];
    cout << (int)(crcTable[(crc >> 0) ^ data]) << endl;
}

void CRC8::update(uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        update(data[i]);
}

/*CRC Operations*/
CRC8::CRC8(CRC8Model cfg) {
    config = cfg;
    crc = xorIn;
    genCRCTable();
}

CRC8::CRC8(uint8_t _reflectIn, uint8_t _reflectOut, uint8_t _polynomial, uint8_t _xorIn, uint8_t _xorOut) {
    reflectIn = _reflectIn;
    reflectOut = _reflectOut;
    polynomial = _polynomial;
    xorIn = _xorIn;
    xorOut = _xorOut;
    crc = xorIn;
    genCRCTable();
}

void CRC8::setModel(CRC8Model cfg) {
    config = cfg;
    genCRCTable();
}

void CRC8::clear() {
    crc = xorIn;
}

uint8_t CRC8::get() {
    return (reflectOut ? reflect(crc) : crc) ^ xorOut;
}

void CRC8::genCRCTable() {
    for (uint16_t i = 0; i <= 255; i++)
        crcTable[i] = calc((uint8_t)i);
}

/*CRC8 Models*/
CRC8Model CRC8DEFAULT = {
  .xorIn = 0x00,
  .xorOut = 0x00,
  .polynomial = 0x07,
  .reflectIn = false,
  .reflectOut = false,
};

CRC8Model CRC8ITU = {
  .xorIn = 0x00,
  .xorOut = 0x55,
  .polynomial = 0x07,
  .reflectIn = false,
  .reflectOut = false,
};

CRC8Model CRC8ROHC = {
  .xorIn = 0xFF,
  .xorOut = 0x00,
  .polynomial = 0x07,
  .reflectIn = true,
  .reflectOut = true,
};

CRC8Model CRC8MAXIM = {
  .xorIn = 0x00,
  .xorOut = 0x00,
  .polynomial = 0x31,
  .reflectIn = true,
  .reflectOut = true,
};
