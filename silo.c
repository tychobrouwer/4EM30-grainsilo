/*
 * Silo.c
 *
 * Multiple particle dynamics simulation
 *
 * (c) 2016 Joris Remmers TU/e
 */

#include "mylib.h"

#define TOTALPARTICLES 2000 // Number of particles that are added to the silo.
#define USE_ORIGINAL_ALG 0  // Use the original algorithm for the collision detection.

int main(void)

{
  int iCyc = 0;      // Cycle counter
  int iPlot = 0;     // Plot counter
  char svgfile[20];  // File name for output
  double ekin = 0.0; // Kinetic Energy

  Plist plist;
  CLList cllist;

  readInput("silo.dat", &plist);

  while (iCyc < 100 || ekin > 1.0e-8)
  {
    iCyc++;

    int i;

    if (iCyc % 50 == 0 && plist.ntot < plist.nwall + plist.ndoor + TOTALPARTICLES && plist.ndoor > 0)
    {
      addParticle(&plist);
    }

    initCLList(&cllist);
    for (i = 0; i < plist.ntot; i++)
    {
      addToCLList(&cllist, plist.p[i].r);
    }

    ekin = solve(&plist, &cllist, USE_ORIGINAL_ALG);

    checkParticles(&plist);

    if (iCyc % 1000 == 0)
    {
      iPlot++;

      getFilename(svgfile, iPlot);

      plot(svgfile, &plist);

      showInfo(svgfile, ekin, plist.ntot);
    }

    if (iCyc > 100 && ekin < 1.0e-4 && plist.ndoor > 0)
    {
      openDoor(&plist);
    }

    if (iPlot >= 200)
    {
      break;
    }
  }

  return 0;
}
