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
	float bsq = 0.1;
	double t_a, t_b;

	char fdir_out[200];


	//load the particles
	n = load_particles(&tv);

	//float V = pow(3.0,3);
	//bsq = pow( V/n, 2./3.);
	bsq = 10.0;

	//print some info to screen
	printf("n = %ld, ntd = %ld, dim = %d, bsq = %e\n",n,tv.size(),dim,bsq);

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
	printf("Time for tree build = %es.\n",t_b-t_a);


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
