#include "stdint.h"

#ifndef BENCRYPTION_H
#define BENCRYPTION_H

void init(uint8_t *K, int32_t b); //key, key length
void encrypt_block(uint32_t *pt, uint32_t *ct);
void decrypt_block(uint32_t *ct, uint32_t *pt);

void encrypt(uint32_t *pt, uint32_t *ct, int32_t len);
void decrypt(uint32_t *ct, uint32_t *pt, int32_t len);

#endif
