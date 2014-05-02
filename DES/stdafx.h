// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include "targetver.h"

#include <io.h>
#include <tchar.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <iterator>


#define EXIT_BAD_USAGE 127
#define EXIT_BAD_INPUT 1
#define EXIT_BAD_OUTPUT 2
#define RAND_PADD 127
#define BITMASK1S  0x1;
#define BITMASK2S  0x2;
#define BITMASK04  0xF;
#define BITMASK28 0xFFFFFFF
#define BITMASK32 0xFFFFFFFFF
#define BITMASK48 0xFFFFFFFFFFFF
#define BITMASK56 0xFFFFFFFFFFFFFF


const int CRYPTO_MODE_ENC = 1;
const int CRYPTO_MODE_DEC = -1;
enum CRYPTO_MODE
{
	ENCRYPT = CRYPTO_MODE_ENC,
	DECRYPT = CRYPTO_MODE_DEC
};

typedef unsigned char uchar;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int8 uint8;
typedef unsigned int uint;

typedef struct CryptoArgs
{
	std::string cmd;
	enum CRYPTO_MODE mode;
	std::string ifname;
	std::string ofname;
	uint64 key;
} CryptoArguments;

