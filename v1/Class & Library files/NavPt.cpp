//
// navpt.cpp
//
// implementation for the Navigation Point (NavPt) class
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//

#include <math.h>
#include <stdio.h>

#include "NavPt.h"
#include "BlockStringAlloc.h"

#define uint unsigned int


double NavPt::PI = 4.0 * atan2(1.0, 1.0);
double NavPt::conv = 180.0 / PI;

extern BlockStringAlloc bsa;


NavPt::NavPt()
{
    _x = _y = _z = _lat = _lon = 0.0;
    _x2 = _y2 = 0;
}

NavPt::NavPt(double lat, double lon)
{
    _lat = lat;
    _lon = lon;
    
    _x2 = _y2 = 0;
    calc_xyz();
}

NavPt::NavPt(double x, double y, double z)
{
    _x = x;
    _y = y;
    _z = z;
    
    _x2 = _y2 = 0;
    calc_latlon();
}

NavPt::~NavPt()
{
    // don't do anything (yet)
}

void NavPt::calc_latlon()
{
    _lat = asin(_z) * conv;
    _lon = atan2(_y, _x) * conv;
}

void NavPt::calc_xyz()
{
    _x = cos(_lon / conv) * cos(_lat / conv);
    _y = sin(_lon / conv) * cos(_lat / conv);
    _z = sin(_lat / conv);
}    

char *NavPt::displaylat()
{
	char   temp[20];
	char   ns;

	short  deg, min, sec;
	double frac;

	char fmt[] = "%2dd %2dm %2ds %c";
	
	double tlat = _lat;

	ns = (tlat < 0) ? 'S' : 'N';
	tlat = fabs(tlat);
	deg = (short) floor(tlat);
	frac = (double) (tlat - deg) * 60;
	min = (short) floor(frac);
	sec = (short) floor((frac - min) * 60 + 0.5);

	if (sec == 60)
	{
		sec = 0;
		min++;
	}

	if (min == 60)
	{
		min = 0;
		deg++;
	}

	sprintf(&temp[0], fmt, deg, min, sec, ns);
	return bsa.alloc(&temp[0]);
}

char *NavPt::displaylon()
{
	char   temp[20];
	char   ew;

	short  deg, min, sec;
	double frac;

	char fmt[] = "%3dd %2dm %2ds %c";

	double tlon = _lon;

	ew = (tlon < 0) ? 'E' : 'W';
	tlon = fabs(tlon);
	deg = (short) floor(tlon);
	frac = (double) (tlon - deg) * 60;
	min = (short) floor(frac);
	sec = (short) floor((frac - min) * 60 + 0.5);

	if (sec == 60)
	{
		sec = 0;
		min++;
	}

	if (min == 60)
	{
		min = 0;
		deg++;
	}

	sprintf(&temp[0], fmt, deg, min, sec, ew);
	return bsa.alloc(&temp[0]);
}

double NavPt::dotproduct(NavPt other)
{
    return _x * other._x + _y * other._y + _z * other._z;
}

double NavPt::getangle(NavPt other)
{
    double dot = dotproduct(other);
    return acos(dot) * conv;
}

double NavPt::getlat()
{
    return _lat;
}

double NavPt::getlon()
{
    return _lon;
}

NavPt NavPt::getmidpoint(NavPt other)
{
    double tx = (_x + other._x) / 2.0;
    double ty = (_y + other._y) / 2.0;
    double tz = (_z + other._z) / 2.0;
    
	double mag = sqrt(tx * tx + ty * ty + tz * tz);
    
    tx /= mag;
    ty /= mag;
    tz /= mag;

    NavPt temp(tx, ty, tz);
    
    return temp;
}

double NavPt::getx()
{
    return _x;
}

double NavPt::gety()
{
    return _y;
}

double NavPt::getz()
{
    return _z;
}

uint NavPt::getx2()
{
    return _x2;
}

uint NavPt::gety2()
{
    return _y2;
}

double NavPt::magnitude()
{
    return sqrt(_x * _x + _y * _y + _z * _z);
}

void NavPt::output()            // for debugging, not called
{
    printf("%.2f:%.2f\n", _lat, _lon);
}

void NavPt::setlat(double lat)
{
    _lat = lat;
}

void NavPt::setlon(double lon)
{
    _lon = lon;
}

void NavPt::setx(double x)
{
    _x = x;
}

void NavPt::sety(double y)
{
    _y = y;
}

void NavPt::setz(double z)
{
    _z = z;
}

void NavPt::setx2(uint x2)
{
    _x2 = x2;
}

void NavPt::sety2(uint y2)
{
    _y2 = y2;
}

