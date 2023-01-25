//
//  apdbifurcationmain.cpp
//
//  Main file for generating APD bifurcation diagrams.
//  .txt output files are organized as:
//  1st column:         Pacing cycle length (PCL)
//  2nd column:         Multiplication factor of the variable parameter (default is "nacafac", but this can be changed with 
//                      the VARIABLE macro)
//  Remaining columns:  Beat-to-beat series of action potential durations (APDs) OR beat-to-beat levels of different variables.
//                      For example, one of the outputs of the ten Tusscher code will output Ca2+ levels at the beginning
//                      and end of an action potential.
//                      
//  The number of remaining columns depends on the macros BEATS and REMOVEBEATS
//  where #remaining columns = (BEATS - REMOVEBEATS) for actional potential durations (APDs)
//  OR #remaining columns = 2*(BEATS - REMOVEBEATS) if saving initial and final variable levels (e.g. Ca2+, Na+, K+, etc.)
//
//  ---------- EXAMPLE CODE ----------
//
//  After running the default Makefile, one output program is TTbif.out, which is the program made from compiling this file.
//  By default, it will simulate 2000 beats, and will "discard" the results the first 1500 beats, thus saving the APDs,
//  Ca2+, Na+, K+, and Ca-SR of the last 500 beats.
//  
//  Additionally, by default it will only run one simulation - this can be changed by changing the macros NUMVAR and NUMPCL.
//  NUMVAR dictates the depth of the parameter search of VARIABLE, which is set to nacafac by default.
//  NUMPCL dictates that for the pacing cycle length (PCL) in the parameter search
//
//  For example, by setting NUMVAR to 10 and NUMPCL to 10, e.g. with the following: 
//
//  >> make NUMVAR=10 NUMPCL=10 VARIABLE=nacafac BEATS=2000 REMOVEBEATS=1500
//
//  then running the program
//  
//  >> ./TTbif.out 1 5 2000 2500 0.05
//
//  will run 100 simulations with the time step dt=0.05, where nacafac ranges between 1 and 5 (evenly spaced), 
//  and pcl ranges between 2000 and 2500.
//
//  The output files will have 100 rows, each corresponding to a different value of pcl and nacafac, which will be 
//  placed in the first and second columns of each row.
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

#define NCELLS (NUMVAR*NUMPCL)

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

#include "APDBifurcation.cpp"

int main(int argc, char *argv[])
{
    const double minvar = atof(argv[1]);      
    const double maxvar = atof(argv[2]);
    const double minpcl = atof(argv[3]);
    const double maxpcl = atof(argv[4]);
    const long double dt = atof(argv[5]);
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
    
    double byvar = (NUMVAR > 1) ? (maxvar - minvar)/(NUMVAR - 1) : 1;
    double bypcl = (NUMPCL > 1) ? (maxpcl - minpcl)/(NUMPCL - 1) : 1;
    
    APDBifurcation<TYPECELL, NCELLS, BEATS>* h_cells;
    
    h_cells = new APDBifurcation<TYPECELL, NCELLS, BEATS>();
    
    
    FILE *allbifs;
    char fileSpec1[100];
    snprintf(fileSpec1, 100, "%sapd_PCL_%g_VAR_%g.txt", TYPECELLSTRING, minpcl, minvar);
    allbifs = fopen(fileSpec1, "w");
    
#ifdef TT
    FILE *caibifs;
    char fileSpec2[100];
    snprintf(fileSpec2, 100, "%scai_PCL_%g_VAR_%g.txt", TYPECELLSTRING, minpcl, minvar);
    caibifs = fopen(fileSpec2, "w");
    
    FILE *casrbifs;
    char fileSpec3[100];
    snprintf(fileSpec3, 100, "%scasr_PCL_%g_VAR_%g.txt", TYPECELLSTRING, minpcl, minvar);
    casrbifs = fopen(fileSpec3, "w");
    
    FILE *kibifs;
    char fileSpec4[100];
    snprintf(fileSpec4, 100, "%ski_PCL_%g_VAR_%g.txt", TYPECELLSTRING, minpcl, minvar);
    kibifs = fopen(fileSpec4, "w");
    
    FILE *naibifs;
    char fileSpec5[100];
    snprintf(fileSpec5, 100, "%snai_PCL_%g_VAR_%g.txt", TYPECELLSTRING, minpcl, minvar);
    naibifs = fopen(fileSpec5, "w");
#endif
    
    int index;
    for (int i = 0; i < NUMPCL; i++) {
        for (int j = 0; j < NUMVAR; j++) {
            index = (NUMVAR*i) + j;
                #ifdef TT
            h_cells->Cells.itofac[index] = _itofac;
            h_cells->Cells.iskfac[index] = _iskfac;
            h_cells->Cells.nai[index] = _nai;
            h_cells->Cells.ki[index] = _ki;
            h_cells->Cells.nacafac[index] = _nacafac;

            h_cells->Cells.ikrfac[index] = _ikrfac;
            h_cells->Cells.iksfac[index] = _iksfac;
            h_cells->Cells.icalfac[index] = _icalfac;
                #endif
            h_cells->Cells.VARIABLE[index] = byvar*j + minvar;
            h_cells->pcls[index] = bypcl*i + minpcl;
        }
    }
    
    printf("Byte Size of Cells on Device: %lu\n", sizeof(APDBifurcation<TYPECELL, NCELLS, BEATS>) );
    //Now run the program
    
    long double t = -stimt;
    h_cells->dobif(dt, t);
    
    for (int i = 0; i < NUMPCL; i++) {
        for (int j = 0; j < NUMVAR; j++) {
            index = (NUMVAR*i) + j;
            fprintf(allbifs, "%g\t%g", bypcl*i + minpcl, byvar*j + minvar);
            for (int l = REMOVEBEATS; l < BEATS; l++) {
                fprintf(allbifs, "\t%g", h_cells->apds[BEATS*(index) + l]);
            }
            fprintf(allbifs, "\n");
                #ifdef TT
            fprintf(caibifs, "%g\t%g", bypcl*i + minpcl, byvar*j + minvar);
            fprintf(casrbifs, "%g\t%g", bypcl*i + minpcl, byvar*j + minvar);
            fprintf(kibifs, "%g\t%g", bypcl*i + minpcl, byvar*j + minvar);
            fprintf(naibifs, "%g\t%g", bypcl*i + minpcl, byvar*j + minvar);
            
            for (int l = 2*REMOVEBEATS; l < 2*BEATS; l++) {
                fprintf(caibifs, "\t%g", h_cells->cais[2*BEATS*(index) + l]);
                fprintf(casrbifs, "\t%g", h_cells->casrs[2*BEATS*(index) + l]);
                fprintf(kibifs, "\t%g", h_cells->kis[2*BEATS*(index) + l]);
                fprintf(naibifs, "\t%g", h_cells->nais[2*BEATS*(index) + l]);
            }
            fprintf(caibifs, "\n");
            fprintf(casrbifs, "\n");
            fprintf(kibifs, "\n");
            fprintf(naibifs, "\n");
                #endif            
        }
    }

    delete h_cells;
    fclose(allbifs);
#ifdef TT
    fclose(caibifs);
    fclose(casrbifs);
    fclose(kibifs);
    fclose(naibifs);
#endif
    return 0;
}


