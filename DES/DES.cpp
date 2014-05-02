// DES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

const size_t L = 8;
const uint N   = 16;

// Lookup Tables for Permutation/Compression/Expansion/Substitution
const uchar PC1[] = 
{
	57,  49,   41,  33,   25,   17,   9,
	 1,  58,   50,  42,   34,   26,  18,
	10,   2,   59,  51,   43,   35,  27,
	19,  11,    3,  60,   52,   44,  36,
	63,  55,   47,  39,   31,   23,  15,
	 7,  62,   54,  46,   38,   30,  22,
	14,   6,   61,  53,   45,   37,  29,
	21,  13,    5,  28,   20,   12,   4,
};

const uchar PC2[] = 
{
	14,   17,  11,   24,    1,   5,
	 3,   28,  15,    6,   21,  10,
	23,   19,  12,    4,   26,   8,
	16,    7,  27,   20,   13,   2,
	41,   52,  31,   37,   47,  55,
	30,   40,  51,   45,   33,  48,
	44,   49,  39,   56,   34,  53,
	46,   42,  50,   36,   29,  32,
};

const uchar IPFWD[] = 
{
	58,   50,  42,   34,   26,  18,   10,   2,
	60,   52,  44,   36,   28,  20,   12,   4,
	62,   54,  46,   38,   30,  22,   14,   6,
	64,   56,  48,   40,   32,  24,   16,   8,
	57,   49,  41,   33,   25,  17,    9,   1,
	59,   51,  43,   35,   27,  19,   11,   3,
	61,   53,  45,   37,   29,  21,   13,   5,
	63,   55,  47,   39,   31,  23,   15,   7,
};

const uchar IPREV[] = 
{
	40,    8,  48,   16,   56,  24,   64,  32,
	39,    7,  47,   15,   55,  23,   63,  31,
	38,    6,  46,   14,   54,  22,   62,  30,
	37,    5,  45,   13,   53,  21,   61,  29,
	36,    4,  44,   12,   52,  20,   60,  28,
	35,    3,  43,   11,   51,  19,   59,  27,
	34,    2,  42,   10,   50,  18,   58,  26,
	33,    1,  41,    9,   49,  17,   57,  25,
};

const uchar E[] = 
{
	32,    1,   2,    3,    4,   5,
	4,    5,   6,    7,    8,   9,
	8,    9,  10,   11,   12,  13,
	12,   13,  14,   15,   16,  17,
	16,   17,  18,   19,   20,  21,
	20,   21,  22,   23,   24,  25,
	24,   25,  26,   27,   28,  29,
	28,   29,  30,   31,   32,   1,
};

const uchar S[][64] = 
{
	{
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
         0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
         4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    },
	{
        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
         3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
         0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    },
	{
        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
         1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
    },
	{
         7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
         3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
    },
	{
         2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
         4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    },
	{
        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
         9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
         4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
    },
	{
         4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
         1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
         6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
    },
	{
        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
         1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
         7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
         2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11,
    }
};

const uchar P[] = 
{
	16,  7, 20, 21,
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25,
};


const size_t readblock(FILE*, uchar[]);
const size_t writeblock(FILE*, uchar[], const uint);
const size_t writeblock(FILE*, uint64, const uint);
const size_t paddblock(uchar[], const size_t);
void scrubblock(uchar[], const size_t);
void printblock(const string, uchar[]);
void printblock(const string, uint64);

void genkeys(/*const uint rounds, */const uint64 key, uint64 subkeys[]);
uint64 docrypt(const uint64, uint64[], const uint);
uint64 docrypt_round(const uint64, const uint64);

uint64 flength(FILE*);
uint32 olength(FILE*, uint64[], uint);

uint64 bytes8toui64(const uchar[]);
void ui64tobytes8(const uint64, uchar[]);
bool is_decrypt(const char[]);
bool is_encrypt(const char[]);
void verify_handle(FILE*, const string, bool);
void wait_for_return();
void print_usage_exit(const char[]);
void parse_args_and_exit(CryptoArguments*, uint, char*[]);
uint64 cstrtoi64(uint, const char*);
template<typename T> T rol(const T, const uint, const uint, const T);


int main(uint argc, char *argv[])
{	
	uint64 PK[N];
	CryptoArguments args;
	FILE* src;
	FILE* snc;
	size_t r;
	size_t w;
	size_t l;
	size_t i = 0;
	size_t o = 0;
	uint64 x = 0;
	uchar blkv[L+1] = {0}; // (+1 to trick debugger into cstring)
	int m = INT32_MAX;

	srand((uint)time(NULL));
	parse_args_and_exit(&args, argc, argv);
	genkeys(/*N, */args.key, PK);

	if (args.mode == ENCRYPT)
	{
		src = fopen(args.ifname.c_str(), "rb");
		verify_handle(src, args.ifname, true);
		snc = fopen(args.ofname.c_str(), "wb");
		verify_handle(snc, args.ofname, false);
		x = flength(src);
		x = docrypt(x, PK, N);
		w = writeblock(snc, x, L);
		o += w;
	}
	else // decrypt
	{
		src = fopen(args.ifname.c_str(), "rb");
		verify_handle(src, args.ifname, true);
		snc = fopen(args.ofname.c_str(), "wb");
		verify_handle(snc, args.ofname, false);
		reverse(begin(PK), end(PK));
		m = olength(src, PK, N);
		i += L;
	}

	while ((r = readblock(src, blkv)) > 0)
	{
		m -= r;
		i += r;
		l = paddblock(blkv, r);
		x = bytes8toui64(blkv);
		x = docrypt(x, PK, N);
		if (m < 0)
			l = L + m;
		w = writeblock(snc, x, l);
		o += w;
	}

	// MS lies... doesn't do what it's supposed to do
	/*if ((args.mode == DECRYPT) && (o > m))
	{
		int fd = _fileno(snc);
		if (_chsize(fd, m) != 0)
		{
			cerr << "Whoops, failed to truncate the file to " << m << " bytes." << endl;
		}
	}*/

	cerr << "SRC:\t" << i << " bytes" << endl;
	cerr << "SNC:\t" << o << " bytes" << endl;

	fclose(src);
	fclose(snc);
	exit(EXIT_SUCCESS);
}

inline uint64 pce(const uint64 x, const uchar t[], uint in, uint out)
{
	uint shift;
	uint64 reslt = 0;
	uint64 curnt = 0;
	uint64 cmask = 0;
	const uint64 mask = 1;
		
	for(uint i = 0; i < out; ++i)
	{
		reslt <<= 1;
		shift = t[i];
		cmask = (mask << (in - shift));
		curnt = cmask & x;
		if (curnt != 0)
		{
			cmask = (mask << (in - i - 1));
			reslt |= mask;
		}
	}
	return reslt;
}

inline uint64 genkeys_round(uint64 *C, uint64 *D, const uint rotations)
{
	uint64 Cn, Dn, Kn;
	Cn = rol<uint64>(*C, rotations, 28, BITMASK28);
	Dn = rol<uint64>(*D, rotations, 28, BITMASK28);
	uint64 CD = (Cn << 28) | Dn;
	Kn = pce(CD, PC2, 56, 48);
	*C = Cn;
	*D = Dn;
	return Kn;
}

void genkeys(/*const uint rounds, */const uint64 K, uint64 subkeys[])
{
	uint64 Kp = 0, Kn = 0, CD = 0;
	uint64 C  = 0, D  = 0;
	Kp = pce(K, PC1, 64, 56);
	C = Kp >> 28;
	D = Kp & BITMASK28;

	//subkeys[ 0] = Kp;
	subkeys[ 0] = genkeys_round(&C, &D, 1);
	subkeys[ 1] = genkeys_round(&C, &D, 1);
	subkeys[ 2] = genkeys_round(&C, &D, 2);
	subkeys[ 3] = genkeys_round(&C, &D, 2);
	subkeys[ 4] = genkeys_round(&C, &D, 2);
	subkeys[ 5] = genkeys_round(&C, &D, 2);
	subkeys[ 6] = genkeys_round(&C, &D, 2);
	subkeys[ 7] = genkeys_round(&C, &D, 2);
	subkeys[ 8] = genkeys_round(&C, &D, 1);
	subkeys[ 9] = genkeys_round(&C, &D, 2);
	subkeys[10] = genkeys_round(&C, &D, 2);
	subkeys[11] = genkeys_round(&C, &D, 2);
	subkeys[12] = genkeys_round(&C, &D, 2);
	subkeys[13] = genkeys_round(&C, &D, 2);
	subkeys[14] = genkeys_round(&C, &D, 2);
	subkeys[15] = genkeys_round(&C, &D, 1);
	/*for (uint i = 0; i < rounds; ++i)
	{
		switch(i)
		{
			case 0:
			case 1:
			case 9:
			case 15:
				C = rol<uint64>(C, 1, 28, BITMASK28);
				D = rol<uint64>(D, 1, 28, BITMASK28);
				break;
			default:
				C = rol<uint64>(C, 2, 28, BITMASK28);
				D = rol<uint64>(D, 2, 28, BITMASK28);
		}
		CD = (C << 28) | D;
		Kn = applypc(CD, PC2, 56, 48);
		subkeys[i] = Kn;
	}*/
}

uint32 f(const uint32 x, uint64 k)
{
	uint32 r;
	uint64 y, z;
	uint shft, rwh, rwl, row, col, off;
	uint32 w = 0;

	y = pce(x, E, 32, 48);  // E-Expansion
	z = y ^ k;				// X-OR with roundkey
	for (uint i = 0; i < 8; ++i)
	{
		w <<= 4;
		shft = 42 - (6 * i);
		rwl = (z >>  shft     ) & BITMASK1S;
		rwh = (z >> (shft + 4)) & BITMASK2S;
		col = (z >> (shft + 1)) & BITMASK04;
		row = rwh | rwl;
		off = (row * 16) + col;
		w |= S[i][off];
	}
	r = (uint32)pce(w, P, 32, 32);
	return r;
}

uint64 docrypt(const uint64 x, uint64 subkeys[], const uint rounds)
{
	//uint64 junk = 0x0123456789ABCDEF;
	uint64 IP, y, z, Rf, Lf;
	uint32 Lp, Rp, Ln, Rn;
	IP = pce(x, IPFWD, 64, 64);
	Lp = (uint32)(IP >> 32);
	Rp = (uint32)IP;

	for(uint i = 0; i < rounds; ++i)
	{
		Ln = Rp;
		Rn = Lp ^ f(Rp, subkeys[i]);
		Lp = Ln;
		Rp = Rn;
	}
	Rf = Rn;
	Lf = Ln;
	y = (Rf << 32) | (Lf);
	z = pce(y, IPREV, 64, 64);
	return z;
}

uint64 docrypt_round(const uint64 x, const uint64 key)
{
	return x ^ key; //TODO
}

const size_t readblock(FILE* fp, uchar arr[])
{
	size_t a, r;
	a = fread(arr, 1, L, fp);
	r = a;
	return r;
}

const size_t writeblock(FILE* fp, uchar arr[], const uint length)
{
	return fwrite(arr, 1, length, fp);
}

const size_t writeblock(FILE* fp, uint64 x, const uint length)
{
	uchar arr[L+1] = {0}; // TODO: Debugggg (cstring)
	ui64tobytes8(x, arr);
	return writeblock(fp, arr, length);
}

const size_t paddblock(uchar arr[], const size_t x)
{
	size_t c = x;
	while (c < L)
	{
		arr[c] = (char)(rand() % RAND_PADD);
		c++;
	}
	return c;
}

void scrubblock(uchar arr[], const size_t length)
{
	memset(arr, 0xFF, length);
}

void printblock(const string prefix, uchar arr[])
{
	size_t i = 0;
	cerr << prefix << ":\t'";
	for (i = 0; i < L; i++)
	{
		printf("%02X", arr[i]);
	}
	cerr << "'" << endl;
}

void printblock(const string prefix, uint64 x)
{
	printf("%s:\t'%016I64X'\n", prefix.c_str(), x);
}

uint64 cstrtoi64(uint length, const char *arg)
{
	// what a massive pain in the 
    static const char* const lookup = "0123456789ABCDEF";
    string result;
    result.reserve(length * 2);
    for (unsigned int i = 0; i < length; ++i)
    {
        uchar c = arg[i];
        result.push_back(lookup[c >> 4]); // nibble
        result.push_back(lookup[c & 0xF]);
    }
	return _strtoui64(result.c_str(), NULL, 16);
}

uint64 bytes8toui64(const uchar arr[])
{
	uint64 r = 0;
	r  = ((uint64)arr[0] << 56)
	    | ((uint64)arr[1] << 48)
		| ((uint64)arr[2] << 40)
		| ((uint64)arr[3] << 32)
		| ((uint64)arr[4] << 24)
		| ((uint64)arr[5] << 16)
		| ((uint64)arr[6] <<  8)
		| (uint64)arr[7];
	return r;
}

void ui64tobytes8(const uint64 x, uchar arr[])
{
	arr[7] = (uchar)x;
	arr[6] = (uchar)(x >>  8);
	arr[5] = (uchar)(x >> 16);
	arr[4] = (uchar)(x >> 24);
	arr[3] = (uchar)(x >> 32);
	arr[2] = (uchar)(x >> 40);
	arr[1] = (uchar)(x >> 48);
	arr[0] = (uchar)(x >> 56);
}

uint64 flength(FILE* fp)
{
	uint64 length;
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return length;
}

uint32 olength(FILE* fp, uint64 keys[], uint rounds)
{
	uchar blkv[L];
	size_t a = readblock(fp, blkv);
	if (a == 8)
	{
		uint64 blk = bytes8toui64(blkv);
		blk = docrypt(blk, keys, rounds);
		blk &= 0xFFFFFFFF;
		return (uint32)blk;
	} else
	{
		cerr << "Failed to read first 8 bytes of file. Only read " << a << " bytes." << endl;
		exit(EXIT_BAD_INPUT);
	}
}

void verify_handle(FILE* fp, const string fname, const bool is_input)
{
	if (fp == NULL)
	{
		cerr << "Failed to open " << ((is_input)? "source" : "destination") << " file: '" << fname << "'." << endl;
		wait_for_return();
		exit((is_input)? EXIT_BAD_INPUT : EXIT_BAD_OUTPUT);
	}
}

void wait_for_return()
{
	string junk;
	cout << "Press enter to continue...";
	getline(cin, junk);
}

void parse_args_and_exit(CryptoArguments *result, uint argc, char *argv[])
{

    string cmd = argv[0];
	std::tr2::sys::path cmdp(cmd);
	result->cmd = cmdp.basename();
    if (argc == 5)
    {
        string mode   = argv[1];
        string keystr = argv[2];
        string ifname = argv[3];
        string ofname = argv[4];

        keystr.erase(remove(keystr.begin(), keystr.end(), '\''), keystr.end());

        // mode
        if (mode == "-e" || mode == "-E")       
			result->mode = ENCRYPT;
        else if (mode == "-d" || mode == "-D")
			result->mode = DECRYPT;
        else
        {
            cerr << "Invalid mode '" << mode << "'." << endl;
            goto BADARG;
        }

        // key
        if (keystr.length() == 8)
			result->key = cstrtoi64(keystr.length(), keystr.c_str());
        else if (keystr.length() == 16)
			result->key = _strtoui64(keystr.c_str(), NULL, 16);
        else
        {
                cerr << "Invalid key '" << keystr << "'." << endl;
                goto BADARG;
        }

        // input_file
        if (!ifname.empty())
			result->ifname = ifname;
        else
        {
            cerr << "Invalid input file '" << ifname << "'.'" << endl;
            goto BADARG;
        }

        // output_file
        if (!ofname.empty())
			result->ofname = ofname;
        else
        {
            cerr << "Invalid output file '" << ofname << "'.'" << endl;
            goto BADARG;
        }

        return;
    }
BADARG:
    cerr << "Usage:" << endl;
    cerr << cmd << " -[e|d] [012345678ABCDEF|'ABCDEFGH'] [input_filename] [output_filename]" << endl;
    exit(EXIT_BAD_USAGE);
}

template<typename T>
T rol(const T value, const uint shift, const uint length, const T mask) {
	/*T left = value << shift;
	auto a = length;
	auto b = a - shift;
	T rght = value >> b;*/
	return ((value << shift) | (value >> (length - shift))) & mask;
    //return  (left | rght) & mask;
}