#ifndef MYLIB_H
#define MYLIB_H

/*------------------------------------------------------------------------------
 * Header file for Library mylib.h for silo.c
 *
 * Multiple particle dynamics simulation
 *
 *
 * Part of assignment 1 in the course 4EM30:
 *   Scientific Computing for Mechanical Engineering
 *   2021-2022
 *
 * (c) 2009-2022 Joris Remmers TU/e
 *
 *----------------------------------------------------------------------------*/

#include "consts.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#if ENABLE_OMP
#include <omp.h>
#endif

#define GRAVITY 9.81
#define MAX_PARTICLES 3000

#define NR_CELL_X 25
#define NR_CELL_Y 49
#define CELL_WIDTH (2.6 / NR_CELL_X)
#define CELL_HEIGHT (5.0 / NR_CELL_Y)

#define B_CONST 0.05
#define K_CONST 100.0
#define DT 0.001
#define PI_CONST 3.1415

//------------------------------------------------------------------------------
// Definition of new structure Vec2 (vector with two components)
//------------------------------------------------------------------------------

typedef struct
{
    double x, y;
} Vec2;

//------------------------------------------------------------------------------
// Definition of new type Particle, containing position(r), velocity(v),
//   acceleration(a), force(f), radius (radius) and mass.
//------------------------------------------------------------------------------

typedef struct
{
    Vec2 r, v, a, f; // position, velocity, accelleration, force
    double radius, mass;
    int type;

} Particle;

//------------------------------------------------------------------------------
// Definition of new type Plist (list of particles, unordered)
//   note that the length of the array is set statically.
//------------------------------------------------------------------------------

typedef struct
{
    Particle p[MAX_PARTICLES];
    int nwall;
    int ndoor;
    int ntot;
} Plist;

//------------------------------------------------------------------------------
// Definition of new type CellLinkedList (linked list of particles in a cell)
//   note that the length of the array is set statically.
//------------------------------------------------------------------------------

typedef struct
{
    int head[NR_CELL_X * NR_CELL_Y];
    int tail[NR_CELL_X * NR_CELL_Y];
    int next[MAX_PARTICLES];
    int ntot;
} CLList;

//------------------------------------------------------------------------------
//  Plot the silo and particle in SVG format
//  pre:    correct filename (ends with .svg)
//  post:   writes a file to the current directory (.svg)
//  return: -
//------------------------------------------------------------------------------

void plot

    (char *name,
     Plist *plist);

//------------------------------------------------------------------------------
//  Reads the input file
//  pre:    valid input txt file and empty particle list
//  post:   Plist is filled
//  return: -
//------------------------------------------------------------------------------

void readInput

    (char *name,
     Plist *plist);

//------------------------------------------------------------------------------
//  Adds a particle to the list from a point on top of the silo
//  pre:    -
//  post:   a particle is added with a velocity v.y=-2.0
//  return: -
//------------------------------------------------------------------------------

void addParticle

    (Plist *plist);

//------------------------------------------------------------------------------
//  Initialises particle
//  pre:    -
//  post:   Initialises particle
//  return: -
//------------------------------------------------------------------------------

void initParticle

    (Particle *plist);

//------------------------------------------------------------------------------
//  Opens the silo door
//  pre:    -
//  post:   removes the particles that represent the door and change
//          the colors of the dynamic particles to make a banded
//          structures (for visualisation)
//  return: -
//------------------------------------------------------------------------------

void openDoor

    (Plist *plist);

//------------------------------------------------------------------------------
//  Solve a single step in the verlet algorithm
//  pre:    -
//  post:   Updated particle positions and velocities
//  return: kinetic energy
//------------------------------------------------------------------------------

double solve

    (Plist *plist,
     CLList *cllist,
     int USE_ORIGINAL_ALG);

//------------------------------------------------------------------------------
//  Checks if particle is below the line at y = -1.0. IF so, the particle is
//    removed
//  pre:    -
//  post:   Particles that are below y = -1.0 are removed.
//  return: -
//------------------------------------------------------------------------------

void checkParticles

    (Plist *plist);

//------------------------------------------------------------------------------
//  Calculate all particle interaction forces
//  pre:    -
//  post:   update force vectors in particle list
//  return: -
//------------------------------------------------------------------------------

void calcInteraction

    (Plist *plist);

//------------------------------------------------------------------------------
//  Calculate the interaction force between two partcles
//  pre:    two valid particles
//  post:   if in contact, updated forces, if not inte forces are zero.
//  return: -
//------------------------------------------------------------------------------

void intForce

    (Particle *pi,
     Particle *pj);

void intForceCalc

    (Vec2 *df,
     Particle *pi,
     Particle *pj);

void intForceUpdate

    (Vec2 df,
     Particle *pi,
     Particle *pj);

//------------------------------------------------------------------------------
//  Add gravity force to all particles
//  pre:    -
//  post:   updated force
//  return: -
//------------------------------------------------------------------------------

void addGravity

    (Plist *plist);

//------------------------------------------------------------------------------
//  Removes particle from the list
//  pre:    non-empty particle list
//  post:   removes the particle iPar, updates position
//  return: -
//------------------------------------------------------------------------------

void removeParticle

    (Plist *plist,
     int iPar);

//------------------------------------------------------------------------------
//  Get filename for svg output file
//  pre:    -
//  post:   returns filename
//  return: -
//------------------------------------------------------------------------------

void getFilename

    (char *names,
     int k);

//------------------------------------------------------------------------------
//  pre:    -
//  post:   writes number of particles, current ouptut file and kinetic energy
//            to the screen
//  return: -
//------------------------------------------------------------------------------

void showInfo

    (char *svgfile,
     double ekin,
     int ntot);

void addToCLList

    (CLList *cllist,
     Vec2 vec);

void initCLList

    (CLList *cl);

#endif
