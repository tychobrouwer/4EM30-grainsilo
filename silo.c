/*
 * Silo.c
 *
 * Multiple particle dynamics simulation
 *
 * (c) 2016 Joris Remmers TU/e
 */

#include "consts.h"
#include "mylib.h"
#include <time.h>

#define TOTALPARTICLES 2000 // Number of particles that are added to the silo.

int main(void)

{ 
  clock_t t = clock();

  int iCyc = 0;      // Cycle counter
  int iPlot = 0;     // Plot counter
  char svgfile[20];  // File name for output
  double ekin = 0.0; // Kinetic Energy

#if ENABLE_OMP
  omp_set_num_threads(NUM_THREADS);
#endif

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

    ekin = solve(&plist, &cllist);

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

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("The simulation took %f seconds to execute \n", time_taken);
  return 0;
}
