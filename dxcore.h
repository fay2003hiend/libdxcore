#ifndef _DXCORE_H_
#define _DXCORE_H_

#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "qlist.h"

#ifdef __arm__
#define CPU_TYPE "_arm"
#else
#define CPU_TYPE "_x86"
#endif
#define DLL_VERSION "1.5" CPU_TYPE

#define UUID_LEN 20

typedef unsigned char u8;

class DxCore {
	u8 uuid[UUID_LEN];

	inline u8 crc(void *buf, int len) {
		u8 ret = *((u8 *) buf);
		for (int i = 1; i < len; i++) {
			ret ^= *((u8 *) buf + i);
		}
		return ret;
	}

	inline void shift(int shift, void *buf, int len) {
		u8 a, b;
		int s = shift;
		for (int i = 0; i < len; i++) {
			u8 *p = (u8 *) buf + i;
			u8 q = *p;
			a = (q << (8 - s)) & 0xFF;
			b = (q >> s) & 0xFF;
			*p = a | b;
			s = 8 - s;
		}
	}

	inline void shift(void *buf, int len) {
		int s = crc(uuid, UUID_LEN) % 8;
		if (s == 0) {
			s = 3;
		}
		shift(s, buf, len);
	}

	inline void shiftBack(void *buf, int len) {
		int s = 8 - crc(uuid, UUID_LEN) % 8;
		if (s == 8) {
			s = 5;
		}
		shift(s, buf, len);
	}
public:
	DxCore();

	inline void markObj(QList<jobject>& objList, jobject obj) {
		objList.append(obj);
	}

	inline void freeObj(QList<jobject>& objList, JNIEnv *env) {
		while (!objList.isEmpty()) {
			env->DeleteLocalRef(objList.takeAtFirst());
		}
	}

	int sub_12FC(void *buf, int len, void *dest);

	int sub_1370(const char *filepath, void *dest);

	inline void setUuid(const void *new_uuid) {
		memcpy(uuid, new_uuid, UUID_LEN);
	}

	char *sub_206C(u8 *encoded, int len);

	int encode(void *in, void *out, int len);
	int decode(void *in, void *out, int len);
};

#endif
