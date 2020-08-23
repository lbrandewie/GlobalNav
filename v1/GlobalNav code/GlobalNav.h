//
// GlobalNav.h
//
// declares for global Great-Circle nav program (CLI version)
//
// Copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//


#ifndef GLOBALNAV_H
#define GLOBALNAV_H


void   fold(NavPt *, short);
NavPt  getAdjacentPoint(NavPt, NavPt);
double getBearing(NavPt, NavPt);
short  getDistFmt(char *);
double getDistance(NavPt, NavPt, short);
double getDistanceNM(NavPt, NavPt);
double getDistanceKM(NavPt, NavPt);
double getDistanceSM(NavPt, NavPt);
void   loadfile(void);
short  getWaypoints(char *);


#endif
