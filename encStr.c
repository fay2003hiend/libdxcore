#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char u8;

void printHex(u8 *buf, int len) {
	int i = 0;
	for (; i < len; i++) {
		printf("0x%02X,", *(buf + i));
	}
}

u8 crc(void *buf, int len) {
	u8 ret = *((u8 *) buf);
	int i = 1;
	for (; i < len; i++) {
		ret ^= *((u8 *) buf + i);
	}
	return ret;
}

void shift(int shift, void *buf, int len) {
	u8 a, b;
	int s = shift;
	int i = 0;
	for (; i < len; i++) {
		u8 *p = (u8 *) buf + i;
		u8 q = *p;
		a = (q << (8 - s)) & 0xFF;
		b = (q >> s) & 0xFF;
		*p = a | b;
		s = 8 - s;
	}
}

char *encStr(u8 *src, int len) {
	char *ret = (char *) malloc(len + 1);
	memcpy(ret, src, len);
	ret[len] = crc(src, len);

	int s = 8 - ret[len] % 8;
	if (s == 8) {
		s = 3;
	}
	shift(s, ret, len);
	return ret;
}

char *getStr(u8 *encoded, int len) {
	char *ret = (char *) malloc(len);
	memcpy(ret, encoded, len - 1);
	ret[len - 1] = '\0';

	int s = encoded[len - 1] % 8;
	if (s == 0) {
		s = 5;
	}
	shift(s, ret, len - 1);
	return ret;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("encStr <name> <string>\n");
		return 1;
	}

	char *title = argv[1];
	char *origin = argv[2];
	int len = strlen(origin);

	char *enc = encStr(origin, len);
	char *dec = getStr(enc, len + 1);

	printf("// %s\n", dec);
	printf("static u8 %s[] = {", title);
	printHex(enc, len + 1);
	printf("};\n");
	free(enc);
	free(dec);

	return 0;
}
