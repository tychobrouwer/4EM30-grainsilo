#include <stdlib.h>
#include <assert.h>
#include "mylib.h"

int main(void)
{
  CLList cllist;
  initCLList(&cllist);

  assert(cllist.ntot == 0);
  for (int i = 1; i < NR_CELL_X * NR_CELL_Y; i++)
  {
    assert(cllist.head[i] == -1);
    assert(cllist.tail[i] == -1);
  }

  Vec2 vec;
  vec.x = 0;
  vec.y = 3;

  addToCLList(&cllist, vec);

  vec.x = 1;
  vec.y = 2;

  addToCLList(&cllist, vec);

  vec.x = 0;
  vec.y = 3;

  addToCLList(&cllist, vec);

  assert(cllist.ntot == 3);
  assert(cllist.head[85] == 0);
  assert(cllist.head[68] == 1);
  assert(cllist.next[0] == 2);
  assert(cllist.next[2] == 1);
  assert(cllist.next[1] == -1);
  assert(cllist.tail[85] == 2);
  assert(cllist.tail[68] == 1);

  return 1;
}
