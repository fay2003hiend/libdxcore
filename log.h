#ifndef _DXCORE_LOG_H_
#define _DXCORE_LOG_H_

#if 0
#include <stdio.h>
#include <android/log.h>

//#define DBG(fmt, args...) printf("%s %d "fmt, __FILE__, __LINE__, ##args)
#define DBG(fmt, args...) __android_log_print(ANDROID_LOG_INFO, "libdxcore.so", "%s %d "fmt, __FILE__, __LINE__, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, "libdxcore.so", "%s %d "fmt, __FILE__, __LINE__, ##args)
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, "libdxcore.so", "%s %d "fmt, __FILE__, __LINE__, ##args)
#define LOGV(fmt, args...) __android_log_print(ANDROID_LOG_VERBOSE, "libdxcore.so", "%s %d "fmt, __FILE__, __LINE__, ##args)

static void DBG_HEX(void *buf, int len) {
	char *ret = new char[len * 2 + 1];
	char *p = ret;
	for (int i = 0; i < len; i++) {
		sprintf(p, "%02X", *((unsigned char *) buf + i));
		p += 2;
	}
	*p = '\0';
	DBG("hex[%d]:%s\n", len, ret);
	delete[] ret;
}
#else
#define DBG(...)
#define DBG_HEX(...)
#define LOGE(...)
#define LOGI(...)
#define LOGV(...)
#endif

#endif
