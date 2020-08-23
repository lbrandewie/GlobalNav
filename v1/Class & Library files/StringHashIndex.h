//
// stringhashindex.h
//
// interface for the StringHashIndex class
//
// Copyright 2020 L.P. "Lars" Brandewie
//

#ifndef STRINGHASHINDEX_H

#define STRINGHASHINDEX_H

#define uint unsigned int

#include "BlockStringAlloc.h"

extern  BlockStringAlloc bsa;

#define BLOCKSIZE_SHI 1024
#define MAXBLOCKS_SHI 32


struct HashEl
{
    char *key;
    char *value;
};

class StringHashIndex
{
    public:
        StringHashIndex();
        StringHashIndex(uint bsize, short mblocks);
        ~StringHashIndex();
        void   deletekey(char *);
		uint   getcount();
		char   *getkey(uint);
        char   *getval(char *);
        char   *getval(uint);
		void   setval(char *key, char *value);

    private:
        void   getnewblock();
        void   insertkey(uint, char *, char *);
        uint   search(char *);

        uint   blocksize;
        HashEl *data;
        uint   maxblocks;
        uint   numblocks;
        uint   numelements;
        char   **values;
};


#endif
