#include <stdio.h>
#include "write_spanning_tree.hpp"

/* \fn void write_spanning_tree(char fname[200], vector<edge> st)
 * \brief Work routine for writing spanning trees */
void write_spanning_tree(char fname[200], vector<edge> st)
{
  long n_edge;      //number of edges in ST
  FILE *fp;         //file pointer
  float x[3];       //float buffer for pos
  float dis;        //float buffer for distances
  long  eid;        //long buffer for edge index


  //open the file for writing
  fp = fopen(fname,"w");

  //remember the number of edges in the ST
  n_edge = st.size();

  //printf("st.size() = %ld\n",n_edge);

  //write the number of edges in the tree to file
  fwrite(&n_edge,1,sizeof(long),fp);

  if(n_edge==0)
  {
  	//exit
  	fclose(fp);
  	return;
  }

  //write the edge A vertex positions from the global tracer array tv
  for(long i=0;i<n_edge;i++)
  {
    for(int k=0;k<3;k++)
      x[k] = data[st[i].idx_A][k];
    fwrite( &x[0], 3, sizeof(float), fp);
  }

  //write the edge B vertex positions from the global tracer array tv
  for(long i=0;i<n_edge;i++)
  {
    for(int k=0;k<3;k++)
      x[k] = data[st[i].idx_B][k];
    fwrite( &x[0], 3, sizeof(float), fp);
  }

  //write the edge euclidean lengths to file
  for(long i=0;i<n_edge;i++)
  {
    dis = st[i].dis;
    fwrite( &dis, 1, sizeof(float), fp);
  }

  //write the edge peak indices to file
  for(long i=0;i<n_edge;i++)
  {
    eid = st[i].peak_index;
    fwrite( &eid, 1, sizeof(long), fp);
  }

  //write the A vertex index to file
  for(long i=0;i<n_edge;i++)
  {
    eid = st[i].idx_A;
    fwrite( &eid, 1, sizeof(long), fp);
  }

  //write the B vertex index to file
  for(long i=0;i<n_edge;i++)
  {
    eid = st[i].idx_B;
    fwrite( &eid, 1, sizeof(long), fp);
  }

  //close the file
  fclose(fp);
}