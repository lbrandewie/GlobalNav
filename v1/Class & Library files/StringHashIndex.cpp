//
// StringHashIndex.cpp
//
// implements a class that allows data to be retrieved by key or index
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//


#include "StringHashIndex.h"
#include "GlobalProcs2.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


StringHashIndex::StringHashIndex()
{
    blocksize = BLOCKSIZE_SHI;
    maxblocks = MAXBLOCKS_SHI;
    
    numblocks = 1;
    numelements = 0;
    
    data = new HashEl[blocksize];
    values = new char *[blocksize];
    
    if (data == NULL)
        die("Can't allocate hash space");
    
    if (values == NULL)
        die("Can't allocate hash space.");
}

StringHashIndex::StringHashIndex(uint bsize, short mblocks)
{
    blocksize = bsize;
    maxblocks = mblocks;
    
    numblocks = 1;
    numelements = 0;
    
    data = new HashEl[blocksize];
    values = new char *[blocksize];

    if (data == NULL)
        die("Can't allocate hash space");
    
    if (values == NULL)
        die("Can't allocate hash space.");
}

StringHashIndex::~StringHashIndex()
{
    //printf("Hash stored %d elements\n", numelements);
    
    delete [] data;
    delete [] values;
}

void StringHashIndex::deletekey(char *key)
{
	
    uint elnum = search(key);
    uint lim = numelements - 1;
    uint x;

    if (strcmp(data[elnum].key, key) == 0)              // found it
    {
        for (x = elnum; x < lim; x++)
        {
            data[x] = data[x + 1];
            values[x] = values[x + 1];
        }
        numelements--;
    }
    else
    {
        printf("Key not found: %s\n", key);
    }
}

uint StringHashIndex::getcount()
{
	return numelements;
}

char *StringHashIndex::getkey(uint index)
{
	return (index < numelements) ? data[index].key : NULL;
}

void StringHashIndex::getnewblock()
{
    if (numblocks < maxblocks)
        numblocks++;
    else
        die("Out of hash space");
    
    HashEl *newdata = new HashEl[blocksize * numblocks];
    
    if (newdata == NULL)
        die("Can't allocate hash space.");
        
    memset(newdata, 0, blocksize * numblocks * 2 * sizeof(char *));
    memcpy(newdata, data, numelements * 2 * sizeof(char *));
    
    char **newvalues = new char *[blocksize * numblocks];
   
    if (newvalues == NULL)
        die("Can't allocate hash space.");
        
    memset(newvalues, 0, blocksize * numblocks * sizeof(char *));
    memcpy(newvalues, values, numelements * sizeof(char *));

    delete [] data;
    delete [] values;
    
    data = newdata;
    values = newvalues;
}
    
char *StringHashIndex::getval(char *key)
{
    if (numelements == 0)
        return NULL;
        
    uint elnum = search(key);
    
	if (data[elnum].key != 0)
		if (strcmp(data[elnum].key, key) == 0)      // found it
			return data[elnum].value;

    return NULL;
}

char *StringHashIndex::getval(uint index)
{
	return (index < numelements) ? values[index] : NULL;
}

void StringHashIndex::insertkey(uint elnum, char *key, char *value)
{
    uint x;
    
    if (numelements == (blocksize * numblocks))
        getnewblock();
    
    for (x = numelements; x > elnum; x--)
        data[x] = data[x - 1];
    
    data[elnum].key = key;
    data[elnum].value = value;
    values[numelements] = value;
    
    numelements++;
}

uint StringHashIndex::search(char *key)
{
    uint x = 0;

	if (numelements == 0) return 0;
    
    while ((x < numelements) && (strcmp(data[x].key, key) < 0))
		x++;
    
    return x;
}
    
void StringHashIndex::setval(char *key, char *val)
{
    uint elnum = search(key);

	if (elnum == numelements)
	{
		data[elnum].key = key;
		data[elnum].value = val;
		values[numelements] = val;
        
        numelements++;
		return;
	}

    if (strcmp(data[elnum].key, key) == 0)         // it's a match
    {
        data[elnum].value = val;
        return;
    }

    insertkey(elnum, key, val);
}

