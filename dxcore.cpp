#include "dxcore.h"
#include "log.h"
#include "unzip.h"
#include "com_dx_core_DxCore.h"

static u8 default_uuid[] = { 0xEC, 0x19, 0x2F, 0xC0, 0x69, 0x56, 0x1C, 0xFE,
		0xC9, 0x99, 0x9B, 0xD9, 0xAC, 0xA7, 0x1E, 0x03, 0x69, 0x61, 0xFA, 0xDC };

// android/os/Build$VERSION
static u8 s_cls_version[] = { 0xB0, 0xDC, 0x32, 0xE4, 0xB7, 0xD2, 0x32, 0x5E,
		0xB7, 0xE6, 0x97, 0x84, 0xBA, 0xD2, 0x36, 0xC8, 0x12, 0xAC, 0xA2, 0xA4,
		0xA9, 0x92, 0xA7, 0x9C, 0x4F, };

// SDK_INT
static u8 s_f_sdk_int[] = { 0x6A, 0x22, 0x69, 0xFA, 0x29, 0x72, 0x8A, 0x50, };

// android/content/Context
static u8 s_cls_context[] = { 0x2C, 0x73, 0x8C, 0x93, 0xED, 0x4B, 0x8C, 0x79,
		0x6C, 0x7B, 0xCD, 0xA3, 0xAC, 0x73, 0x8E, 0x79, 0x68, 0x7B, 0xCD, 0xA3,
		0xAC, 0xC3, 0x8E, 0x4D, };

// getApplicationInfo
static u8 s_m_getappInfo[] = { 0x9D, 0x59, 0xD1, 0x50, 0xC1, 0x1C, 0xB1, 0x5A,
		0x8D, 0x58, 0xD1, 0x5A, 0xBD, 0x9B, 0x25, 0x9B, 0x99, 0xDB, 0x02, };

// ()Landroid/content/pm/ApplicationInfo;
static u8 s_arg_getappInfo[] = { 0xA0, 0x4A, 0x31, 0x58, 0xB9, 0x19, 0xC9, 0xDB,
		0xA5, 0x19, 0xBC, 0xD8, 0xBD, 0x9B, 0xD1, 0x59, 0xB9, 0x1D, 0xBC, 0x1C,
		0xB5, 0xCB, 0x05, 0x1C, 0xC1, 0x1B, 0xA5, 0xD8, 0x85, 0x1D, 0xA5, 0xDB,
		0xB9, 0x52, 0xB9, 0x99, 0xBD, 0xCE, 0x22, };

// android/content/pm/ApplicationInfo
static u8 s_cls_appinfo[] = { 0x16, 0xE6, 0x46, 0x27, 0xF6, 0x96, 0x46, 0xF2,
		0x36, 0xF6, 0xE6, 0x47, 0x56, 0xE6, 0x47, 0xF2, 0x07, 0xD6, 0xF2, 0x14,
		0x07, 0x07, 0xC6, 0x96, 0x36, 0x16, 0x47, 0x96, 0xF6, 0xE6, 0x94, 0xE6,
		0x66, 0xF6, 0x54, };

// Ljava/lang/String;
static u8 s_type_string[] = { 0x31, 0x9A, 0x85, 0x9D, 0x85, 0xCB, 0xB1, 0x58,
		0xB9, 0xD9, 0xBC, 0xD4, 0xD1, 0x9C, 0xA5, 0x9B, 0x9D, 0xCE, 0x5A, };

// sourceDir
static u8 s_f_sourcedir[] = { 0xCD, 0xDB, 0xD5, 0x9C, 0x8D, 0x59, 0x11, 0x5A,
		0xC9, 0x42, };

// packageName
static u8 s_f_packageName[] = { 0x0E, 0x0B, 0x6C, 0x5B, 0x2C, 0x3B, 0xAC, 0x72,
		0x2C, 0x6B, 0xAC, 0x5D, };

// baseCodePath
static u8 s_f_baseCodePath[] = { 0x4C, 0x0B, 0x6E, 0x2B, 0x68, 0x7B, 0x8C, 0x2B,
		0x0A, 0x0B, 0x8E, 0x43, 0x15, };

// /libdxcore.so
static u8 s_libname[] = { 0xE5, 0x63, 0x2D, 0x13, 0x8C, 0xC3, 0x6C, 0x7B, 0x4E,
		0x2B, 0xC5, 0x9B, 0xED, 0x7D, };

// android/content/pm/PackageParser$Package
static u8 s_cls_package[] = { 0x0B, 0xCD, 0x23, 0x4E, 0x7B, 0x2D, 0x23, 0xE5,
		0x1B, 0xED, 0x73, 0x8E, 0x2B, 0xCD, 0xA3, 0xE5, 0x83, 0xAD, 0x79, 0x0A,
		0x0B, 0x6C, 0x5B, 0x2C, 0x3B, 0xAC, 0x82, 0x2C, 0x93, 0x6E, 0x2B, 0x4E,
		0x21, 0x0A, 0x0B, 0x6C, 0x5B, 0x2C, 0x3B, 0xAC, 0x23, };

// android/content/pm/PackageParser
static u8 s_cls_pkgparser[] = { 0x2C, 0x73, 0x8C, 0x93, 0xED, 0x4B, 0x8C, 0x79,
		0x6C, 0x7B, 0xCD, 0xA3, 0xAC, 0x73, 0x8E, 0x79, 0x0E, 0x6B, 0xE5, 0x82,
		0x2C, 0x1B, 0x6D, 0x0B, 0xEC, 0x2B, 0x0A, 0x0B, 0x4E, 0x9B, 0xAC, 0x93,
		0x5D, };

// <init>
static u8 s_cons_init[] = { 0x87, 0x4B, 0xCD, 0x4B, 0x8E, 0xF1, 0x18, };

// ()V
static u8 s_arg_void[] = { 0x14, 0x52, 0x2B, 0x57, };

// (Ljava/lang/String;)V
static u8 s_arg_initstr[] = { 0x05, 0x62, 0x4D, 0x0B, 0xCE, 0x0B, 0xE5, 0x63,
		0x2C, 0x73, 0xEC, 0x79, 0x6A, 0xA3, 0x4E, 0x4B, 0xCD, 0x3B, 0x67, 0x49,
		0xCA, 0x0D, };

// (Ljava/lang/String;Ljava/lang/String;)V
static u8 s_arg_initstrstr[] = { 0x14, 0x98, 0x35, 0xC2, 0x3B, 0xC2, 0x97, 0xD8,
		0xB0, 0xDC, 0xB3, 0x5E, 0xA9, 0xE8, 0x39, 0xD2, 0x37, 0xCE, 0x9D, 0x98,
		0x35, 0xC2, 0x3B, 0xC2, 0x97, 0xD8, 0xB0, 0xDC, 0xB3, 0x5E, 0xA9, 0xE8,
		0x39, 0xD2, 0x37, 0xCE, 0x9D, 0x52, 0x2B, 0x57, };

// collectCertificates
static u8 s_m_collectcert[] =
		{ 0x6C, 0x7B, 0x8D, 0x63, 0xAC, 0x1B, 0x8E, 0x1A, 0xAC, 0x93, 0x8E,
				0x4B, 0xCC, 0x4B, 0x6C, 0x0B, 0x8E, 0x2B, 0x6E, 0x58, };

// (Landroid/content/pm/PackageParser$Package;I)Z
static u8 s_arg_collectcert[] = { 0x0A, 0x31, 0x58, 0xB9, 0x19, 0xC9, 0xDB,
		0xA5, 0x19, 0xBC, 0xD8, 0xBD, 0x9B, 0xD1, 0x59, 0xB9, 0x1D, 0xBC, 0x1C,
		0xB5, 0xCB, 0x41, 0x58, 0x8D, 0xDA, 0x85, 0xD9, 0x95, 0x14, 0x85, 0x9C,
		0xCD, 0x59, 0xC9, 0x09, 0x41, 0x58, 0x8D, 0xDA, 0x85, 0xD9, 0x95, 0xCE,
		0x25, 0x4A, 0x69, 0x46, };

// (Landroid/content/pm/PackageParser$Package;I)V
static u8 s_arg_collectcert_void[] = { 0xA0, 0x13, 0x85, 0x9B, 0x91, 0x9C, 0xBD,
		0x5A, 0x91, 0xCB, 0x8D, 0xDB, 0xB9, 0x1D, 0x95, 0x9B, 0xD1, 0xCB, 0xC1,
		0x5B, 0xBC, 0x14, 0x85, 0xD8, 0xAD, 0x58, 0x9D, 0x59, 0x41, 0x58, 0xC9,
		0xDC, 0x95, 0x9C, 0x90, 0x14, 0x85, 0xD8, 0xAD, 0x58, 0x9D, 0x59, 0xEC,
		0x52, 0xA4, 0x95, 0x4A, };

// META-INF/
static u8 s_meta_inf[] = { 0x53, 0x15, 0x15, 0x05, 0x4B, 0x25, 0x93, 0x19, 0xCB,
		0x5E, };

// .RSA
static u8 s_cert_entry[] = { 0x8B, 0x49, 0xD4, 0x05, 0x6E, };

// /proc/self/maps
static u8 s_procmaps[] = { 0xBC, 0x1C, 0xC9, 0xDB, 0x8D, 0xCB, 0xCD, 0x59, 0xB1,
		0x99, 0xBC, 0x5B, 0x85, 0x1C, 0xCD, 0x32, };

#include "sha.c"

static void HexToStr(u8 *in, int len, char *dest) {
	char *p = dest;
	for (int i = 0; i < len; i++) {
		sprintf(p, "%02x", *(in + i));
		p += 2;
	}DBG("HexToStr <%s>\n", dest);
}

DxCore::DxCore() {
	setUuid(default_uuid);
}

int DxCore::sub_12FC(void *buf, int len, void *dest) {
	SHA_CTX ctx;
	SHA_init(&ctx);
	int left = len;
	int pos = 0;

	while (left > 0) {
		SHA_update(&ctx, (u8 *) buf + pos, left > 4096 ? 4096 : left);
		left -= 4096;
		pos += 4096;
	}
	const u8* ret = SHA_final(&ctx);
	memcpy(dest, ret, UUID_LEN);
	return 0;
}

int DxCore::sub_1370(const char *filepath, void *dest) {
	FILE *fp = fopen(filepath, "rb");
	if (fp == NULL) {
		return -1;
	}

	SHA_CTX ctx;
	u8 buf[4096];
	int n = -1;
	SHA_init(&ctx);
	while ((n = fread(buf, 1, 4096, fp)) > 0) {
		SHA_update(&ctx, buf, n);
	}
	fclose(fp);
	const u8* ret = SHA_final(&ctx);
	memcpy(dest, ret, UUID_LEN);
	return 0;
}

char *DxCore::sub_206C(u8 *encoded, int len) {
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

int DxCore::encode(void *in, void *out, int len) {
    /* confidential */
    /* confidential */
    /* confidential */
    /* confidential */
    /* confidential */
	return len + 2;
}

int DxCore::decode(void *in, void *out, int len) {
    /* confidential */
    /* confidential */
    /* confidential */
    /* confidential */
    /* confidential */
	return len - 2;
}

JNIEXPORT jint JNICALL Java_com_dx_core_DxCore_create(JNIEnv *, jobject) {
	DxCore *p = new DxCore();
	return (jint) p;
}

JNIEXPORT void JNICALL Java_com_dx_core_DxCore_destroy(JNIEnv *, jobject,
		jint ptr) {
	DxCore *p = (DxCore *) ptr;
	delete p;
}

static bool verifyZip(const char *path) {
	QList<char *> entryList;
	bool found_duplicate = false;
	bool master_key_bug = false;
	bool bad_zip = false;

	do {
		unzFile file = unzOpen(path);
		if (file == NULL) {
			DBG("unzOpen failed!\n");
			bad_zip = true;
			break;
		}

		unz_global_info global_info;
		unz_file_info file_info;

		char entry[512];
		int n = -1;

		int r = unzGetGlobalInfo(file, &global_info);
		for (int i = 0; i < global_info.number_entry; i++) {
			if ((r = unzGetCurrentFileInfo(file, &file_info, entry,
					sizeof(entry), NULL, 0, NULL, 0)) != UNZ_OK) {
				DBG("unzGetCurrentFileInfo error\n");
				bad_zip = true;
				break;
			}

			for (int j = 0; j < entryList.size(); j++) {
				if (strcmp(entryList[j], entry) == 0) {
					DBG("found duplicate entry '%s' !\n", entry);
					found_duplicate = true;
					break;
				}
			}

			if (!found_duplicate) {
				char *t = new char[sizeof(entry)];
				strcpy(t, entry);
				entryList.append(t);
			} else {
				break;
			}

			if (i < global_info.number_entry - 1) {
				if ((r = unzGoToNextFile(file)) != UNZ_OK) {
					DBG("unzGoToNextFile error\n");
					bad_zip = true;
					break;
				}
			}
		}
		master_key_bug = unzHasMasterKeyBug(file) != 0;
		unzClose(file);
	} while (0);

	while (!entryList.isEmpty()) {
		delete[] entryList.takeAtFirst();
	}

	bool ret = !bad_zip && !found_duplicate && !master_key_bug;
	DBG("verifyZip ret = %d\n", ret);
	return ret;
}

static bool verifyApk(JNIEnv *env, DxCore *dx, const char *pkg_name,
		const char *apk_path) {
	QList<jobject> objList;
	bool ret = false;
	do {
		char *str_cls_version = dx->sub_206C(s_cls_version,
				sizeof(s_cls_version));
		char *str_f_sdk_int = dx->sub_206C(s_f_sdk_int, sizeof(s_f_sdk_int));
		jclass cls_version = env->FindClass(str_cls_version);
		jfieldID f_sdkInt = env->GetStaticFieldID(cls_version, str_f_sdk_int,
				"I");
		int apiLevel = env->GetStaticIntField(cls_version, f_sdkInt);
		free(str_cls_version);
		free(str_f_sdk_int);
		DBG("apiLevel: %d\n", apiLevel);
		dx->markObj(objList, cls_version);

		char *str_cls_package = dx->sub_206C(s_cls_package,
				sizeof(s_cls_package));
		jclass cls_pkg = env->FindClass(str_cls_package);
		free(str_cls_package);
		if (cls_pkg == NULL) {
			DBG("cls_pkg not found!\n");
			break;
		}
		dx->markObj(objList, cls_pkg);

		char *str_cls_pkgparser = dx->sub_206C(s_cls_pkgparser,
				sizeof(s_cls_pkgparser));
		jclass cls_pkgparser = env->FindClass(str_cls_pkgparser);
		free(str_cls_pkgparser);
		if (cls_pkgparser == NULL) {
			DBG("cls_pkgparser not found!\n");
			break;
		}
		dx->markObj(objList, cls_pkgparser);

		char *str_init = dx->sub_206C(s_cons_init, sizeof(s_cons_init));
		char *str_arg_initstr = dx->sub_206C(s_arg_initstr,
				sizeof(s_arg_initstr));
		char *str_arg_void = dx->sub_206C(s_arg_void, sizeof(s_arg_void));

		jmethodID m_pkgcons = env->GetMethodID(cls_pkg, str_init, str_arg_initstr);
		if (m_pkgcons == NULL) {
			DBG("m_pkgcons not found!\n");
			free(str_init);
			free(str_arg_initstr);
			free(str_arg_void);
			break;
		}

		jmethodID m_pkgparsercons;
		if (apiLevel <= 20) {
			m_pkgparsercons = env->GetMethodID(cls_pkgparser, str_init,
					str_arg_initstr);
		} else {
			m_pkgparsercons = env->GetMethodID(cls_pkgparser, str_init,
					str_arg_void);
		}
		free(str_init);
		free(str_arg_initstr);
		free(str_arg_void);

		if (m_pkgparsercons == NULL) {
			DBG("m_pkgparsercons not found!\n");
			break;
		}

		char *str_m_collectcert = dx->sub_206C(s_m_collectcert,
				sizeof(s_m_collectcert));
		char *str_arg_collectcert = dx->sub_206C(s_arg_collectcert,
				sizeof(s_arg_collectcert));
		char *str_arg_collectcert_void = dx->sub_206C(s_arg_collectcert_void,
				sizeof(s_arg_collectcert_void));

		jmethodID m_collect;
		if (apiLevel <= 20) {
			m_collect = env->GetMethodID(cls_pkgparser, str_m_collectcert,
					str_arg_collectcert);
		} else {
			m_collect = env->GetMethodID(cls_pkgparser, str_m_collectcert,
					str_arg_collectcert_void);
		}

		free(str_m_collectcert);
		free(str_arg_collectcert);
		free(str_arg_collectcert_void);

		if (m_collect == NULL) {
			DBG("m_collect not found!\n");
			break;
		}

		jstring js_pkgname = env->NewStringUTF(pkg_name);
		jstring js_apkpath = env->NewStringUTF(apk_path);
		jobject obj_pkg = env->NewObject(cls_pkg, m_pkgcons, js_pkgname);

		jobject obj_pkgparser;
		if (apiLevel <= 20) {
			obj_pkgparser = env->NewObject(cls_pkgparser, m_pkgparsercons,
					js_apkpath);
		} else {
			obj_pkgparser = env->NewObject(cls_pkgparser, m_pkgparsercons);
		}
		dx->markObj(objList, js_pkgname);
		dx->markObj(objList, js_apkpath);
		dx->markObj(objList, obj_pkg);
		dx->markObj(objList, obj_pkgparser);

		if (apiLevel <= 20) {
			ret = env->CallBooleanMethod(obj_pkgparser, m_collect, obj_pkg, 0);
		} else {
			char *str_f_baseCodePath = dx->sub_206C(s_f_baseCodePath,
					sizeof(s_f_baseCodePath));
			char *str_type_string = dx->sub_206C(s_type_string,
					sizeof(s_type_string));

			jfieldID f_baseCodePath = env->GetFieldID(cls_pkg,
					str_f_baseCodePath, str_type_string);
			free(str_f_baseCodePath);
			free(str_type_string);

			if (f_baseCodePath != NULL) {
				DBG("set baseCodePath '%s'\n", apk_path);
				env->SetObjectField(obj_pkg, f_baseCodePath, js_apkpath);
			}

			env->CallVoidMethod(obj_pkgparser, m_collect, obj_pkg, 0);
			ret = !env->ExceptionCheck();
		}
	} while (0);
	dx->freeObj(objList, env);
	DBG("verifyApk ret = %d\n", ret);
	return ret;
}

static bool strStartsWith(char *str, char *prefix) {
	char *p = strstr(str, prefix);
	return p == str;
}

static bool strEndsWidth(char *str, char *suffix) {
	char *p = strstr(str, suffix);
	if (p == NULL) {
		return false;
	}

	return p[strlen(suffix)] == '\0';
}

static int charCountInStr(char *str, char c) {
	int i = strlen(str);
	int count = 0;
	while (i-- > 0) {
		if (str[i] == c) {
			count++;
		}
	}
	DBG("char <%c> in <%s> = %d\n", c, str, count);
	return count;
}

static bool parseRsa(DxCore *dx, const char *path, void *dest) {
	bool ret = false;
	do {
		unzFile file = unzOpen(path);
		if (file == NULL) {
			DBG("unzOpen failed!\n");
			break;
		}

		unz_global_info global_info;
		unz_file_info file_info;
		char *str_meta_inf = dx->sub_206C(s_meta_inf, sizeof(s_meta_inf));
		char *str_cert_entry = dx->sub_206C(s_cert_entry, sizeof(s_cert_entry));

		char entry[512];
		char rsa_entry[512];
		int n = -1;

		int r = unzGetGlobalInfo(file, &global_info);
		for (int i = 0; i < global_info.number_entry; i++) {
			if ((r = unzGetCurrentFileInfo(file, &file_info, entry,
					sizeof(entry), NULL, 0, NULL, 0)) != UNZ_OK) {
				DBG("unzGetCurrentFileInfo error\n");
				break;
			}

			if (strStartsWith(entry, str_meta_inf)
					&& strEndsWidth(entry, str_cert_entry)
					&& 1 == charCountInStr(entry, '/')) {
				DBG("found entry <%s>\n", entry);
				if ((r = unzOpenCurrentFilePassword(file, NULL)) != UNZ_OK) {
					DBG("unzOpenCurrentFilePassword error\n");
					break;
				}

				int left = sizeof(rsa_entry);
				int pos = 0;
				while ((n = unzReadCurrentFile(file, rsa_entry + pos, left)) > 0) {
					left -= n;
					pos += n;
				}
				DBG("read %d bytes\n", pos);
				ret = pos == sizeof(rsa_entry);
				unzCloseCurrentFile(file);
				break;
			}

			if (i < global_info.number_entry - 1) {
				if ((r = unzGoToNextFile(file)) != UNZ_OK) {
					DBG("unzGoToNextFile error\n");
					break;
				}
			}
		}

		free(str_meta_inf);
		free(str_cert_entry);
		unzClose(file);

		if (ret == true) {
			dx->sub_12FC(rsa_entry, sizeof(rsa_entry), dest);
		}
	} while (0);
	DBG("parseRsa ret = %d\n", ret);
	return ret;
}

static int getLibPath(DxCore *dx, const char *libname, char *dest, int len) {
	int liblen = strlen(libname);
	char line[512];
	*dest = '\0';
	char *str_procmaps = dx->sub_206C(s_procmaps, sizeof(s_procmaps));
	FILE *fp = fopen(str_procmaps, "rb");
	free(str_procmaps);

	if (fp != NULL) {
		while (fgets(line, 512, fp) != NULL) {
			char *p = strstr(line, libname);
			if (p != NULL) {
				line[strlen(line) - 1] = '\0';
				if (strlen(p) == liblen) {
					while (*--p != ' ')
						;
					strncpy(dest, ++p, len);
					break;
				}
			}
		}
		fclose(fp);
		return strlen(dest);
	}
	return 0;
}

JNIEXPORT jboolean JNICALL Java_com_dx_core_DxCore_setContext(JNIEnv *env,
		jobject, jint ptr, jobject context) {
	DxCore *dx = (DxCore *) ptr;
	QList<jobject> objList;
	bool ret = false;
	do {
		char *str_cls_context = dx->sub_206C(s_cls_context,
				sizeof(s_cls_context));
		jclass cls_context = env->FindClass(str_cls_context);
		free(str_cls_context);

		if (cls_context == NULL) {
			DBG("cls_context not found!\n");
			break;
		}
		dx->markObj(objList, cls_context);

		char *str_m_getappInfo = dx->sub_206C(s_m_getappInfo,
				sizeof(s_m_getappInfo));
		char *str_arg_getappInfo = dx->sub_206C(s_arg_getappInfo,
				sizeof(s_arg_getappInfo));

		jmethodID m_getAppInfo = env->GetMethodID(cls_context, str_m_getappInfo,
				str_arg_getappInfo);
		free(str_m_getappInfo);
		free(str_arg_getappInfo);

		if (m_getAppInfo == NULL) {
			DBG("m_getAppInfo not found!\n");
			break;
		}

		char *str_cls_appinfo = dx->sub_206C(s_cls_appinfo,
				sizeof(s_cls_appinfo));
		jclass cls_appinfo = env->FindClass(str_cls_appinfo);
		free(str_cls_appinfo);

		if (cls_appinfo == NULL) {
			DBG("cls_appinfo class not found!\n");
			break;
		}
		dx->markObj(objList, cls_appinfo);

		char *str_f_sourcedir = dx->sub_206C(s_f_sourcedir,
				sizeof(s_f_sourcedir));
		char *str_f_pkgname = dx->sub_206C(s_f_packageName,
				sizeof(s_f_packageName));
		char *str_type_string = dx->sub_206C(s_type_string,
				sizeof(s_type_string));
		jfieldID f_sourcedir = env->GetFieldID(cls_appinfo, str_f_sourcedir,
				str_type_string);
		jfieldID f_packageName = env->GetFieldID(cls_appinfo, str_f_pkgname,
				str_type_string);

		free(str_f_sourcedir);
		free(str_f_pkgname);
		free(str_type_string);

		if (f_sourcedir == NULL) {
			DBG("f_sourcedir not found!\n");
			break;
		}

		if (f_packageName == NULL) {
			DBG("f_packageName not found!\n");
			break;
		}

		u8 sha_apk[UUID_LEN] = { 0 };
		u8 sha_lib[UUID_LEN] = { 0 };
		u8 ret_uuid[UUID_LEN];

		// ==== sha of apk ====
		jobject appinfo = static_cast<jobject>(env->CallObjectMethod(context,
				m_getAppInfo));
		jstring apkpath = static_cast<jstring>(env->GetObjectField(appinfo,
				f_sourcedir));
		jstring pkgname = static_cast<jstring>(env->GetObjectField(appinfo,
				f_packageName));
		dx->markObj(objList, appinfo);
		dx->markObj(objList, apkpath);
		dx->markObj(objList, pkgname);
		const char *apk_path = env->GetStringUTFChars(apkpath, NULL);
		const char *pkg_name = env->GetStringUTFChars(pkgname, NULL);
		DBG("apk path:<%s>, pkg name:<%s>\n", apk_path, pkg_name);

		if (!verifyZip(apk_path)) {
			DBG("apk verify fail!\n");
			env->ReleaseStringUTFChars(apkpath, apk_path);
			break;
		}

		if (!verifyApk(env, dx, pkg_name, apk_path)) {
			DBG("apk verify fail!\n");
			env->ReleaseStringUTFChars(apkpath, apk_path);
			break;
		}

		if (!parseRsa(dx, apk_path, sha_apk)) {
			DBG("apk rsa fail!\n");
			env->ReleaseStringUTFChars(apkpath, apk_path);
			break;
		}

		DBG("sha_apk:\n");
		DBG_HEX((void *) sha_apk, UUID_LEN);
		env->ReleaseStringUTFChars(apkpath, apk_path);

		// ==== sha of lib ====
		char *libname = dx->sub_206C(s_libname, sizeof(s_libname));
		char lib_path[256];
		getLibPath(dx, libname, lib_path, sizeof(lib_path));
		DBG("lib path:<%s>\n", lib_path);
		dx->sub_1370(lib_path, sha_lib);
		free(libname);

		DBG("sha_lib:\n");
		DBG_HEX((void *) sha_lib, UUID_LEN);

		// ==== mix lib & apk ====
		for (int i = 0; i < UUID_LEN; i++) {
			ret_uuid[i] = sha_apk[i] ^ sha_lib[i];
			//DBG("%02x ^ %02x = %02x\n", sha_apk[i], sha_lib[i], ret_uuid[i]);
		}

		DBG("ret uuid:\n");
		DBG_HEX(ret_uuid, UUID_LEN);
		dx->setUuid(ret_uuid);
		ret = true;
	} while (0);
	dx->freeObj(objList, env);
	DBG("setContext ret: %d\n", ret);

	return ret;
}

JNIEXPORT jstring JNICALL Java_com_dx_core_DxCore_getFingerPrint(JNIEnv *env,
		jobject, jint ptr, jbyteArray data, jint pos, jint len) {
	DxCore *dx = (DxCore *) ptr;

	u8 *in = (u8 *) (env->GetByteArrayElements(data, NULL));
	u8 sha[UUID_LEN];
	dx->sub_12FC(in, len, sha);
	env->ReleaseByteArrayElements(data, (jbyte *) in, 0);

	u8 result[UUID_LEN + 2];
	dx->encode(sha, result, UUID_LEN);

	char str[sizeof(result) * 2 + 1];
	HexToStr(result, sizeof(result), str);
	return env->NewStringUTF(str);
}

JNIEXPORT jstring JNICALL Java_com_dx_core_DxCore_getDllVersion(JNIEnv *env,
		jobject) {
	return env->NewStringUTF(DLL_VERSION);
}

JNIEXPORT jint JNICALL Java_com_dx_core_DxCore_encode(JNIEnv *env, jobject,
		jint ptr, jbyteArray in, jbyteArray out, jint pos, jint len) {
	DxCore *dx = (DxCore *) ptr;
	u8 *inData = (u8 *) (env->GetByteArrayElements(in, NULL));
	DBG("source:\n");
	DBG_HEX(inData, len);

	u8 *outData = (u8 *) (env->GetByteArrayElements(out, NULL));
	int ret = dx->encode(inData + pos, outData, len);
	DBG("encoded:\n");
	DBG_HEX(outData, len + 2);

	env->ReleaseByteArrayElements(in, (jbyte *) inData, 0);
	env->ReleaseByteArrayElements(out, (jbyte *) outData, 0);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_dx_core_DxCore_decode(JNIEnv *env, jobject,
		jint ptr, jbyteArray in, jbyteArray out, jint pos, jint len) {
	DxCore *dx = (DxCore *) ptr;
	u8 *inData = (u8 *) (env->GetByteArrayElements(in, NULL));
	DBG("source:\n");
	DBG_HEX(inData, len);

	u8 *outData = (u8 *) (env->GetByteArrayElements(out, NULL));
	int ret = dx->decode(inData + pos, outData, len);
	DBG("decoded:\n");
	DBG_HEX(outData, len - 2);

	env->ReleaseByteArrayElements(in, (jbyte *) inData, 0);
	env->ReleaseByteArrayElements(out, (jbyte *) outData, 0);
	return ret;
}
