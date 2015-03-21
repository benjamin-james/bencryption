#include "bencryption.h"
#ifndef BENCRYPTION_DEMO_H
#define BENCRYPTION_DEMO_H

#define KEYSIZE 32
#define BUF_SIZE 512

void printUnicode(uint32_t *str, int strl);
void printHex(uint32_t *str, int strl);
void printKey(uint8_t *key);
int loadText(uint32_t *str, char *filename, uint8_t i);
int loadKey(uint8_t *keys, char *filename);

#endif
