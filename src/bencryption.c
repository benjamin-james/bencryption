#include "bencryption.h"
   
#define w 32    //word size
#define r 20    //number of rounds
 
#define P32 0xB7E15163  //constants for key schedule
#define Q32 0x9E3779B9
 
#define bytes   (w / 8)             //bytes per word
#define c       ((b + bytes - 1) / bytes)   //key in words, rounded up
#define R24     (2 * r + 4)
#define logw     5                           //log2(32) == 5 

//circular rotation
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))
 
uint32_t S[R24 - 1];        //key schedule

void encrypt(uint32_t *pt, uint32_t *ct, int32_t len)
{
	int32_t i;
	uint32_t npt[4],nct[4];
	for(i = 0; i < len; i += 4)
	{
		npt[0] = pt[i];
		npt[1] = pt[i+1];
		npt[2] = pt[i+2];
		npt[3] = pt[i+3];
		encrypt_block(npt,nct);
		ct[i] = nct[0];
		ct[i+1] = nct[1];
		ct[i+2] = nct[2];
		ct[i+3] = nct[3];
	}
}
void decrypt(uint32_t *ct, uint32_t *pt, int32_t len)
{
	int32_t i;
	uint32_t npt[4],nct[4];
	for(i = 0; i < len; i += 4)
	{
		nct[0] = ct[i];
		nct[1] = ct[i+1];
		nct[2] = ct[i+2];
		nct[3] = ct[i+3];
		decrypt_block(nct,npt);
		pt[i] = npt[0];
		pt[i+1] = npt[1];
		pt[i+2] = npt[2];
		pt[i+3] = npt[3];
	}
}

void init(uint8_t *K, int32_t b)
{
    int32_t i, j, s, v;
    uint32_t L[(32 + bytes - 1) / bytes]; //should be big enough
    uint32_t A, B;
 
    L[c - 1] = 0;
    for (i = b - 1; i >= 0; i--)
        L[i / bytes] = (L[i / bytes] << 8) + K[i];
 
    S[0] = P32;
    for (i = 1; i <= 2 * r + 3; i++)
        S[i] = S[i - 1] + Q32;
 
    A = B = i = j = 0;
    v = R24;
    if (c > v) v = c;
    v *= 3;
 
    for (s = 1; s <= v; s++)
    {
        A = S[i] = ROTL(S[i] + A + B, 3);
        B = L[j] = ROTL(L[j] + A + B, A + B);
        i = (i + 1) % R24;
        j = (j + 1) % c;
    }
}
 
void encrypt_block(uint32_t *pt, uint32_t *ct)
{
    uint32_t A, B, C, D, t, u, x;
    int32_t i;
 
    A = pt[0];
    B = pt[1];
    C = pt[2];
    D = pt[3];
    B += S[0];
    D += S[1];
    for (i = 2; i <= 2 * r; i += 2)
    {
        t = ROTL(B * (2 * B + 1), logw);
        u = ROTL(D * (2 * D + 1), logw);
        A = ROTL(A ^ t, u) + S[i];
        C = ROTL(C ^ u, t) + S[i + 1];
        x = A;
        A = B;
        B = C;
        C = D;
        D = x;
    }
    A += S[2 * r + 2];
    C += S[2 * r + 3];
    ct[0] = A;
    ct[1] = B;
    ct[2] = C;
    ct[3] = D;
}
 
void decrypt_block(uint32_t *ct, uint32_t *pt)
{
    uint32_t A, B, C, D, t, u, x;
    int32_t i;
 
    A = ct[0];
    B = ct[1];
    C = ct[2];
    D = ct[3];
    C -= S[2 * r + 3];
    A -= S[2 * r + 2];
    for (i = 2 * r; i >= 2; i -= 2)
    {
        x = D;
        D = C;
        C = B;
        B = A;
        A = x;
        u = ROTL(D * (2 * D + 1), logw);
        t = ROTL(B * (2 * B + 1), logw);
        C = ROTR(C - S[i + 1], t) ^ u;
        A = ROTR(A - S[i], u) ^ t;
    }
    D -= S[1];
    B -= S[0];
    pt[0] = A;
    pt[1] = B;
    pt[2] = C;
    pt[3] = D;  
}
