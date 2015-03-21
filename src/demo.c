#include "bencryption_demo.h"
#include "stdio.h"
#include "stdlib.h"

const char options[5][10] = {
 "--mode","--message","--key","--input","--output"
};
int main(int argc, char **argv)
{
	if(argc < 3) 
	{
		printf("format:\t%s (--mode [encrypt || decrypt]) --message message.txt --key key.txt (--input [hex || unicode]) (--output [hex || unicode]) \n\t(note: options in parentheses are optional, and the format of the key file is 8 32 bit unsigned integers in hexadecimal)\n",argv[0]);
		return -1;
	}
	uint8_t mode = 255;// 0b00111111
	int inIndex = -1;
	int strl = -1;
	int i,j,k;
	uint32_t str[BUF_SIZE];
	uint32_t out[BUF_SIZE];
	uint8_t key[KEYSIZE];
	for(i = 1; i < argc; i++)
	{
		k = -1;
		for(j = 0; j < 5; j++)
		{
			if(!strcmp(argv[i],options[j]))
			{
				k = j;
				i++;
				break;
			}
		}
		switch(k)
		{
			case 0:	if(!strcmp(argv[i],"encrypt"))
					{
						mode ^= 1; //0b00000001 -> turning off decrypt
					}
					else if(!strcmp(argv[i],"decrypt"))
					{
						mode ^= 2; //0b00000010 -> turning off encrypt
					}
					break;
			case 1:	inIndex = i;
					break;
			case 2:	loadKey(key,argv[i]);
					break;
			case 3:	if(!strcmp(argv[i],"hex"))
					{
						mode ^= 4; //0b00000100 -> turning off unicode
					}
					else if(!strcmp(argv[i],"unicode"))
					{
						mode ^= 8; //0b00001000 -> turning off hex
					}
					break;
			case 4:	if(!strcmp(argv[i],"hex"))
					{
						mode ^= 16; //0b00010000 -> turning off unicode
					}
					else if(!strcmp(argv[i],"unicode"))
					{
						mode ^= 32; //0b00100000 -> turning off hex
					}
			default: break;
		}
	}
	strl = loadText(str,argv[inIndex],mode & 4);
	//printKey(key);
	init(key,KEYSIZE);
	if(mode & 2) 
	{
		encrypt(str,out,strl);
		//puts("encrypted:");
		if(mode & 32)
		{
			printHex(out,strl);
		}
		if(mode & 16)
		{
			printUnicode(out,strl);
		}
	}
	if(mode & 1)
	{
		decrypt(str,out,strl);
		//puts("decrypted:");
		if(mode & 32)
		{
			printHex(out,strl);
		}
		if(mode & 16)
		{
			printUnicode(out,strl);
		}
	}
	
	return 0;
}
