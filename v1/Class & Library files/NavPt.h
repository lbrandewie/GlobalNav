//
// NavPt.h
//
// declares for Navigation Point class
//
// copyright 2020 L.P. "Lars" Brandewie. All Rights Reserved.
//

#ifndef NAVPT_H

#define NAVPT_H

#define uint unsigned int


class NavPt
{
    public:
        NavPt();
        ~NavPt();
        NavPt(double lat, double lon);
        NavPt(double x, double y, double z);
        void   calc_latlon();
        void   calc_xyz();
		char   *displaylat();
		char   *displaylon();
		double dotproduct(NavPt);
        double getangle(NavPt);
        double getlat();
        double getlon();
        NavPt  getmidpoint(NavPt);
        double getx();
        double gety();
        double getz();
        uint   getx2();
        uint   gety2();
        double magnitude();
        void   output();
        void   setlat(double);
        void   setlon(double);
        void   setx(double);
        void   sety(double);
        void   setz(double);
        void   setx2(uint);
        void   sety2(uint);
        static double PI;
        static double conv;
    private:
        double _lat;
        double _lon;
        double _x;
        double _y;
        double _z;
        uint   _x2;
        uint   _y2;
};

#endif
