#include "construct_fof_catalogue.hpp"
#include "shock_data_types.hpp"
#include "comparison_functions.hpp"
#include "write_fof_catalogues.hpp"
#include "limit_fof_group_size.hpp"

void construct_fof_catalogue(long ntd)
{
  //filename
  char fname[200];

  //number of catalogue groups
  long n_peak;

  //index for shocks
  long ip, ioff;

  vector<shock> svA;      //shock -- the groups we are trying to find

  //minimum group size
  int n_limit = 10;

  //remember the tracer list sorted by density
  td = tv;

  //sort tracers by peak index
  std::sort( tv.begin(), tv.end(), tracer_peak_index_comparison);

  //count the number of peaks
  n_peak = 0;
  for(long tt=0;tt<ntd;tt++)
  {
    if(tt==0)
    {
      n_peak++;
    }else{
      if(tv[tt].peak_index != tv[tt-1].peak_index)
        n_peak++;
    }
  }

  //calculate information about the peaks
  svA.resize(n_peak);

  //if the peak has particles
  if(n_peak>0)
  {
    ip = 0;
    for(int i=0;i<n_peak;i++)
    {
      svA[i].l = 0;
      svA[i].o = 0;
    }

    //for the first peak, there is at least one particle
    svA[ip].l = 1;

    //assign the peak density
    svA[ip].d = td[tv[0].peak_index].d;

    //assign the peak id, remember which tracer is the peak
    svA[ip].id = td[tv[0].peak_index].id;

    //loop over the remaining particles
    for(long tt=1;tt<ntd;tt++)
    {
      //if this particle is in a different peak, increment ip
      if( tv[tt].peak_index != tv[tt-1].peak_index )
      {
        ip++;           //increment peak
        svA[ip].d  = td[tv[tt].peak_index].d;   //record the peak density
        svA[ip].id = td[tv[tt].peak_index].id;  //record the peak id
      }
      svA[ip].l++;  //increment the peak size
    }
  }

  //printf("peak lengths set (tv.size() = %ld)\n",tv.size());

  //find group bounding boxes and offsets
  ioff = 0;
  for(int i=0;i<n_peak;i++)
  {
    //adjust group offset
    if(i>0)
      svA[i].o = svA[i-1].o + svA[i-1].l;

    //initialize bounding box
    for(int k=0;k<3;k++)
    {
      svA[i].min[k] =  1.0e9;
      svA[i].max[k] = -1.0e9;
    }

    //loop over tracers in this group
    for(ip=0;ip<svA[i].l;ip++)
    {
      if(ip+ioff > tv.size())
      {
        printf("ERROR! ip %ld ioff %ld tvs %ld\n",ip,ioff,tv.size());
        fflush(stdout);
        exit(0);
      }

      //find the bounding box of the peak
      for(int k=0;k<3;k++)
      {
        if(tv[ioff+ip].x[k] < svA[i].min[k])
          svA[i].min[k] = tv[ioff+ip].x[k];
        if(tv[ioff+ip].x[k] > svA[i].max[k])
          svA[i].max[k] = tv[ioff+ip].x[k];
        if(svA[i].min[k] > svA[i].max[k])
        {
          printf("Error!!!! min % f max % f\n",svA[i].min[k],svA[i].max[k]);
          fflush(stdout);
        }
      }
    }
    ioff+=svA[i].l;
  }

  //remove small groups
  n_peak = limit_fof_group_size(n_limit, &svA, &tv);

  //printf("n_peak = %ld\n",n_peak);
  printf("%ld\t%ld\n",n_peak,tv.size());

  //output the peak offsets and lengths for info
  //for(int il=0;il<svA.size();il++)
  //  printf("svA o %ld l %ld\n",svA[il].o,svA[il].l);

  //write the peak data and list to file
  sprintf(fname,"fof_data.dat");
  write_fof_data(fname, svA, tv);
  sprintf(fname,"fof_list.dat");
  write_fof_list(fname, svA);

  //time io
  //t_io_end = MPI_Wtime();
  //t_io += (t_io_end-t_io_start);

  /*delete the tree*/
  //delete tree;

  //free tracer particle vector
  //data.resize(extents[1][1]);
  vector<tracer>().swap(tv);
  vector<shock>().swap(svA);
}