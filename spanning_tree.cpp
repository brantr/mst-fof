
#include "spanning_tree.hpp"
#include "kdtree2.hpp"
#include "shock_data_types.hpp"

int *flag;										//flags for tracers

vector<edge> cgst;            //connected graph; a vector of edges
vector<edge> mst;             //mst of edges
vector<edge>::iterator eui;   //iterator for edge vectors

struct vertex_linked_list *vll;		//vertex linked list array

int initialize_spanning_tree_flag=0;


//kdtree for neighbor searching
//kdtree2* tree;

//data for kdtree
//array2dfloat    data;

/* \fn void produce_minimum_spanning_tree(array2dfloat data, long ntd, int dim, int nngb, float bsq)
 * \brief Produce a minimum spanning tree from the data locations */
void produce_minimum_spanning_tree(array2dfloat data, long ntd, int dim, int nngb, float bsq)
{
  initialize_spanning_tree(ntd,dim);
  construct_spanning_tree(data,ntd,dim,nngb);
  construct_minimum_spanning_tree(bsq,ntd);
}

/* \fn void initialize_spanning_tree(long ntd, int dim)
 * \brief Initialize the spanning tree construction routines. */
void initialize_spanning_tree(long ntd, int dim)
{
	//allocate linked list
	vll = (struct vertex_linked_list *) malloc( ntd * sizeof(struct vertex_linked_list));

	for(long tt = ntd-1; tt>=0; tt--)
	{
	  //initialize vertex linked list
    vll[tt].prev = NULL;
    vll[tt].next = NULL;
    vll[tt].head = &vll[tt];
    vll[tt].tail = NULL;
    vll[tt].pid  = tt;
  }


  //allocate tracer flags and initialize to 0
  flag   = (int *) calloc( ntd, sizeof(int));

  //remember initialization
  initialize_spanning_tree_flag=1337;

  //printf("ntd = %ld\n",ntd);

}

/* \fn void construct_spanning_tree(array2dfloat vertex_data, long ntd, int dim, int nngb)
 * \brief Construct a connected graph that contains a spanning tree. */
void construct_spanning_tree(array2dfloat vertex_data, long ntd, int dim, int nngb)
{

	//3d position
  vector<float> xc(dim);
  vector<float> xa(dim);

  //result from KD tree search
  kdtree2_result_vector	res;

	//buffer for edge insertion to spanning tree
	edge e_in;

  //ensure spanning tree has been initialized
  if(!initialize_spanning_tree_flag)
    initialize_spanning_tree(ntd,dim);


  //place tracers in FOF catalogues
  for(long tt = ntd-1; tt>=0; tt--)
  {
    //current particle position
    for(int k=0;k<dim;k++)
      xc[k] = tv[tt].x[k];
    	//xc[k] = vertex_data[tt][k];
 

    //find nearest neighbors
    tree->n_nearest(xc, nngb, res);

    //add nngb-1 shortest edges to ST
    for(int i=1;i<res.size();i++)
    {

      // the smaller index is stored in idx_A
      // the larger index is stored in idx_B
      if(tt<res[i].idx)
      {
        e_in.idx_A = tt;
        e_in.idx_B = res[i].idx;
      }else{
        e_in.idx_A = res[i].idx;
        e_in.idx_B = tt;
      }

      for(int k=0;k<dim;k++)
      {
        xc[k] = vertex_data[res[i].idx][k];
        xa[k] = vertex_data[tt][k];
      }

      //store the euclidean distance between tracers to find the edge length
      e_in.dis   = euclidean_distance( xc, xa);


      //add the edge to the ST
      cgst.push_back(e_in);

    }

    //initialize tracer peak index to -1
    //already done
    //tv[tt].peak_index = -1;


  }

  //first, sort edges by length
  sort( cgst.begin(), cgst.end(), edge_length_comparison); 
    
  //retain only unique edges (AB vs. BA reflections will be stored above*/
  eui = unique(cgst.begin(), cgst.end(), edge_equal_comparison);
  cgst.resize(std::distance(cgst.begin(),eui));
}

/* \fn void construct_minimum_spanning_tree(float bsq, long ntd)
 * \brief Construct the minimum spanning tree from a pre-built connected graph. */
void construct_minimum_spanning_tree(float bsq, long ntd)
{
	//make the mst
  long pid = 1; 

  //buffer for edge insertion to spanning tree
  edge e_in;

  //linked list of vertices
  struct vertex_linked_list *vl;


  for(long tt=0;tt<cgst.size();tt++) //loop over all edges
  {
    if(cgst[tt].dis < bsq)           //if edge length is < linking length^2
    {

      if(cgst[tt].idx_A >= ntd || cgst[tt].idx_B >= ntd)
      {
        printf("ERROR\n");  
        fflush(stdout);
        exit(-1);
      }

      //first condition -- if either vertex has been added */
      if( flag[cgst[tt].idx_A] || flag[cgst[tt].idx_B] )
      {

        //second condition -- have both vertices been added? */
        if( flag[cgst[tt].idx_A] && flag[cgst[tt].idx_B] )
        {
          //we have to make sure we are not making a loop
          if( vll[cgst[tt].idx_A].head->pid != vll[cgst[tt].idx_B].head->pid )
          {
            //flag_A != flag_B, so we need to meld groups
            e_in = cgst[tt];

            //we will adopt the id of the first group
            if( vll[cgst[tt].idx_A].head->pid < vll[cgst[tt].idx_B].head->pid )
            {
              //pidA < pidB, so adopt pidA
              flag[cgst[tt].idx_B]  = flag[cgst[tt].idx_A];
              e_in.peak_index    = vll[cgst[tt].idx_A].head->pid;

              //need to add B to the end of A
              vll[cgst[tt].idx_B].head->prev       = vll[cgst[tt].idx_A].head->tail;  //B's head's prev == tail of A
              vll[cgst[tt].idx_A].head->tail->next = vll[cgst[tt].idx_B].head;        //A's tail's next == head of B
              vll[cgst[tt].idx_A].head->tail       = vll[cgst[tt].idx_B].head->tail;  //Tail of combined A+B == B's tail 

              //inform B of new head and new pid
              for(vl = vll[cgst[tt].idx_B].head; vl!=NULL; vl=vl->next)
              {
                vl->head = vll[cgst[tt].idx_A].head;
                vl->pid  = vll[cgst[tt].idx_A].pid;
              }

            }else{

              //pidB < pidA, so adopt pidB
              flag[cgst[tt].idx_A]  = flag[cgst[tt].idx_B];
              e_in.peak_index   = vll[cgst[tt].idx_B].head->pid;

              //need to add A to the end of B
              vll[cgst[tt].idx_A].head->prev       = vll[cgst[tt].idx_B].head->tail;  //A's head's prev == tail of B
              vll[cgst[tt].idx_B].head->tail->next = vll[cgst[tt].idx_A].head;        //B's tail's next == head of A
              vll[cgst[tt].idx_B].head->tail       = vll[cgst[tt].idx_A].head->tail;  //Tail of combined B+A == A's tail

              //inform A of new head and new pid
              for(vl = vll[cgst[tt].idx_A].head; vl!=NULL; vl=vl->next)
              {
                vl->head = vll[cgst[tt].idx_B].head;
                vl->pid  = vll[cgst[tt].idx_B].pid;
              }
            }

            //OK, the edge is ready to insert

            //add the edge
            mst.push_back(e_in);
          }

        }else{ //one or the other have been added so we can add indiscriminately

          //copy the edge
          e_in = cgst[tt];

          if(flag[cgst[tt].idx_A]) //B is the new vertex
          {
            //assign edge and vertex pid to A's peak
            e_in.peak_index = vll[cgst[tt].idx_A].head->pid;

            //the new vertex is B
            flag[cgst[tt].idx_B] = e_in.peak_index;

            //add B to linked list
            vll[cgst[tt].idx_B].head = vll[cgst[tt].idx_A].head;
            vll[cgst[tt].idx_B].prev = vll[cgst[tt].idx_A].head->tail;
            vll[cgst[tt].idx_B].head->tail->next = &vll[cgst[tt].idx_B]; //have end of linked list point to B
            vll[cgst[tt].idx_B].head->tail = &vll[cgst[tt].idx_B];       //reset the tail to be B

            vll[cgst[tt].idx_B].idx = cgst[tt].idx_B;
            vll[cgst[tt].idx_B].pid = vll[cgst[tt].idx_B].head->pid;
          
          }else{  //A is the new vertex

            //assign edge and vertex pid to B's peak
            e_in.peak_index = vll[cgst[tt].idx_B].head->pid;

            //the new vertex is A
            flag[cgst[tt].idx_A] = e_in.peak_index;

            //add A to linked list
            vll[cgst[tt].idx_A].head = vll[cgst[tt].idx_B].head;
            vll[cgst[tt].idx_A].prev = vll[cgst[tt].idx_B].head->tail;
            vll[cgst[tt].idx_A].head->tail->next = &vll[cgst[tt].idx_A]; //have end of linked list point to B
            vll[cgst[tt].idx_A].head->tail = &vll[cgst[tt].idx_A];       //reset the tail to be B

            vll[cgst[tt].idx_A].idx = cgst[tt].idx_A;
            vll[cgst[tt].idx_A].pid = vll[cgst[tt].idx_A].head->pid;
          }
          
          //add the edge
          mst.push_back(e_in);
        }

      }else{  //neither vertex has been added, so make a new group
        
        //make the new edge with an appropriate peak id 
        e_in = cgst[tt];
        e_in.peak_index = pid;

        //add the edge to the mst 	
        mst.push_back(e_in);

        //mark these vertices as being added
        flag[cgst[tt].idx_A] = pid;
        flag[cgst[tt].idx_B] = pid;

        //create new vertex insertions to the linked list
        if( cgst[tt].idx_A < cgst[tt].idx_B )
        {
          vll[cgst[tt].idx_A].head = &vll[cgst[tt].idx_A];  //A is the head of the new group
          vll[cgst[tt].idx_A].next = &vll[cgst[tt].idx_B];  //B follows A
          vll[cgst[tt].idx_B].head = &vll[cgst[tt].idx_A];  //A is the head of the new group
          vll[cgst[tt].idx_B].prev = &vll[cgst[tt].idx_A];  //B follows A

        }else{
          vll[cgst[tt].idx_B].head = &vll[cgst[tt].idx_B];  //B is the head of the new group
          vll[cgst[tt].idx_B].next = &vll[cgst[tt].idx_A];  //a follows B
          vll[cgst[tt].idx_A].head = &vll[cgst[tt].idx_B];  //B is the head of the new group
          vll[cgst[tt].idx_A].prev = &vll[cgst[tt].idx_B];  //A follows B
        }
        vll[cgst[tt].idx_A].idx = cgst[tt].idx_A;
        vll[cgst[tt].idx_B].idx = cgst[tt].idx_B;
        vll[cgst[tt].idx_A].pid = pid;
        vll[cgst[tt].idx_B].pid = pid;
        vll[cgst[tt].idx_A].head->tail = &vll[cgst[tt].idx_B];	//set the tail

        //make a new group
        pid++;
      }
    }
  }

  //need to make fof groups from the mst vector of edges by limiting edge lengths

  //First, sort by edge length.  Needed to set PIDs correctly.
  sort( mst.begin(), mst.end(), edge_length_comparison);

  //we need to correct the MST peak ids
	for(long tt=0; tt<mst.size(); tt++)
  	mst[tt].peak_index = vll[mst[tt].idx_A].head->pid;

}

/* \fn void refine_fof_groups(long ntd)
   \brief Refine the FOF groups constructed from the MST */
void refine_fof_groups(long ntd)
{
  long  pid;       //peak index
  float dmax  = 0; //highest density in group
  long  idmax = 0; //index of particle with largest density

  //if the MST is null, then we don't need to refine anything
  if(mst.size()==0)
    return;

  //first, sort by increasing peak index
  sort( mst.begin(), mst.end(), edge_peak_index_comparison); 

  //set pid to the lowest peak id
  pid = mst[0].peak_index;

  //loop over all tracers in the st
  for(long tt=0; tt<=mst.size(); tt++)
  { 
    
    //if we're at the end of the mst
    if(tt==mst.size())
    {
      //remember the peak id of the densest tracer in the tree
      vll[mst[tt-1].idx_A].head->pid = idmax;
      mst[tt-1].peak_index = idmax;
      break;
    }else{

      //if we've found a tracer that's in a new group
      if(mst[tt].peak_index != pid && tt>0)
      {
        //record the current peak index
        vll[mst[tt-1].idx_A].head->pid = idmax;
        mst[tt-1].peak_index           = idmax;

        dmax  = 0;                      //reset max density for new group
        pid   = mst[tt].peak_index;     //remember peak_index of new group

      }

      //previously, the following was an else
      //but in the case that the first edge of the 
      //new group contained the most dense tracer, then
      //it's not clear that was always accounted for.
      //This now explicitly checks in all cases whether
      //vertex A and B contain the densest particle
      //in the group

      //if the tracer at the A vertex has the highest density
      if( tv[vll[mst[tt].idx_A].idx].d > dmax )
      {
        //remember its density and index
        dmax  = tv[vll[mst[tt].idx_A].idx].d;
        idmax = vll[mst[tt].idx_A].idx;
      }

      //if the tracer at the B vertex has the highest density
      if( tv[vll[mst[tt].idx_B].idx].d > dmax )
      {
        //remember its density and index
        dmax  = tv[vll[mst[tt].idx_B].idx].d;
        idmax = vll[mst[tt].idx_B].idx;
      }
    }// tt = mst.size()
  }// for 0<=tt<=mst.size()

  //we need to correct the MST peak ids
  for(long tt=0; tt<mst.size(); tt++)
  {
    //check that head is allocated
    if(vll[mst[tt].idx_A].head == NULL)
    { 
      printf("ERROR: head not allocated for idxA = %ld\n",mst[tt].idx_A);
      fflush(stdout);
    }

    //check that the peak ids are set properly
    if(vll[mst[tt].idx_A].head->pid < 0 || vll[mst[tt].idx_A].head->pid >= ntd)
    {
      printf("ERROR: assigning pid idxA = %ld head->pid = %ld\n",mst[tt].idx_A,vll[mst[tt].idx_A].head->pid);
      fflush(stdout);
    }

    //set the peak index of this member
    mst[tt].peak_index = tv[vll[mst[tt].idx_A].head->pid].id;
  } 

  ////set particle pids to head list pid
  for(long tt=0;tt<ntd;tt++)  
  {
    if(vll[tt].head == NULL)
    { 
      printf("head not allocated for tt = %ld\n",tt);
      fflush(stdout);
    }

    //set the peak index for the tracer
    tv[tt].peak_index = vll[tt].head->pid;
  }

  //should have an mst ordered by edge length
  sort( mst.begin(), mst.end(), edge_length_comparison); 
}

/* \fn void destroy_spanning_tree(void)
 * \brief Destroy the spanning tree and associated data. */
void destroy_spanning_tree(void)
{

  //free the vertex linked list
  free(vll);

  //free the flags associated with data points
  free(flag);

  //clear the mst and de-allocate memory
  mst.clear();
  vector<edge>().swap(mst);

  //clear the edges and de-allocate memory
  cgst.clear();
  vector<edge>().swap(cgst);

  //reset the initialization flag
  initialize_spanning_tree_flag=0;
}

/* \fn bool edge_equal_comparison(edge a, edge b)
 * \brief Are these the same edge?*/
bool edge_equal_comparison(edge a, edge b)
{
  return ( (a.idx_A == b.idx_A)&&(a.idx_B == b.idx_B) );
}

/* \fn bool edge_length_comparison(edge a, edge b)
 * \brief Is edge A shorter than edge B? */
bool edge_length_comparison(edge a, edge b)
{
  return (a.dis < b.dis);
}

/* \fn bool edge_peak_index_comparison(edge a, edge b)
 * \brief Is the peak index of A less than the peak index of B? */
bool edge_peak_index_comparison(edge a, edge b)
{
  return (a.peak_index < b.peak_index);
}

/* \fn bool vertex_index_comparison(vertex a, vertex b)
 * \brief Is the index of vertex A < index of vertex B? */
bool vertex_index_comparison(vertex a, vertex b)
{
  return (a.idx < b.idx);
}

/* \fn bool vertex_pid_comparison(vertex a, vertex b)
 * \brief Is the pid of vertex A < pid of vertex B? */
bool vertex_pid_comparison(vertex a, vertex b)
{
  return (a.pid < b.pid);
}


/* \fn float euclidean_distance( vector<float> a, vector<float> b)
 * \brief Find the euclidean distance between two vectors*/
float euclidean_distance( vector<float> a, vector<float> b)
{
  float d = 0;
  for(int k=0;k<a.size();k++)
    d += (a[k]-b[k])*(a[k]-b[k]);
  return d;
}
