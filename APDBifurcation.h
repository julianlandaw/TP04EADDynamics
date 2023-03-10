//
//  APDBifurcation.h
//
//  Structure for performing analysis of action potential durations (APDs)
//  given the pacing cycle length (PCL).
//
//  Created by Julian Landaw on 12/25/16.
//  Copyright © 2016 Julian Landaw. All rights reserved.
//

#ifndef APDBifurcation_h
#define APDBifurcation_h

#ifndef PRECTYPE
#define PRECTYPE double
#endif

template <template<int> class typecell, int ncells, int beats>
class APDBifurcation
{
public:
    typecell<ncells> Cells;
    PRECTYPE apds[2*ncells*beats];
#ifdef LR1
    PRECTYPE xrs[2*ncells*beats];
    PRECTYPE ytoss[2*ncells*beats];
#endif
#ifdef LR1_taud
    PRECTYPE xrs[2*ncells*beats];
#endif

#ifdef TT
    PRECTYPE kis[2*ncells*beats];
    PRECTYPE nais[2*ncells*beats];
    PRECTYPE cais[2*ncells*beats];
    PRECTYPE casrs[2*ncells*beats];
#endif
#ifdef UCLA
    //PRECTYPE kis[2*ncells*beats];
    PRECTYPE nais[2*ncells*beats];
    PRECTYPE cais[2*ncells*beats];
    PRECTYPE cass[2*ncells*beats];
    //PRECTYPE casrs[2*ncells*beats];
#endif

#ifdef OHara
    //PRECTYPE kis[2*ncells*beats];
    PRECTYPE nais[2*ncells*beats];
    PRECTYPE cais[2*ncells*beats];
    PRECTYPE cass[2*ncells*beats];
    //PRECTYPE casrs[2*ncells*beats];
#endif

#ifdef LR2
    PRECTYPE kis[2*ncells*beats];
    PRECTYPE nais[2*ncells*beats];
    PRECTYPE cais[2*ncells*beats];
#endif
#ifdef TTMod
    PRECTYPE kis[2*ncells*beats];
    PRECTYPE nais[2*ncells*beats];
    PRECTYPE cais[2*ncells*beats];
    PRECTYPE casrs[2*ncells*beats];
#endif
    PRECTYPE pcls[ncells];
    PRECTYPE startapds[ncells];
    PRECTYPE st[ncells];
    int curbeat[ncells];
    double stimtime[ncells];
    int donebifurcation;
    
    APDBifurcation();
    
    void iterate(const int id, long double dt, long double t);
    
    void iterateall(long double dt, long double t);
    
    void dobif(long double dt, double startt);
};

#endif //APDBifurcation_h
