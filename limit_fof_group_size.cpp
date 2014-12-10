#include "limit_fof_group_size.hpp"


size_t limit_fof_group_size(int n_limit, vector<shock> *sA, vector<tracer> *tA)
{
  long i, j, ttA, ttB;
  vector<shock>  sB;
  vector<tracer> tB;

  ttA = 0;
  ttB = 0;
  for(i=0;i<sA->size();i++)
  {
    if((*sA)[i].l>=n_limit)
    {
      sB.push_back((*sA)[i]);

      for(j=0;j<(*sA)[i].l;j++)
      {
        tB.push_back((*tA)[ttA]);
        tB[ttB].x[0] = (*tA)[ttA].x[0];
        tB[ttB].x[1] = (*tA)[ttA].x[1];

        ttB++;
        ttA++;
      }

    }else{
      ttA += (*sA)[i].l;
    }
  }
  if(sB.size()==0)
  {
    vector<tracer>().swap((*tA));
    vector<shock>().swap((*sA));
    return 0;
  }



  (*tA).swap((tB));
  (*sA).swap((sB));

  (*sA)[0].o = 0;
  for(i=1;i<sA->size();i++)
    (*sA)[i].o = (*sA)[i-1].o + (*sA)[i-1].l;

  return sA->size();
}