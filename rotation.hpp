#ifndef BRANT_ROTATION
#define BRANT_ROTATION
class Rotation
{
	public:
		int ndim;
		double alpha;	//angle about x-direction
		double beta;	//angle about y-direction
		double theta;	//angle about z-direction

		Rotation(void);
		Rotation(double alpha_in, double beta_in, double theta_in);
		~Rotation(void);

		void ShowRx(void);
		void ShowRy(void);
		void ShowRz(void);

		double **R_x;
		double **R_y;
		double **R_z;

		double **ApplyRotation(double **R, double **x, int n);
		double *ApplyRotation(double **R, double *x);

		double **ApplyRotationX(double **x, int n);
		double **ApplyRotationY(double **x, int n);
		double **ApplyRotationZ(double **x, int n);
		double **ApplyRotationZX(double **x, int n);
		double *ApplyRotationZYX(double *x);

		double SetAngle(double angle_in_degrees);
		void SetAlpha(double alpha_in_degrees);
		void SetBeta( double beta_in_degrees);
		void SetTheta(double theta_in_degrees);

		void SetRx(void);
		void SetRy(void);
		void SetRz(void);
};
#endif //BRANT_ROTATION
