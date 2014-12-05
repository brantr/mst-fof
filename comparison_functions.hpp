#ifndef COMPARISON_FUNCTIONS_H
#define COMPARISON_FUNCTIONS_H

#include"shock_data_types.hpp"	/*tracer, merger data types*/

/*compare tracer ids*/
bool tracer_id_comparison(tracer a, tracer b);


/*compare tracer densities*/
bool tracer_density_comparison(tracer a, tracer b);


/*compare shock densities*/
bool merger_density_comparison(merger a, merger b);

/*compare shock densities, A then B*/
//bool mbcomp(merger a, merger b);
bool merger_joint_density_comparison(merger a, merger b);


/*compare shock densities, A then B, in reverse*/
//bool mbrcomp(merger a, merger b);
bool merger_reverse_joint_density_comparison(merger a, merger b);

/*compare A and B snap densities*/
//bool mmaxcomp(merger a, merger b);
bool merger_maximum_density_comparison(merger a, merger b);



/*compare snap, then index, then density*/
//bool sicomp(merger a, merger b);
bool merger_joint_snapshot_index_density_comparison(merger a, merger b);


/*compare snap, then density, then order*/
//bool scomp(merger a, merger b);
bool merger_joint_snapshot_density_comparison(merger a, merger b);


/*sort by peak A density*/
//bool dacomp(merger a, merger b);
bool merger_A_density_comparison(merger a, merger b);

/*sort by B snap, then B index, then A snap, then A index*/
//bool sacomp(merger a, merger b);
bool merger_joint_B_index_A_index_comparison(merger a, merger b);

/*reflected sort by B snap, then B index, then A snap, then A index*/
//bool rsacomp(merger a, merger b);
bool merger_joint_snapshot_index_id_comparison(merger a, merger b);


/*compare shock indices*/
//bool micomp(merger a, merger b);
bool merger_id_comparison(merger a, merger b);

/*compare shock peak indicies*/
//bool peak_index_comparison(tracer a, tracer b);
bool tracer_peak_index_comparison(tracer a, tracer b);

/*is this the same snap?*/
//bool same_snap(merger m);
bool merger_snapshot_equality(merger m);

/*compare shock indices, then B density*/
//bool macomp(merger a, merger b);
bool merger_index_B_density_comparison(merger a, merger b);

/*are these the same merger?*/
//bool ucomp(merger a, merger b);
bool merger_unique_equality(merger a, merger b);

/*are these the reflected mergers?*/
//bool rucomp(merger a, merger b);
bool merger_reflected_equality(merger a, merger b);

#endif /*COMPARISON_FUNCTIONS_H*/
