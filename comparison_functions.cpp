#include <math.h>
#include "comparison_functions.hpp"


/*compare tracer ids*/
bool tracer_id_comparison(tracer a, tracer b)
{
  return (a.id<b.id);
}

/*compare tracer densities*/
bool tracer_density_comparison(tracer a, tracer b)
{
  return (a.d<b.d);
}

/*compare shock densities*/
bool merger_density_comparison(merger a, merger b)
{
  if(a.d_B==b.d_B)
    return (a.d_A>b.d_A);

  return (a.d_B>b.d_B);

}

/*sort by dA, then by dB*/
bool merger_joint_density_comparison(merger a, merger b)
{
  if(a.d_A==b.d_A)
    return (a.d_B < b.d_B);

  return (a.d_A < b.d_A);
}

/*sort by dA, then by dB, reverse*/
bool merger_reverse_joint_density_comparison(merger a, merger b)
{
  if(a.d_A==b.d_A)
    return (a.d_B > b.d_B);

  return (a.d_A > b.d_A);
}

/*compare based on density of A and B*/
bool merger_maximum_density_comparison(merger a, merger b)
{
  float dA = fminf(a.d_A, a.d_B);
  float dB = fminf(b.d_A, b.d_B);

  return (dA<dB);
}

/*compare snap, then index, then density*/
bool merger_joint_snapshot_index_density_comparison(merger a, merger b)
{
  if(a.snap_A==b.snap_A)
  {
    if(a.i_A == b.i_A)
    {
      return (a.d_B>b.d_B);
    }else{
      return (a.i_A<b.i_A);
    }
  }

  return (a.snap_A<b.snap_A);

}

/*compare snap, then density, then order*/
bool merger_joint_snapshot_density_comparison(merger a, merger b)
{
  if(a.snap_A==b.snap_A)
  {
    if(a.d_B==b.d_B)
      return (a.i_A<b.i_A);

    return (a.d_B>b.d_B);
  }

  return (a.snap_A<b.snap_A);

}

/*sort by peak A density*/
bool merger_A_density_comparison(merger a, merger b)
{
  return (a.d_A < b.d_A);
}

/*sort by B snap, then B index, then A snap, then A index*/
bool merger_joint_B_index_A_index_comparison(merger a, merger b)
{
  if(a.snap_B==b.snap_B)
  {
    if(a.i_B==b.i_B)
    {
      if(a.snap_A==b.snap_A)
	return (a.i_A < b.i_A);
  
      return (a.snap_A < b.snap_A);
    }
    return(a.i_B<b.i_B);
  }

  return (a.snap_B < b.snap_B);
}

/*reflected sort by B snap, then B index, then A snap, then A index*/
bool merger_joint_snapshot_index_id_comparison(merger a, merger b)
{
  int check_A1, check_B1;
  int check_A2, check_B2;
  int check_A3, check_B3;

  check_A1 = ( (a.snap_A+1) * (a.snap_B+1) );
  check_B1 = ( (b.snap_A+1) * (b.snap_B+1) );

  check_A2 = ( (a.i_A+1) * (a.i_B+1) );
  check_B2 = ( (b.i_A+1) * (b.i_B+1) );

  check_A3 = a.id_A + a.id_B;
  check_B3 = b.id_A + b.id_B;

  if(check_A1 == check_B1)
  {
    if(check_A2 == check_B2)
    {
      if(check_A3 == check_B3)
      {
	return (a.snap_A < b.snap_A);
      }
      return (check_A3 < check_B3);
    }
    return (check_A2 < check_B2);
  }

  return (check_A1 < check_B1);
}

/*compare shock indices*/
bool merger_id_comparison(merger a, merger b)
{
  return (a.id_B<b.id_B);
}


/*compare tracer peak indicies*/
bool tracer_peak_index_comparison(tracer a, tracer b)
{
  return (a.peak_index>b.peak_index);
}

/*are these the same snapshot?*/
bool merger_snapshot_equality(merger m)
{
  return (m.snap_A==m.snap_B);
}

/*compare shock indices, then B density*/
bool merger_index_B_density_comparison(merger a, merger b)
{
  if(a.i_A==b.i_A)
    return(a.d_B>b.d_B);
  return (a.i_A<b.i_A);
}

/*are these the same merger?*/
bool merger_unique_equality(merger a, merger b)
{
  return( (a.snap_A==b.snap_A)&&(a.id_A==b.id_A)&&(a.snap_B==b.snap_B)&&(a.id_B==b.id_B) );
}

/*are these reflected mergers?*/
bool merger_reflected_equality(merger a, merger b)
{
  return( (a.snap_A==b.snap_B)&&(a.id_A==b.id_B)&&(a.snap_B==b.snap_A)&&(a.id_B==b.id_A) );
}
