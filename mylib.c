/*------------------------------------------------------------------------------
 * mylib.c for silo.c
 *
 * Multiple particle dynamics simulation
 *
 *
 * Part of assignment 1 in the course 4EM30:
 *   Scientific Computing for Mechanical Engineering
 *   2020-2021
 *
 * (c) 2009-2022 Joris Remmers TU/e
 *
 *----------------------------------------------------------------------------*/

#include <stdlib.h>
#include "mylib.h"

//------------------------------------------------------------------------------
//  plot
//------------------------------------------------------------------------------

void plot

    (char *name,
     Plist *pl)

{
  FILE *of;
  int iPar;

  char colors[5][10] = {"red", "green", "blue", "grey", "black"};

  of = fopen(name, "w");

  fprintf(of, "<?xml version='1.0' standalone='no'?>\n");
  fprintf(of, "<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN'\n");
  fprintf(of, "'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\n");

  fprintf(of, "<svg width='600px' height='800px' version='1.1'\n");
  fprintf(of, "xmlns='http://www.w3.org/2000/svg'>\n");
  fprintf(of, "<g transform='translate(300,0)'>\n");
  fprintf(of, "<g transform='scale(1.5)'>\n");

  for (iPar = 0; iPar < pl->ntot; iPar++)
  {
    fprintf(of, "<circle cx='%f' cy='%f' r='%f' fill='%s'/>\n",
            100 * pl->p[iPar].r.x,
            500 - 100 * pl->p[iPar].r.y,
            100 * pl->p[iPar].radius,
            colors[pl->p[iPar].type]);
  }

  fprintf(of, "</g>\n</g>\n</svg>\n");
  fclose(of);
}

//------------------------------------------------------------------------------
//  readInput
//------------------------------------------------------------------------------

void readInput

    (char *name,
     Plist *pl)

{
  FILE *fp;
  float x, y, r;
  int iPar;
  int nFix, nGap, i;

  if ((fp = fopen(name, "r")) == NULL)
  {
    printf("Cannot open file.\n");
  }

  i = fscanf(fp, "%d %d", &nFix, &nGap);

  pl->nwall = nFix;
  pl->ndoor = nGap;

  pl->ntot = nFix + nGap;

  for (iPar = 0; iPar < pl->ntot; iPar++)
  {
    i = fscanf(fp, "%e %e %e", &x, &y, &r);

    pl->p[iPar].r.x = x;
    pl->p[iPar].r.y = y;

    pl->p[iPar].radius = r;

    pl->p[iPar].mass = pl->p[iPar].radius * pl->p[iPar].radius;

    if (iPar < nFix)
    {
      pl->p[iPar].type = 0;
    }
    else
    {
      pl->p[iPar].type = 1;
    }
  }

  fclose(fp);
}

//------------------------------------------------------------------------------
//  calcInteraction
//------------------------------------------------------------------------------

void calcInteraction

    (Plist *pl)

{
  int iPar, jPar;

  const int nPar = pl->ntot;

  for (iPar = 0; iPar < nPar; iPar++)
  {
    for (jPar = iPar + 1; jPar < nPar; jPar++)
    {
      intForce(&pl->p[iPar], &pl->p[jPar]);
    }
  }
}

void calcInteractionCL

    (Plist *pl,
     CLList *cl)

{
  int iCell;

  for (iCell = 0; iCell < NR_CELL_X * NR_CELL_Y; iCell++)
  {
    int iPar = cl->head[iCell];

    while (iPar != -1)
    {

      int iNeigh;
      for (int i = 0; i < 9; i++)
      {
        // Loop over row belowe, the same and above cell
        for (int j = -1; j < 2; j++)
        {
          // Loop over cell left, the same and right of cell
          for (int k = -1; k < 2; k++)
          {
            iNeigh = iCell + j * NR_CELL_X + k;

            // check if this neighbour exists, could be a non-existent cell
            if (iNeigh >= 0 && iNeigh < NR_CELL_X * NR_CELL_Y)
            {

              // get all particales in neighbouring cell
              int iParNeigh = cl->head[iNeigh];

              while (iParNeigh != -1)
              {
                if (iPar != iParNeigh)
                {
                  // call intForce for all particles in the bounding cells
                  intForce(&pl->p[iPar], &pl->p[iParNeigh]);
                }
                iParNeigh = cl->next[iParNeigh];
              }
            }
          }
        }
      }

      iPar = cl->next[iPar];
    }
  }
}

//------------------------------------------------------------------------------
//  intForce
//------------------------------------------------------------------------------

void intForce

    (Particle *pi,
     Particle *pj)

{
  Vec2 dr, f;
  double dist, eps, force;

  dr.x = pj->r.x - pi->r.x;
  dr.y = pj->r.y - pi->r.y;

  dist = sqrt(dr.x * dr.x + dr.y * dr.y);

  eps = pi->radius + pj->radius - dist;

  if (eps > 0.)
  {
    force = eps * K_CONST / dist;

    f.x = -force * dr.x + B_CONST * (pj->v.x - pi->v.x);
    f.y = -force * dr.y + B_CONST * (pj->v.y - pi->v.y);

    pi->f.x += f.x;
    pi->f.y += f.y;

    pj->f.x += -f.x;
    pj->f.y += -f.y;
  }
}

//------------------------------------------------------------------------------
//  addGravity
//------------------------------------------------------------------------------

void addGravity

    (Plist *pl)

{
  int iPar;

  for (iPar = 0; iPar < pl->ntot; iPar++)
  {
    pl->p[iPar].f.y += -GRAVITY * pl->p[iPar].mass;
  }
}

//------------------------------------------------------------------------------
//  addParticle
//------------------------------------------------------------------------------

void addParticle

    (Plist *pl)

{
  double xpos;
  double rad;

  int iPar = pl->ntot;

  initParticle(&pl->p[iPar]);

  xpos = -0.25 + rand() % 11 * 0.05;
  assert(xpos >= -0.25 && xpos <= 0.25);

  pl->p[iPar].r.x = xpos;
  pl->p[iPar].r.y = 4.0;

  pl->p[iPar].v.y = -2.0;

  if (iPar % 2 == 0)
  {
    rad = 0.04;
    pl->p[iPar].type = 2;
  }
  else
  {
    rad = 0.03;
    pl->p[iPar].type = 3;
  }

  pl->p[iPar].radius = rad;

  pl->p[iPar].mass = 3.1415 * rad * rad;

  pl->ntot++;

  if (pl->ntot == MAX_PARTICLES)
  {
    printf("Too many particles\n");
  }
}

//------------------------------------------------------------------------------
//  initParticle
//------------------------------------------------------------------------------

void initParticle

    (Particle *p)

{
  p->r.x = 0.;
  p->r.y = 0.;

  p->radius = 0.;

  p->v.x = 0.;
  p->v.y = 0.;

  p->a.x = 0.;
  p->a.y = 0.;

  p->f.x = 0.;
  p->f.y = 0.;

  p->mass = 0.;

  p->type = 0;
}

//------------------------------------------------------------------------------
//  solve
//------------------------------------------------------------------------------

double solve

    (Plist *pl,
     CLList *cl,
     int USE_ORIGINAL_ALG)

{
  const double dt2 = DT * DT;

  double ekin = 0.;

  int iPar;

  const int nPar = pl->ntot;

  for (iPar = pl->nwall + pl->ndoor; iPar < nPar; iPar++)
  {
    pl->p[iPar].r.x += DT * pl->p[iPar].v.x + 0.5 * dt2 * pl->p[iPar].a.x;
    pl->p[iPar].r.y += DT * pl->p[iPar].v.y + 0.5 * dt2 * pl->p[iPar].a.y;

    pl->p[iPar].v.x += 0.5 * DT * pl->p[iPar].a.x;
    pl->p[iPar].v.y += 0.5 * DT * pl->p[iPar].a.y;

    pl->p[iPar].f.x = 0.;
    pl->p[iPar].f.y = 0.;
  }

  if (USE_ORIGINAL_ALG == 1)
  {
    calcInteraction(pl);
  }
  else if (USE_ORIGINAL_ALG == 0)
  {
    calcInteractionCL(pl, cl);
  }

  addGravity(pl);

  for (iPar = pl->nwall + pl->ndoor; iPar < nPar; iPar++)
  {
    pl->p[iPar].a.x = pl->p[iPar].f.x / pl->p[iPar].mass;
    pl->p[iPar].a.y = pl->p[iPar].f.y / pl->p[iPar].mass;

    pl->p[iPar].v.x += 0.5 * DT * pl->p[iPar].a.x;
    pl->p[iPar].v.y += 0.5 * DT * pl->p[iPar].a.y;

    ekin += pl->p[iPar].mass * (pl->p[iPar].v.x * pl->p[iPar].v.x +
                                pl->p[iPar].v.y * pl->p[iPar].v.y);
  }

  return 0.5 * ekin;
}

//------------------------------------------------------------------------------
//  checkParticles
//------------------------------------------------------------------------------

void checkParticles

    (Plist *pl)

{
  int i;

  for (i = pl->ndoor + pl->nwall; i < pl->ntot; i++)
  {
    if (pl->p[i].r.y < -1.0)
    {
      removeParticle(pl, i);
    }
  }
}

//------------------------------------------------------------------------------
//  openDoor
//------------------------------------------------------------------------------

void openDoor

    (Plist *pl)

{
  int i, j;

  for (i = 0; i < pl->ndoor; i++)
  {
    pl->p[pl->nwall + i] = pl->p[pl->ntot - pl->ndoor + i];
  }

  pl->ntot = pl->ntot - pl->ndoor;
  pl->ndoor = 0;

  for (i = pl->nwall; i < pl->ntot; i++)
  {
    j = (int)(pl->p[i].r.y / 0.3);

    if (j % 2 == 0)
    {
      pl->p[i].type = 2;
    }
    else
    {
      pl->p[i].type = 3;
    }
  }
}

//------------------------------------------------------------------------------
//  removeParticle
//------------------------------------------------------------------------------

void removeParticle

    (Plist *pl,
     int iPar)

{
  if (iPar >= pl->ntot || iPar < pl->ndoor + pl->nwall)
  {
    printf("Error\n");
  }

  pl->ntot = pl->ntot - 1;

  pl->p[iPar] = pl->p[pl->ntot];
}

//------------------------------------------------------------------------------
//  getFilename
//------------------------------------------------------------------------------

void getFilename

    (char *names,
     int k)

{
  int m;
  char cc[4];

  strcpy(names, "output000.svg");

  m = sprintf(cc, "%d", k);

  if (k < 10)
  {
    names[8] = cc[0];
  }
  else if (k < 100)
  {
    names[7] = cc[0];
    names[8] = cc[1];
  }
  else
  {
    names[6] = cc[0];
    names[7] = cc[1];
    names[8] = cc[2];
  }
}

//------------------------------------------------------------------------------
//  showInfo
//------------------------------------------------------------------------------

void showInfo

    (char *svgfile,
     double ekin,
     int ntot)

{
  printf("Writing file        : %s\n", svgfile);
  printf("Kinetic energy      : %f\n", ekin);
  printf("Number of particles : %d\n\n", ntot);
}

//------------------------------------------------------------------------------
//  initCLList
//------------------------------------------------------------------------------

void initCLList

    (CLList *cl)

{
  int i;

  for (i = 0; i < NR_CELL_X * NR_CELL_Y; i++)
  {
    cl->head[i] = -1;
  }

  for (i = 0; i < MAX_PARTICLES; i++)
  {
    cl->next[i] = -1;
  }
  cl->ntot = 0;
}

//------------------------------------------------------------------------------
//  addToCLList
//------------------------------------------------------------------------------

void addToCLList

    (CLList *cl,
     Vec2 vec)

{
  int i, j;

  i = (int)((vec.x + 1.3) / CELL_WIDTH);
  j = (int)((vec.y + 1) / CELL_HEIGHT);

  int cell = j * NR_CELL_X + i;

  int id = cl->head[cell];

  int prevId = -1;
  while (id != -1)
  {
    prevId = id;
    id = cl->next[id];
  }

  if (cl->head[cell] == -1)
  {
    cl->head[cell] = cl->ntot;
  }
  else
  {
    cl->next[prevId] = cl->ntot;
  }

  cl->next[cl->ntot] = -1;
  cl->ntot++;
}
