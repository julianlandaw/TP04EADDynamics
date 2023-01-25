//
//  apmain.cpp
//
//  Main file for generating action potential plots
//
//
//  Created by Julian Landaw on 10/06/22.
//  Copyright © 2022 Julian Landaw. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>

#ifndef PRECTYPE
#define PRECTYPE double
#endif

#define TYPECELL TTCellIto
#define TYPECELLSTRING "TT"
#include "TTCellIto.cpp"

#ifndef NUMVAR
#define NUMVAR 1
#endif

#ifndef NUMPCL
#define NUMPCL 1
#endif

#define NCELLS NUMVAR*NUMPCL

#ifndef BEATS
#define BEATS 20
#endif

#ifndef REMOVEBEATS
#define REMOVEBEATS 10
#endif

#ifndef stimt
#define stimt 100.0L
#endif

#ifndef VARIABLE
#define VARIABLE nacafac
#endif

#define t_savestart (pcl*REMOVEBEATS)

int main(int argc, char *argv[])
{
    const double var = atof(argv[1]);
    const double pcl = atof(argv[2]);
    const double dt = atof(argv[3]);
    
        #ifdef TT
    double _itofac = 0.0;  // NO ITO
    double _iskfac = 0.0;  // NO ISK
    double _nai = 12.0;    // initiate sodium concentration
    double _ki = 138.0;    // initiate potassium concentration AND WILL BE CLAMPED
    double _nacafac = 1.0; // sodium-calcium exchanger current; 1.0 means it's at baseline, 5.0 means 5x current
    double _ikrfac = 0.0;  // NO IKR
    double _iksfac = 0.5;  // IKS half conductance, at 0.5*0.245 = 0.1225 conductance
    double _icalfac = 0.8; // ICal at 0.000175*0.8 = 0.00014 conductance
        #endif
    
    TYPECELL<1>* h_cell;
    h_cell = new TYPECELL<1>();
    
    FILE *ap;
    char fileSpec1[100];
    snprintf(fileSpec1, 100, "%sap_PCL_%g_VAR_%g.txt", TYPECELLSTRING, pcl, var);
    ap = fopen(fileSpec1, "w");
    
    FILE *saveconditions;
    char fileSpec2[100];
    snprintf(fileSpec2, 100, "%sVARS_PCL_%g_VAR_%g.txt", TYPECELLSTRING, pcl, var);
    saveconditions = fopen(fileSpec2, "w");
        #ifdef TT
    h_cell->itofac[0] = _itofac;
    h_cell->iskfac[0] = _iskfac;
    h_cell->ki[0] = _ki;
    h_cell->nai[0] = _nai;
    h_cell->ikrfac[0] = _ikrfac;
    h_cell->iksfac[0] = _iksfac;
    h_cell->icalfac[0] = _icalfac;
    h_cell->nacafac[0] = _nacafac;
        #endif
    h_cell->VARIABLE[0] = var;
    
    long double t = -stimt;
    double t_save = t_savestart - 100; //200; //pcl*200 - 200;
    double other_tsave = 0;

        #ifdef TT
    double ito, isk, dxtof, dytof;
        #endif
    double stimtime = 0;
    while (t < pcl*BEATS + pcl - dt/2) {
        if (t > -dt/4.0 && t > stimtime - dt/4.0 && t < stimtime + stimduration - dt/4.0) {
            h_cell->stepdt(0,dt,stimulus);
        }
        else {
            h_cell->stepdt(0,dt,0);
        }
        t = t + dt;
        if (t > stimtime + pcl/2.0) {
            stimtime = stimtime + pcl;
        }
        if (t > t_save - dt/4) {
                #ifdef TT
            ito = h_cell->comp_ito(0,dt,dxtof,dytof);
            isk = h_cell->comp_isk(0);
            fprintf(ap,"%g\t%g\t%g\t%g\t%g\t%g\n",(double)t - t_savestart,h_cell->v[0],h_cell->cai[0], h_cell->nai[0], ito, isk);
                #endif
            t_save = t_save + 1.0;
        }
        if (t > other_tsave - dt/4) {
            printf("%g\t%g\n",(double)t,h_cell->v[0]);
            other_tsave = other_tsave + pcl;
        }
    }

    delete h_cell;
    fclose(ap);
    fclose(saveconditions);
    return 0;
}


