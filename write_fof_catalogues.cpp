#include <stdio.h>
#include "write_fof_catalogues.hpp"


using namespace std;

void write_null_fof_data(char fname[])
{
  int  n;
  long io;
  FILE *fp_dat;

  //write snap peak details
  if(!(fp_dat = fopen(fname,"w")))
  {
    printf("Error opening %s.\n",fname);
    fflush(stdout);
  }
  n = 0;

  fwrite(&n,sizeof(int),1,fp_dat);
  fclose(fp_dat);

}

void write_fof_data(char fname[], vector<shock> s, vector<tracer> t)
{
  int  n;
  long io;
  FILE *fp_dat;

  float *dpeak;
  float *xpeak;
  float *ypeak;
  float *zpeak;
  float *vxpeak;
  float *vypeak;
  float *vzpeak;
  long  *lpeak;

  if(s.size()==0)
  {
    write_null_fof_data(fname);
    return;
  }
  
  //write snap peak details
  if(!(fp_dat = fopen(fname,"w")))
  {
    printf("Error opening %s.\n",fname);
    fflush(stdout);
  }
  io = 0;
  n = s.size();

  //printf("s.size() %ld\n",s.size());
  //fflush(stdout);

  fwrite(&n,sizeof(int),1,fp_dat);
  if(n==0)
  {
    fclose(fp_dat);
    return;
  }
  for(int i=0;i<n;i++)
    io += s[i].l;
  fwrite(&io,sizeof(long),1,fp_dat);


  io = 0;
  for(int i=0;i<n;i++)
  {
    dpeak  = (float *) malloc(s[i].l * sizeof(float));
    xpeak  = (float *) malloc(s[i].l * sizeof(float));
    ypeak  = (float *) malloc(s[i].l * sizeof(float));
    zpeak  = (float *) malloc(s[i].l * sizeof(float));
    vxpeak = (float *) malloc(s[i].l * sizeof(float));
    vypeak = (float *) malloc(s[i].l * sizeof(float));
    vzpeak = (float *) malloc(s[i].l * sizeof(float));
    lpeak  = (long  *) malloc(s[i].l * sizeof(long));

    for(long k=0;k<s[i].l;k++)
    {
      dpeak[k]  = t[io].d;
      xpeak[k]  = t[io].x[0];
      ypeak[k]  = t[io].x[1];
      zpeak[k]  = t[io].x[2];
      vxpeak[k] = t[io].v[0];
      vypeak[k] = t[io].v[1];
      vzpeak[k] = t[io].v[2];
      lpeak[k]  = t[io].id;

		
      io++;
    }

    fwrite(dpeak,sizeof(float),s[i].l,fp_dat);
    fwrite(xpeak,sizeof(float),s[i].l,fp_dat);
    fwrite(ypeak,sizeof(float),s[i].l,fp_dat);
    fwrite(zpeak,sizeof(float),s[i].l,fp_dat);
    fwrite(vxpeak,sizeof(float),s[i].l,fp_dat);
    fwrite(vypeak,sizeof(float),s[i].l,fp_dat);
    fwrite(vzpeak,sizeof(float),s[i].l,fp_dat);
    fwrite(lpeak,sizeof(long),s[i].l,fp_dat);

    free(dpeak);
    free(xpeak);
    free(ypeak);
    free(zpeak);
    free(vxpeak);
    free(vypeak);
    free(vzpeak);
    free(lpeak);
  }

  //close peak details
  fclose(fp_dat);
}

void write_null_fof_list(char fname[])
{

  FILE  *fp_list;
  int    n;
  long  *l;
  long  *o;
  float *d;
  long  *id;
  float *b_min;
  float *b_max;
  int isub;

  //for(isub=0;isub<nfiles;isub++)
  //{

    //write the peak list to file
    if(!(fp_list = fopen(fname,"w")))
    {
      printf("Error opening %s.\n",fname);
      fflush(stdout);
    }

    n = 0;
    fwrite(&n,1,sizeof(int),fp_list);
    fclose(fp_list);
  //}
}

void write_fof_list(char fname[], vector<shock> s)
{
  FILE  *fp_list;
  int    n;
  long  *l;
  long  *o;
  float *d;
  long  *id;
  float *b_min;
  float *b_max;

  if(s.size()==0)
  {
    write_null_fof_list(fname);
    return;
  }

  //write the peak list to file
  if(!(fp_list = fopen(fname,"w")))
  {
    printf("Error opening %s.\n",fname);
    fflush(stdout);
  }

  n	= s.size();

  fwrite(&n,1,sizeof(int),fp_list);
  if(n==0)
  {
    fclose(fp_list);  
    return;
  }


  l	= (long  *) malloc(n*sizeof(long));
  o     = (long  *) malloc(n*sizeof(long));
  d     = (float *) malloc(n*sizeof(float));
  id    = (long  *) malloc(n*sizeof(long));
  b_min = (float *) malloc(3*n*sizeof(float));
  b_max = (float *) malloc(3*n*sizeof(float));

  for(int k=0;k<n;k++)
  {
    l[k]  = s[k].l;
    o[k]  = s[k].o;
    d[k]  = s[k].d;
    id[k] = s[k].id;

    for(int j=0;j<3;j++)
    {
      b_min[3*k+j] = s[k].min[j];
      b_max[3*k+j] = s[k].max[j];
    }
  }

  //read peak catalog for the B snapshot 
  fwrite(l,n,sizeof(long),fp_list);
  fwrite(o,n,sizeof(long),fp_list);
  fwrite(d,n,sizeof(float),fp_list);
  fwrite(id,n,sizeof(long),fp_list);
  fwrite(b_min,3*n,sizeof(float),fp_list);
  fwrite(b_max,3*n,sizeof(float),fp_list);

  //close the file
  fclose(fp_list);

  //free memory
  free(l);
  free(o);
  free(d);
  free(id);
  free(b_min);
  free(b_max);
}
