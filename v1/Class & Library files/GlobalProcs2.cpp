//
// GlobalProcs2.cpp
//
// misc functions for GlobalNav app
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "BlockStringAlloc.h"
#include "GlobalProcs2.h"


extern BlockStringAlloc bsa;


void die(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

double getSubDouble(char *arg, short fieldnum)
{
    double dub;
    short x;
    
    for (x = 0; x < fieldnum; x++)
        arg = findpipeornull(arg) + 1;
    
    sscanf(arg, "%lf", &dub);
    return dub;
}
    
char *getSubString(char *arg, short fieldnum)
{
    char  temp[100];
    short x;
    
    for (x = 0; x < fieldnum; x++)
        arg = findpipeornull(arg) + 1;

    uint len = findpipeornull(arg) - arg;
    
    memcpy(&temp[0], arg, len);
    temp[len] = 0;
    
    return bsa.alloc(&temp[0]);
}

char *findpipeornull(char *where)
{
    uint x, lim = strlen(where);
    
    for (x = 0; x <= lim; x++)
        if ((where[x] == '|') || (where[x] == 0))
            return &where[x];
    
    return NULL;        // can this be reached?
}

void lcase(char *arg)
{
	uint x, len = strlen(arg);

	for (x = 0; x < len; x++)
		arg[x] = tolower(arg[x]);
}

int nosuchfile(char *fname)
{
    FILE *thefile = fopen(fname, "r");
    
    if (thefile == NULL)
        return 1;
    
    fclose(thefile);
    
    return 0;
}

void ucase(char *arg)
{
    uint x, len = strlen(arg);
    
    for (x = 0; x < len; x++)
        arg[x] = toupper(arg[x]);
}

