#pragma once
#ifdef	STATIC_TRANSLATE_DLL
#define TRANSLATE_API
#else
#define TRANSLATE_API __declspec(dllimport)
#endif
#include <iostream>

#define BUF_LEN_HUGE	1024
#define BUF_LEN_BIG		256
#define BUF_LEN_SMALL	128

#ifdef __cplusplus
extern "C" {
#endif
	//@ 初始化资源
	void TRANSLATE_API InitDllTranslate();
	//@ 释放资源
	void TRANSLATE_API ReleaseDllTranslate();
	//@inputString 输入字符串
	//@outputString 输出字符串
	//bool TRANSLATE_API Translate(const char* inputString, const char* outputString);
	bool TRANSLATE_API Translate(const WCHAR* inputString, WCHAR* outputString);


#ifdef __cplusplus
}
#endif