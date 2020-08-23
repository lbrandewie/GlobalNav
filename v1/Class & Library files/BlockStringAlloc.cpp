//
// blockstringalloc.cpp
//
// implementation of the BlockStringAlloc class
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//

#include <stdio.h>
#include <string.h>

#include "BlockStringAlloc.h"
#include "GlobalProcs2.h"


BlockStringAlloc::BlockStringAlloc()
{
    blocksize = BLOCKSIZE_BSA;
    maxblocks = MAXBLOCKS_BSA;
    
    blocklist = new char *[maxblocks];

    if (blocklist == NULL)
        die("Can't allocate string space.");
        
    clear();
    
    numblocks = 1;
    nextstr = 0;
    blocklist[0] = new char[blocksize];
    
    if (blocklist[0] == NULL)
        die("Can't allocate string space.");
        
    curblock = blocklist[0];
}

BlockStringAlloc::BlockStringAlloc(uint bsize, short mblocks)
{
    blocksize = bsize;
    maxblocks = mblocks;
    
    blocklist = new char *[maxblocks];

    if (blocklist == NULL)
        die("Can't allocate string space.");
    
    clear();
    
    numblocks = 1;
    nextstr = 0;
    blocklist[0] = new char[blocksize];

    if (blocklist[0] == NULL)
        die("Can't allocate string space.");

    curblock = blocklist[0];
}

BlockStringAlloc::~BlockStringAlloc()
{
    uint x;
    
    //printf("Used %d blocks of strings with %d allocated in last block\n", numblocks, nextstr);
    
    for (x = 0; x < maxblocks; x++)
        if (blocklist[x] != 0)
            delete [] blocklist[x];
}

char *BlockStringAlloc::alloc(char *what)
{
    uint len = strlen(what) + 1;

    if (len > blocksize)
        die("Can't allocate string longer than blockssize.");
    
    if ((nextstr + len) > blocksize)
        getnewblock();
        
	uint tnextstr = nextstr;
    nextstr += len;
    strcpy(curblock + tnextstr, what);
    
    return curblock + tnextstr;
}
    
char *BlockStringAlloc::alloc(uint size)
{
    if (size > blocksize)
        die("Can't allocate string longer than blocksize.");
        
    if ((nextstr + size) > blocksize)
        getnewblock();
    
	uint tnextstr = nextstr;
    nextstr += size;
    
    return curblock + tnextstr;
}

void BlockStringAlloc::clear()	         // called by constructor only
{
    uint x;
    
    for (x = 0; x < maxblocks; x++)
        blocklist[x] = 0;
}

void BlockStringAlloc::getnewblock()
{
	if (numblocks < maxblocks)
    {
		blocklist[numblocks] = new char[blocksize];
        
        if (blocklist[numblocks] == NULL)
            die("Can't allocate string space.");
            
        curblock = blocklist[numblocks];
		nextstr = 0;
        numblocks++;
    }
    else
	{
        die("Out of string space.");
	}
}

