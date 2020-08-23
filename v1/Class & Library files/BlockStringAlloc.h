//
// blockstringalloc.h
//
// declares for block string allocation class
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//


#ifndef BLOCKSTRINGALLOC_H

#define BLOCKSTRINGALLOC_H

#define uint unsigned int

#define BLOCKSIZE_BSA 65536
#define MAXBLOCKS_BSA 16


class BlockStringAlloc
{
    public:
        BlockStringAlloc();
        BlockStringAlloc(uint, short);
        ~BlockStringAlloc();
        char  *alloc(char *);
        char  *alloc(uint);
    private:
        void  clear();
        void  getnewblock();

        char  **blocklist;
        uint  blocksize;
        char  *curblock;
        uint  maxblocks;
        uint  nextstr;
        uint  numblocks;
};

#endif