#ifndef BRANT_SPANNING_TREE_H
#define BRANT_SPANNING_TREE_H
#include "kdtree2.hpp"
#include "spanning_tree_data_types.hpp"

// define, for convenience a 2d array of floats. 
//
typedef multi_array<float,2> array2dfloat;

// k-D tree based on point data
extern kdtree2* tree;

// Point data
extern array2dfloat    data;

//connected graph
extern vector<edge> cgst;

//minimal spanning tree
extern vector<edge> mst;

/* \fn bool edge_equal_comparison(edge a, edge b)
 * \brief Are these the same edge?*/
bool edge_equal_comparison(edge a, edge b);

/* \fn bool edge_length_comparison(edge a, edge b)
 * \brief Is edge A shorter than edge B? */
bool edge_length_comparison(edge a, edge b);

/* \fn bool edge_peak_index_comparison(edge a, edge b)
 * \brief Is the peak index of A less than the peak index of B? */
bool edge_peak_index_comparison(edge a, edge b);

/* \fn bool vertex_index_comparison(vertex a, vertex b)
 * \brief Is the index of vertex A < index of vertex B? */
bool vertex_index_comparison(vertex a, vertex b);

/* \fn bool vertex_pid_comparison(vertex a, vertex b)
 * \brief Is the pid of vertex A < pid of vertex B? */
bool vertex_pid_comparison(vertex a, vertex b);

/* \fn float euclidean_distance( vector<float> a, vector<float> b)
 * \brief Find the euclidean distance between two vectors*/
float euclidean_distance( vector<float> a, vector<float> b);

/* \fn void construct_spanning_tree(array2dfloat vertex_data, long ntd, int dim, int nngb)
 * \brief Construct a connected graph that contains a spanning tree. */
void construct_spanning_tree(array2dfloat vertex_data, long ntd, int dim, int nngb);

/* \fn void initialize_spanning_tree(long ntd, int dim)
 * \brief Initialize the spanning tree construction routines. */
void initialize_spanning_tree(long ntd, int dim);

/* \fn void construct_minimum_spanning_tree(float bsq, long ntd)
 * \brief Construct the minimum spanning tree from a pre-built connected graph. */
void construct_minimum_spanning_tree(float bsq, long ntd);


/* \fn void produce_minimum_spanning_tree(array2dfloat data, long ntd, int dim, int nngb, float bsq)
 * \brief Produce a minimum spanning tree from the data locations */
void produce_minimum_spanning_tree(array2dfloat data, long ntd, int dim, int nngb, float bsq);


/* \fn void refine_fof_groups(long ntd)
   \brief Refine the FOF groups constructed from the MST */
void refine_fof_groups(long ntd);

/* \fn void destroy_spanning_tree(void)
 * \brief Destroy the spanning tree and associated data. */
void destroy_spanning_tree(void);

#endif /*BRANT_SPANNING_TREE_H*/
