#ifndef ASSIGN_PEAK_H
#define ASSIGN_PEAK_H

#include "kdtree2.hpp"
#include "shock_data_types.hpp"

//find FOF groups using minimum spanning tree
void find_fof_mst(array2dfloat vertex_data, float b, float dthresh, char fdir_out[], int isnap, int isub);

//write a spanning tree to file
void write_st(char fdir_out[], int isub, vector<edge> st);

//write a minimum spanning tree to file
void write_mst(char fdir_out[], int isub, vector<edge> mst);


#endif /*ASSIGN_PEAK_H*/
