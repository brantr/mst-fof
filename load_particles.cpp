#include "load_particles.hpp"
#include "rng.h"
#include "rotation.hpp"

struct fake_shock
{
  int id;
  int n;
  float x[3];
  float theta[3];
  float Sigma;
  float d;
  float h;
  float dmax;
  int idmax;
};

long load_particles(vector<tracer> *t)
{
  return fake_particles(t);
}
long fake_particles(vector<tracer> *t)
{
  FILE *fp;
  char fname[200];
  long n_groups = 1;
  long n_per_group = 1000;
  long ntd = n_groups * n_per_group;
  fake_shock *fs;

  tracer t_in;

  long k=0;
  long kk=0;

  //float xc[3] = {-1.,1.,0.};
  //float yc[3] = {-1.,1.,1.};
  //float zc[3] = {-1.,1.,0.};
  double tx[3];
  float xc, yc, zc;
  float Sigma;
  float h;
  float theta;
  float r;
  float Mach = 5.0;
  double a,b,c;
  double *txr;
  float density;
  float dmax;
  int idmax;

  Rotation R;

  fs = (struct fake_shock *) malloc(n_groups*sizeof(fake_shock));
  for(long i=0;i<n_groups;i++)
  {

    a = 0.0;
    b = 0.0;
    c = 0.0;
    //a = rng_uniform(0.,360.);
    //b = rng_uniform(0.,360.);
    //c = rng_uniform(0.,360.);

    fs[i].theta[0] = a;
    fs[i].theta[1] = b;
    fs[i].theta[2] = c;


    //printf("a %e b %e c %e\n",a,b,c);
    R.SetAlpha(a);
    R.SetBeta(b);
    R.SetTheta(c);

    //R.ShowRx();
    //R.ShowRy();
    //R.ShowRz();

    //exit(0);

    xc = 0.5;
    yc = 0.5;
    zc = 0.5;
    //xc = rng_uniform(0,1.);
    //yc = rng_uniform(0,1.);
    //zc = rng_uniform(0,1.);

    fs[i].x[0] = xc;
    fs[i].x[1] = yc;
    fs[i].x[2] = zc;

    fs[i].id = i;
    fs[i].n  = n_per_group;

    density = 1.0 + rng_exponential(1.0);


    Sigma = 0.04 + 0.006*density;
    h = Sigma/density;
    fs[i].d = density;
    fs[i].h = h;
    fs[i].Sigma = Sigma;   

    dmax = 0.0;
    for(long j=0;j<n_per_group;j++)
    {
      tin.id = k;
      
      //tin.x[0] = xc[i]; 
      //tin.x[1] = yc[i]; 
      //tin.x[2] = zc[i];



      tin.v[0] = 0.0;
      tin.v[1] = 0.0;
      tin.v[2] = 0.0;



      tx[0] = rng_exponential(h);
      r = 0.1*rng_exponential(Mach*h); //norm to 1, such that 1./h * exp(-x/h)
      theta = rng_uniform(0.,2.*M_PI);
      tx[1] = r*cos(theta);
      tx[2] = r*sin(theta);

      tin.d = density*exp(-1.*r/(Mach*h))*exp(-1.0*tx[0]/h);

      if(tin.d>dmax)
      {
        dmax  = tin.d;
        idmax = k;
      }

      //rotate in 3-d
      txr = R.ApplyRotationZYX(tx);

      //printf("txr %e %e %e\n",txr[0],txr[1],txr[2]);


      //translate
      txr[0] += xc;
      txr[1] += yc;
      txr[2] += zc;
      for(kk=0;kk<3;kk++)
        tin.x[kk] = txr[kk]; 

      free(txr);

      //tin.x[0] += rng_exponential(h);
      //tin.x[0] += rng_gaussian(0.0,1.0);
      //tin.x[1] += rng_gaussian(0.0,1.0);
      //tin.x[2] += rng_gaussian(0.0,1.0);


      //data[i][k] = rng_gaussian(0.0,1.0);
      //data[i][k] = rng_levy(1.0);
      //data[i][k] = rng_levy(1.1);
      //data[i][k] = rng_tdist(500.0);
      //data[i][k] = rng_uniform(0.0,1.0);
      //data[i][k] = pow(data[i][k],6);

      //add to tracer list
      (*t).push_back(tin);

      //advance k
      k++;
    }
    fs[i].dmax = dmax;
    fs[i].idmax = idmax;
  }


      //save data to file
  sprintf(fname,"points.txt");
  fp = fopen(fname,"w");
  fprintf(fp,"%ld\n",ntd);
  for(long i=0;i<ntd;i++)
    fprintf(fp,"%e\t%e\t%e\n",(*t)[i].x[0],(*t)[i].x[1],(*t)[i].x[2]);
  fclose(fp);
  sprintf(fname,"points.dat");
  fp = fopen(fname,"w");
  fwrite(&ntd,1,sizeof(int),fp);
  for(long i=0;i<ntd;i++)
  {
    fwrite(&(*t)[i].x[0],1,sizeof(float),fp);
    fwrite(&(*t)[i].x[1],1,sizeof(float),fp);
    fwrite(&(*t)[i].x[2],1,sizeof(float),fp);

  }
  fclose(fp);


  sprintf(fname,"fake_shocks.dat");
  fp = fopen(fname,"w");
  fwrite(&n_groups,1,sizeof(int),fp);
  fwrite(&fs[0],n_groups,sizeof(fake_shock),fp);
  fclose(fp);

  free(fs);

  //return number of particles
  return ntd;
}
/*
{
  FILE *fp;
  char filename[200];   //name of file containing the tracers
  long n_tracers;       //number of tracers in the file
  float *d;             //tracer densities
  float *x;             //tracer x positions
  float *y;             //tracer y positions
  float *z;             //tracer z positions
  float *vx;            //tracer x velocities
  float *vy;            //tracer y velocities
  float *vz;            //tracer z velocities
  long  *l;             //tracer ids
  long ntd = 0;         //number of tracers above the density threshold
  AthenaHeader *h;

  //corners of the bounding box
  float t_min[3] = {1e9,1e9,1e9};
  float t_max[3] = {-1e9,-1e9,-1e9};


  //buffer for storing tracers into the tracer vector *t
  tracer t_in;
  
  if(isub==0)
  {
    //create a new filename
    sprintf(filename,"%s/%s.%04d.tra",fdir,filebase,isnap);
  }else{
    //create a new filename
    sprintf(filename,"%s/%s-id%d.%04d.tra",fdir,filebase,isub,isnap);
  }

  //open tracer file
  if(!(fp = fopen(filename,"r")))
  {
    printf("Error opening %s in load tracers (fdir=%s, filebase=%s, fdout=%s.\n",filename,fdir,filebase,fdir_out);
    fflush(stdout); 
  }

  // Read header 
  h = ReadAthenaHeader(fp);

  //ShowAthenaHeader(h);


  // read the number of tracer in this file 
  fread(&n_tracers,1,sizeof(long),fp);

  // Allocate buffer 
  if(!(d = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property buf.\n");
    fflush(stdout);
    exit(-1);
  }

  // Allocate buffer 
  if(!(x = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property buf.\n");
    fflush(stdout);
    exit(-1);
  }

  // Allocate buffer 
  if(!(y = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property y.\n");
    fflush(stdout);
    exit(-1);
  }

  // Allocate buffer 
  if(!(z = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property z.\n");
    fflush(stdout);
    exit(-1);
  }

  // Allocate buffer 
  if(!(vx = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property z.\n");
    fflush(stdout);
    exit(-1);
  }

    // Allocate buffer 
    if(!(vy = (float *)malloc(n_tracers*sizeof(float))))
    {
      printf("Error allocating tracer property z.\n");
      fflush(stdout);
      exit(-1);
    }


    // Allocate buffer 
    if(!(vz = (float *)malloc(n_tracers*sizeof(float))))
    {
      printf("Error allocating tracer property z.\n");
      fflush(stdout);
      exit(-1);
    }


    // read density 
    fread(d,n_tracers,sizeof(float),fp);

    // read M1 
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vx[tt] = x[tt]/d[tt];
  
    // read M2 
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vy[tt] = x[tt]/d[tt];

    // read M3 
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vz[tt] = x[tt]/d[tt];

#ifndef BAROTROPIC
    // read E 
    fread(x,n_tracers,sizeof(float),fp);
#endif // BAROTROPIC 

#ifdef MHD
    // read B1c 
    fread(x,n_tracers,sizeof(float),fp);

    // read B2c 
    fread(x,n_tracers,sizeof(float),fp);

    // read B3c 
    fread(x,n_tracers,sizeof(float),fp);
#endif //MHD

#if (NSCALARS > 0)
    for(k=0;k<NSCALARS;k++)
      fread(x,n_tracers,sizeof(float),fp);
#endif

    // read x1 
    fread(x,n_tracers,sizeof(float),fp);
  
    // read x2 
    fread(y,n_tracers,sizeof(float),fp);

    // read x3 
    fread(z,n_tracers,sizeof(float),fp);  

    // Allocate buffer 
    if(!(l = (long *)malloc(n_tracers*sizeof(long))))
    {
      printf("Error allocating tracer property buf.\n");
      fflush(stdout);
    }

    // read id 
    fread(l,n_tracers,sizeof(long),fp);

    // close tracer file
    fclose(fp);


    // keep only particles with densities above or = threshold
    ntd = 0;
    for(long tt=0;tt<n_tracers;tt++)
    {

      if( x[tt] < t_min[0])
        t_min[0] = x[tt];
      if( y[tt] < t_min[1])
        t_min[1] = y[tt];
      if( z[tt] < t_min[2])
        t_min[2] = z[tt];

      if( x[tt] > t_max[0])
        t_max[0] = x[tt];
      if( y[tt] > t_max[1])
        t_max[1] = y[tt];
      if( z[tt] > t_max[2])
        t_max[2] = z[tt];

      //if particle is above the threshold, keep it
      if(d[tt]>=dthresh)
      {  
        tin.id = l[tt];
        tin.d = d[tt];
        tin.x[0] = x[tt];
        tin.x[1] = y[tt];
        tin.x[2] = z[tt];
        tin.v[0] = vx[tt];
        tin.v[1] = vy[tt];
        tin.v[2] = vz[tt];

        //add to tracer list
        (*t).push_back(tin);

        //remember that we've kept a particle
        ntd++;
      }
    }

    // free buffer memory
    free(d);
    free(x);
    free(y);
    free(z);
    free(vx);
    free(vy);
    free(vz);
    free(l);


  // free header
  free(h);


  //write min and max information to output directory
  sprintf(filename,"%s/range.%04d.%04d.txt",fdir_out,isnap,isub);
  if(!(fp = fopen(filename,"w")))
  {
    printf("Error opening %s.\n",filename);
    fflush(stdout);
  }
  fprintf(fp,"%e\t%e\t%e\n",t_min[0],t_min[1],t_min[2]);
  fprintf(fp,"%e\t%e\t%e\n",t_max[0],t_max[1],t_max[2]);
  fclose(fp);

  //return number of tracers > density
  return ntd;

}
*/