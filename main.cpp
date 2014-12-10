#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_sort.h>
#include "spanning_tree.hpp"
#include "write_spanning_tree.hpp"
#include "load_particles.hpp"
#include "comparison_functions.hpp"
#include "assign_peak.hpp"
#include "construct_fof_catalogue.hpp"
#include "rng.h"
#include "timer.h"

std::vector<tracer> tv; /* tracers read in from file */
std::vector<tracer> td; /* tracers sorted by density */
tracer tin;             /* buffer for adding tracers to tracer vectors */

kdtree2          *tree; /* global kdtree2 declaration */
array2dfloat     data;  /* global data elements for kdtree2 */


int main(int argc, char **argv)
{
	long n;
	int dim = 3;
	int nngb = 20;
	//int nngb = 100;
	float bsq = 1.0e-4;
	double t_a, t_b;
  long n_groups = 100;
  long n_per_group = 1000;

	char fdir_out[200];

  //adjust bsq if argc==2
  if(argc>=2)
    bsq = pow(atof(argv[1]),2);

  printf("%15.14e\n",bsq);

  //load groups
  if(argc>=3)
    n_groups = atol(argv[2]);
  if(argc>=4)
    n_per_group = atol(argv[3]);

	//load the particles
	n = load_particles(&tv,n_groups,n_per_group);

	//print some info to screen
	//printf("n = %ld, ntd = %ld, dim = %d, bsq = %e\n",n,tv.size(),dim,bsq);

	/*sort tracer particles by density*/ 
  std::sort( tv.begin(), tv.end(), tracer_density_comparison);

	//make data
	data.resize(extents[n][dim]);
	for(long i=0;i<n;i++)
		for(int k=0;k<dim;k++)
    	data[i][k] = tv[i].x[k];


	//build the kdtree
	t_a = timer();
	tree = new kdtree2(data,true);
	t_b = timer();
	//printf("Time for tree build = %es.\n",t_b-t_a);


	//define output directory
	sprintf(fdir_out,"./");

	//find fof groups from minimal spanning tree
	find_fof_mst(data, bsq, 0.0, fdir_out, 0, 0);

	//construct fof catalog from fof groups
	construct_fof_catalogue(n);


	/*delete the tree*/
	delete tree;

	//free tracer particle vector
	data.resize(extents[1][1]);

	//end program
	return 0;
}
