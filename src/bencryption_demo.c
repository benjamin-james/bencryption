#include "bencryption_demo.h"
#include "stdio.h"

void printUnicode(uint32_t *str, int strl)
{
	int i;
	for(i = 0; i < strl; i++)
	{
		char c = str[i] & 0xFF;
		putchar(c);
	}
	puts("");
}
void printHex(uint32_t *str, int strl)
{
	int i;
	for(i = 0; i < strl; i++)
	{
		printf("0x%08X ",str[i]);
	}
	puts("");
}
void printKey(uint8_t *key)
{
	int i;
	printf("\nkey:\n0x");
	for(i = 0; i < KEYSIZE; i++)
	{
		printf("%X",key[i]);
	}
	puts("");
}
int loadText(uint32_t *str, char *filename, uint8_t isUnicode)
{
	FILE *file = fopen(filename,"r");
	int i = 0;
	if(isUnicode)
	{
		char c;
		for(i = 0; (c = fgetc(file)) != EOF && i < BUF_SIZE; i++) str[i] = c;
	}
	else
	{
		//for(i = 0;fscanf(file,"%x ",str + i); i++);
		char buf[4096];
		char *p;
		while((p = fgets(buf, sizeof(buf), file)) != NULL) 
		{
			int n = 0;
			uint32_t t = 0;
			while (sscanf(p, "%x%n", &t, &n) > 0)
			{
				str[i++] = t;
				//printf("0x%08X ", str[i-1]);
				p += n;
			}
		}
	}
	fclose(file);
	return i;
}
int loadKey(uint8_t *keys, char *filename)
{
	FILE *file = fopen(filename,"r");
	int i = 0;
	uint32_t ikeys[8];
	for(i = 0; i < 8 && fscanf(file,"%x",ikeys+i); i++);
	/*fscanf(file,"%x",ikeys);
	fscanf(file,"%x",ikeys+1);
	fscanf(file,"%x",ikeys+2);
	fscanf(file,"%x",ikeys+3);
	fscanf(file,"%x",ikeys+4);
	fscanf(file,"%x",ikeys+5);
	fscanf(file,"%x",ikeys+6);
	fscanf(file,"%x",ikeys+7);
	*/
	fclose(file);
	for(i = 0; i < 8; i++)
	{
		keys[4*i] = (ikeys[i] >> 24) & 0xFF;
		keys[4*i+1] = (ikeys[i] >> 16) & 0xFF;
		keys[4*i+2] = (ikeys[i] >> 8) & 0xFF;
		keys[4*i+3] = ikeys[i] & 0xFF;
	}
	return 0;
}
