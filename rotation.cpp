#include <math.h>
#include "rotation.h"
#include "routines.h"

void Rotation::Rotation(void)
{
	ndim  = 3;
	alpha = 0;
	beta  = 0;
	theta = 0;

	R_x = two_dimensional_array(ndim,ndim);
	R_y = two_dimensional_array(ndim,ndim);
	R_z = two_dimensional_array(ndim,ndim);
}
void Rotation::Rotation(double alpha_in,double beta_in,double theta_in)
{
	ndim = 3;
	SetAlpha(alpha_in);
	SetBeta(beta_in);
	SetTheta(theta_in);

	R_x = two_dimensional_array(ndim,ndim);
	R_y = two_dimensional_array(ndim,ndim);
	R_z = two_dimensional_array(ndim,ndim);

	SetRx();
	SetRy();
	SetRz();
}
void Rotation::~Rotation(void)
{
	deallocate_two_dimensional_array(R_x);
	deallocate_two_dimensional_array(R_y);
	deallocate_two_dimensional_array(R_z);
}
void Rotation::SetAlpha(double alpha_in)
{
	alpha = SetAngle(alpha_in);
	SetRx();
}
void Rotation::SetBeta(double beta_in)
{
	beta = SetAngle(beta_in);
	SetRy();
}
void Rotation::SetTheta(double beta_in)
{
	theta = SetAngle(theta_in);
	SetRz();
}
double Rotation::SetAngle(double angle_in_degrees)
{
	return M_PI*angle_in_degrees/180.0;
}
double **Rotation::ApplyRotation(double **R, double **x, int n)
{
	double **xr = two_dimensional_array(n,ndim);
	for(int i=0;i<n;i++)
		for(int j=0;j<ndim;j++)
			for(int k=0;k<ndim;k++)
				xr[i][j] += R[j][k]*x[i][k];
	return xr;
}
double *Rotation::ApplyRotation(double **R, double *x)
{
	double *xr = calloc_array(ndim);
	for(int j=0;j<ndim;j++)
		for(int k=0;k<ndim;k++)
			xr[j] += R[j][k]*x[k];
	return xr;
}
double **Rotation::ApplyRotationX(double **x, int n)
{
	double **xr;

	xr = ApplyRotation(R_x, x, n);

	return xr;
}
double **Rotation::ApplyRotationY(double **x, int n)
{
	double **xr;

	xr = ApplyRotation(R_y, x, n);

	return xr;
}
double **Rotation::ApplyRotationZ(double **x, int n)
{
	double **xr;

	xr = ApplyRotation(R_z, x, n);

	return xr;
}
double **Rotation::ApplyRotationZX(double **x, int n)
{
	double **xz;
	double **xzx;

	xz  = ApplyRotation(R_z, x, n);
	xzx = ApplyRotation(R_x, xz, n);

	deallocate_two_dimensional_array(xz,n,ndim);

	return xzx;
}
double *Rotation::ApplyRotationZYX(double *x)
{
	double *z;
	double *zy;
	double *zyx;

	z   = ApplyRotation(R_z, x);
	zy  = ApplyRotation(R_y, z);
	zyx = ApplyRotation(R_x,zy);
	free(z);
	free(zy);

	return zyx;
}
void Rotation::SetRx(void)
{
	R_x[0][0] =  1.0;
	R_x[0][1] =  0.0;
	R_x[0][2] =  0.0;
	R_x[1][0] =  0.0
	R_x[1][1] =  cos(alpha);
	R_x[1][2] = -sin(alpha);
	R_x[2][0] =  0.0
	R_x[2][1] =  sin(alpha);
	R_x[2][2] =  cos(alpha);
}
void Rotation::SetRy(void)
{
	R_y[0][0] =  cos(beta);
	R_y[0][1] =  0.0
	R_y[0][2] =  sin(beta);
	R_y[1][0] =  0.0
	R_y[1][1] =  1.0
	R_y[1][2] =  0.0
	R_y[2][0] = -sin(beta);
	R_y[2][1] =  0.0
	R_y[2][2] =  cos(beta);
}
void Rotation::SetRy(void)
{
	R_z[0][0] =  cos(theta);
	R_z[0][1] = -sin(theta);
	R_z[0][2] =  0.0
	R_z[1][0] =  sin(theta);  
	R_z[1][1] =  cos(theta);
	R_z[1][2] =  0.0
	R_z[2][0] =  0.0
	R_z[2][1] =  0.0
	R_z[2][2] =  1.0
}
