#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/********* Input Variables*******************/
typedef struct
{
	double latitude;
	double longitude;
}COORDINATES;
COORDINATES input;

typedef struct
{
	double l_angle;
	double g_local;
}DATA;
DATA data;

typedef struct
{
	float gx;
	float gy;
	float gz;
	float ax;
	float ay;
	float az;
}Chk_Val;
Chk_Val check_val;  

double ppcf =0; ppcf_z = 0;
int K_correct = 0;

/****************** end of input Variables********************/

/********** Intermediate Varaibles and Functions****************************/
double pi ;
double cdr ; 
double earth_rate; 
double mm[3][3];
double gc_dcm_nwv_body[3][3];		
void compute_angles(double gc_dcm_nwv_body[3][3] );
void vect_cross_mult(double v4[3],double v5[3],double v6[3]);
void mat_mult3( double nlmat1[3][3],double nlmat2[3][3],double nlmat3[3][3]);
void psi_calc();

double xx=0,lookup_ang=0,az=0,ay=0;
double lat_detic_launch, alt_launch, long_launch;
double gravity[3], omega[3];
/********** end of Intermediate Varaibles and Functions****************************/

/********** Output Varaibles****************************/        
double output1, output2, output3;
/********** end of Output Varaibles****************************/

int main()
{


	 pi = 4.0*atan(1.0);
  	 cdr  = (pi)/180.0;
     earth_rate = 15.0409 * cdr/3600.0;

	 psi_calc();
  
}

void vect_cross_mult( double v4[3],double v5[3],double v6[3])
{
	v6[0] = v4[1]*v5[2] - v4[2]*v5[1];
	v6[1] = v4[2]*v5[0] - v4[0]*v5[2];
	v6[2] = v4[0]*v5[1] - v4[1]*v5[0];
}


void mat_mult3( double nlmat1[3][3],double nlmat2[3][3],double nlmat3[3][3])
{
/*****************************************************************************
 pdl arg3 = arg1 * arg2
 pdl first column
*****************************************************************************/

      nlmat3[0][0] = nlmat1[0][0] * nlmat2[0][0]
            	   + nlmat1[0][1] * nlmat2[1][0]
	              + nlmat1[0][2] * nlmat2[2][0];

      nlmat3[1][0] = nlmat1[1][0] * nlmat2[0][0]
		        + nlmat1[1][1] * nlmat2[1][0]
		        + nlmat1[1][2] * nlmat2[2][0];

      nlmat3[2][0] = nlmat1[2][0] * nlmat2[0][0]
		        + nlmat1[2][1] * nlmat2[1][0]
		        + nlmat1[2][2] * nlmat2[2][0];
  
/*****************************************************************
 pdl  second column
*****************************************************************/

      nlmat3[0][1] = nlmat1[0][0] * nlmat2[0][1]
	              + nlmat1[0][1] * nlmat2[1][1]
		         + nlmat1[0][2] * nlmat2[2][1];

      nlmat3[1][1] = nlmat1[1][0] * nlmat2[0][1]
       	         + nlmat1[1][1] * nlmat2[1][1]
		         + nlmat1[1][2] * nlmat2[2][1];

      nlmat3[2][1] = nlmat1[2][0] * nlmat2[0][1]
		        + nlmat1[2][1] * nlmat2[1][1]
		        + nlmat1[2][2] * nlmat2[2][1];
  
/*****************************************************************
 pdl  third column
*****************************************************************/

      nlmat3[0][2] = nlmat1[0][0] * nlmat2[0][2]
		        + nlmat1[0][1] * nlmat2[1][2]
		        + nlmat1[0][2] * nlmat2[2][2];

      nlmat3[1][2] = nlmat1[1][0] * nlmat2[0][2]
		        + nlmat1[1][1] * nlmat2[1][2]
		        + nlmat1[1][2] * nlmat2[2][2];

      nlmat3[2][2] = nlmat1[2][0] * nlmat2[0][2]
         	        + nlmat1[2][1] * nlmat2[1][2]
		        + nlmat1[2][2] * nlmat2[2][2];
} // end of void nlmat_mul3(...

void compute_angles(double gc_dcm_nwv_body[3][3])
{
	double temp;
	temp = mm[2][1];

	if (fabs(mm[2][1]) > 1.0)
		mm[2][1]=temp/fabs(temp);

	output1   = asin(mm[2][1]);
	output2= atan2(-mm[2][0],mm[2][2]);
	output3    = atan2(-mm[0][1],mm[1][1]);
	
}

void psi_calc()
{
	int i=0;
	double c[3];
    double det,g;
	double m1[3][3],m2[3][3];
	double temp_acc_pulses[3], temp_gyro_pulses[3];

	
	lat_detic_launch = input.latitude*cdr;
	long_launch = input.longitude*cdr;

	g = data.g_local;
	lookup_ang=data.l_angle*cdr;


	gravity[0] = 0.0;
	gravity[1] = 0.0;
    gravity[2] = -g;

	temp_acc_pulses[0] = check_val.ax * g;
	temp_acc_pulses[1] = check_val.ay * g;
	temp_acc_pulses[2] = check_val.az * g;

	temp_gyro_pulses[0] = check_val.gx*cdr/3600.0;
	temp_gyro_pulses[1] = check_val.gy*cdr/3600.0;
	temp_gyro_pulses[2] = check_val.gz*cdr/3600.0;

	omega[0] = earth_rate * cos(lat_detic_launch);
	omega[1] = 0.0;
	omega[2] = earth_rate * sin(lat_detic_launch);

	for( i=0;i<3;i++)
		gravity[i] =  -gravity[i];

	det = gravity[0]*omega[2] -   
            gravity[2]*omega[0];
    
    m1[0][0] =  omega[2]/det;
	m1[0][1] = -gravity[2]/det;
	m1[0][2] =  0.0;

	m1[1][0] =  0.0;
	m1[1][1] =  0.0;
	m1[1][2] =  -1/det;

	m1[2][0] = -omega[0]/det;
	m1[2][1] =  gravity[0]/det;
	m1[2][2] =  0.0;

	vect_cross_mult(temp_acc_pulses,temp_gyro_pulses, c);

	for(i = 0 ; i<3; i++)
	{
		m2[0][i] = temp_acc_pulses[i];
		m2[1][i] = temp_gyro_pulses[i];
		m2[2][i] = c[i];
	}

    mat_mult3(m1,m2,gc_dcm_nwv_body);
	compute_angles( gc_dcm_nwv_body );
	
	xx = (180-(output3/cdr))*cdr;
	ay = check_val.ay;
	az = check_val.az;
	output3 = xx - (ppcf + ((az + ppcf_z) * ay * K_correct) +((az + ppcf_z) * tan(lookup_ang))); 

}






















































