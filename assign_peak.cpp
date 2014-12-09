#include <vector>
#include <gsl/gsl_sort_int.h>
#include "kdtree2.hpp"
#include "assign_peak.hpp"
#include "shock_data_types.hpp"
#include "spanning_tree.hpp"
#include "spanning_tree_data_types.hpp"
#include "write_spanning_tree.hpp"

//make a set of minimal spanning trees based on FOF catalogs
void find_fof_mst(array2dfloat vertex_data, float bsq, float dthresh, char fdir_out[], int isnap, int isub)
{
  int dim = 3;            //number of dimensions
  long ntd = tv.size();   //number of tracers greater than the density threshold

  int nngb = 20;


  //construct the spanning tree

  construct_spanning_tree(vertex_data, ntd, dim, nngb);

  //write the ST to file
  write_st(fdir_out, isub, cgst); 


  //OK, we have constructed a spanning tree, but we 
  //need to revise to mst

  //make the mst

  construct_minimum_spanning_tree(bsq, ntd);


  //refine the FOF groups, computing the maximum density
  //in each group, and setting the group peak to the 
  //densest particle

  refine_fof_groups(ntd);

  //Write to the MST file

  write_mst(fdir_out, isub, mst);

  //free some memory
  
  destroy_spanning_tree();

  //matches find_fof_mst
}

//write a spanning tree to file
void write_st(char fdir_out[], int isub, vector<edge> st)
{

	char fname[200];	//filename to write

	//Make the ST filename
  sprintf(fname,"%s/st.%04d.dat",fdir_out,isub);

  //write the spanning tree
  write_spanning_tree(fname, st);

}

//write a minimum spanning tree to file
void write_mst(char fdir_out[], int isub, vector<edge> mst)
{

	char fname[200];	//filename to write

	//Make the MST filename
  sprintf(fname,"%s/mst.%04d.dat",fdir_out,isub);

  //write the spanning tree
  write_spanning_tree(fname, mst);

}
