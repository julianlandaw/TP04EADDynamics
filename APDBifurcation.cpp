//
//  APDBifurcation.cpp
//
//  Structure for performing analysis of action potential durations (APDs)
//  given the pacing cycle length (PCL) and parameter level.
//
//  Created by Julian Landaw on 10/10/22.
//  Copyright © 2022 Julian Landaw. All rights reserved.
//

#ifndef PRECTYPE
#define PRECTYPE double
#endif

#include "APDBifurcation.h"

#ifndef APDBifurcation_cpp
#define APDBifurcation_cpp

#ifndef threshold
#define threshold -75.0
#endif

#ifndef stimulus
#define stimulus -80.0
#endif

#ifndef stimduration
#define stimduration 0.5
#endif

#ifndef DV_MAX
#define DV_MAX 0.1
#endif

#ifndef ADAPTIVE
#define ADAPTIVE 10
#endif

template <template<int> class typecell, int ncells, int beats>
APDBifurcation<typecell, ncells, beats>::APDBifurcation() {
    int i;
    for (i = 0; i < ncells*beats; i++) {
        apds[i] = 0.1;
    }
        #ifdef TT
    for (i = 0; i < 2*ncells*beats; i++) {  
        kis[i] = 0; 
        nais[i] = 0;
        cais[i] = 0;
        casrs[i] = 0;
    }
        #endif
    for (i = 0; i < ncells; i++) {
        pcls[i] = 0.0;
        stimtime[i] = 0.0;
        startapds[i] = 0.0;
        st[i] = 0.0;
        curbeat[i] = -1;
    }
    donebifurcation = 0;
}

template <template<int> class typecell, int ncells, int beats>
void APDBifurcation<typecell, ncells, beats>::iterate(const int id, long double dt, long double t) {
    if (id < ncells && curbeat[id] < beats) {
        PRECTYPE vold = Cells.v[id];
        st[id] = (t > -dt/4.0 && t > stimtime[id] - dt/4.0 && t < stimtime[id] + stimduration - dt/4.0) ? stimulus : 0.0;
        Cells.stepdt(id, (PRECTYPE)dt, st[id]);
        PRECTYPE vnew = Cells.v[id];
        if (vold < threshold && vnew >= threshold) { //Is the voltage now crossing -75 mV --> starting the AP? 
            startapds[id] = t;
                #ifdef TT
            if (curbeat[id] >= -1 && curbeat[id] < beats - 1) {
                kis[2*(beats*id + curbeat[id] + 1)] = Cells.ki[id];      // Save the initial K+ level 
                nais[2*(beats*id + curbeat[id] + 1)] = Cells.nai[id];    // Save the initial Na+ level
                cais[2*(beats*id + curbeat[id] + 1)] = Cells.cai[id];    // Save the initial Ca2+ level
                casrs[2*(beats*id + curbeat[id] + 1)] = Cells.casr[id];  // Save the initial Ca-SR level
            }
                #endif
        }
        else if (vold >= threshold && vnew < threshold) { //Is the voltage going below -75 mV -> ending the AP?
            curbeat[id] += 1;
            stimtime[id] += pcls[id];
            while (stimtime[id] < t - dt/4.0) {
               stimtime[id] += pcls[id];
            }
                #ifdef TT
            kis[2*(beats*id + curbeat[id]) + 1] = Cells.ki[id];          // Save the end K+ level 
            nais[2*(beats*id + curbeat[id]) + 1] = Cells.nai[id];        // Save the end Na+ level 
            cais[2*(beats*id + curbeat[id]) + 1] = Cells.cai[id];        // Save the end Ca2+ level
            casrs[2*(beats*id + curbeat[id]) + 1] = Cells.casr[id];      // Save the end Ca-SR level
                #endif
            apds[beats*id + curbeat[id]] = t - startapds[id];            // Save the APD (the AP end time minus start time)
            if (curbeat[id] == beats - 1) {
                donebifurcation = donebifurcation + 1;
                printf("%d\n",donebifurcation);
            }
            
        }
    }
}

template <template<int> class typecell, int ncells, int beats>
void APDBifurcation<typecell, ncells, beats>::iterateall(long double dt, long double t) {
    for (int i = 0; i < ncells; i++) {
        iterate(i, dt, t);
    }
}

template <template<int> class typecell, int ncells, int beats>
void APDBifurcation<typecell, ncells, beats>::dobif(long double dt, double startt) {
    long double t = startt;
    long double t_save = 0;
    double dum;
    while (donebifurcation < ncells) {
        iterateall(dt, t);
        t = t + dt;
        if ((t > stimtime[0] - dt/4.0 && t < stimtime[0] + 3*dt/4.0)) {
                #ifdef TT
            printf("%g\t%d\t%g\t%g\t%g\t%g\n",(double)t, curbeat[0],Cells.v[0],Cells.cai[0],Cells.nai[0], apds[curbeat[0]]);
                #endif
            t_save = t_save + pcls[0];
        }
    }
}
#endif //APDBifurcation_cpp
