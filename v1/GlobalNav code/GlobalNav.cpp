//
// GlobalNav.cpp
//
// Global Great-Circle Navigation program (CLI version)
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//


#include "BlockStringAlloc.h"
#include "StringHashIndex.h"
#include "NavPt.h"
#include "GlobalProcs2.h"
#include "GlobalNav.h"

#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


BlockStringAlloc bsa;
StringHashIndex  shi;

void main(int argc, char *argv[])
{
    char   *city1, *city2, temp[20];
    double lat1, lon1, lat2, lon2;
    
    short  waypoints, lim, cnt, x, distfmt = 0;
	double bearing[129];

    NavPt  arr[129];
	
    if (argc < 4)					// process args
	    {
        printf("Usage: %s airport1 airport2 waypoints [distfmt]\n", argv[0]);
        exit(0);
    }
    
    if (argc == 5)
		distfmt = getDistFmt(argv[4]);
    
	if (distfmt < 0)
		die("Don't understand distance format.");
			
    waypoints = getWaypoints(argv[3]);
    lim = waypoints + 2;

    if (nosuchfile("airports.txt"))			// make sure we have our data
        die("airports.txt not found!");
        
	loadfile();

    ucase(argv[1]);
    ucase(argv[2]);
    
	city1 = shi.getval(argv[1]);			// get city data
    city2 = shi.getval(argv[2]);
    
    if (city1 == NULL)
	{
		printf("Airport %s not found.\n", argv[1]);
		exit(0);
	}

	
    if (city2 == NULL)
    {
		printf("Airport %s not found.\n", argv[2]);
		exit(0);
	}
        
    lat1 = getSubDouble(city1, 2);			// get lat/lon data
    lon1 = getSubDouble(city1, 3);
    
    lat2 = getSubDouble(city2, 2);
    lon2 = getSubDouble(city2, 3);
        
	NavPt temp1(lat1, lon1);
	NavPt temp2(lat2, lon2);

	arr[0] = temp1;
	arr[1] = temp2;
	
	cnt = 2;

	while (cnt < lim)						// calc waypoints
	{
		fold(arr, cnt);
		cnt += cnt - 1;
	}
    
    lim--;

	for (x = 0; x < lim; x++)				// get bearing at each waypoint
	{
		temp1 = getAdjacentPoint(arr[x], arr[x + 1]);
		bearing[x] = getBearing(arr[x], temp1);
	}

	temp1 = getAdjacentPoint(arr[x], arr[x - 1]);
	bearing[x] = getBearing(temp1, arr[x]);

	// print it out

	printf("\nWhere   Lat            Lon               Dist  Course\n");
	printf("\n%6s: %s, %s %7.2lf %7.2lf\n", "Start", arr[0].displaylat(), arr[0].displaylon(), 0.0, bearing[0]);

    
	for (x = 1; x < lim; x++)
	{
		sprintf(&temp[0], "Way%d", x);
		printf("%6s: %s, %s %7.2lf %7.2lf\n", &temp[0], arr[x].displaylat(), arr[x].displaylon(), getDistance(arr[0], arr[x], distfmt), bearing[x]);
	}

	printf("%6s: %s, %s %7.2lf %7.2lf\n\n", "End", arr[x].displaylat(), arr[x].displaylon(), getDistance(arr[0], arr[x], distfmt), bearing[x]);
}

short getWaypoints(char *arg)
{
    short val, temp, x;
    
    sscanf(arg, "%d", &val);
    
    for (x = 4; x <= 7; x++)
    {
        temp = (short) pow(2, x) - 1;
    
        if (val <= temp)
            return temp;
    }
    
    return 127;
}

void fold(NavPt *arr, short cnt)
{
	NavPt arr2[129];
	NavPt arr3[129];

	short x, lim = cnt - 1;
	short cnt2 = 0;

	for (x = 0; x < lim; x++)
		arr2[x] = arr[x].getmidpoint(arr[x + 1]);

	lim = 2 * cnt - 1;
	x = 0;

	while (cnt2 < lim)
	{
		arr3[cnt2++] = arr[x];
		arr3[cnt2++] = arr2[x];
		x++;
	}
	arr3[cnt2] = arr[x];

	for (x = 0; x < lim; x++)
		arr[x] = arr3[x];
}

void loadfile(void)
{
	char key[4];
	char value[100];

    key[3] = 0;

    FILE *infile = fopen("airports.txt", "r");

    if (infile == NULL)
        die("airports.txt can't be opened.");
    
    while (!feof(infile))
    {
        fgets(&value[0], 99, infile);
		value[strlen(&value[0]) - 1] = 0;         // get rid of LF
        
		if (strchr(&value[0], '|') != NULL)      // sanity check
        {
            memcpy(&key[0], &value[0], 3);
           
            char *k = bsa.alloc(&key[0]);
            char *v = bsa.alloc(&value[0]);
        
            shi.setval(k, v);
        }
    }
    
    fclose(infile);
}

double getDistance(NavPt pt1, NavPt pt2, short distfmt)
{
    switch (distfmt)
    {
        case 0:
            return getDistanceNM(pt1, pt2);
        case 1:
            return getDistanceSM(pt1, pt2);
        default:
            return getDistanceKM(pt1, pt2);
    }
}

double getDistanceKM(NavPt pt1, NavPt pt2)
{
    return getDistanceSM(pt1, pt2) * 1.609344;
}

double getDistanceNM(NavPt pt1, NavPt pt2)
{
    return getDistanceSM(pt1, pt2) / 1.151400819;
}

double getDistanceSM(NavPt pt1, NavPt pt2)
{
	double anglefrac = pt1.getangle(pt2) / 360.0;
	return anglefrac * 24881.0;
}

short getDistFmt(char *arg)
{
	ucase(arg);

	switch (arg[0])
	{
		case 'N':
			return 0;
		case 'S':
			return 1;
		case 'K':
			return 2;
	}
    
	return -1;
}

NavPt getAdjacentPoint(NavPt pt1, NavPt pt2)
{
    NavPt temp = pt2;
    
    while (getDistanceSM(pt1, temp) > 0.01)
	    temp = pt1.getmidpoint(temp);
        
	return temp;
}

double getBearing(NavPt pt1, NavPt pt2)		    // requires two closely-spaced points (getAdjacentPoint)
{
	double avglat = (pt1.getlat() + pt2.getlat()) / 2.0;
	double dx = (pt2.getlon() - pt1.getlon()) * cos(avglat / NavPt::conv);
	double dy = (pt2.getlat() - pt1.getlat());

	if (dx < 0)
		return 90 - atan2(dy, -dx) * NavPt::conv;
	
	if (dx > 0)
		return 270 + atan2(dy, dx) * NavPt::conv;

	return (dy > 0) ? 0.0 : 180.0;
}

