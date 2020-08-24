/*
Subroutine to compute "phi,theta,a,aprime" according to the BEM method,
when "CL" is imposed, and the 'twist' in the pitch "theta" is to be
obtained.
"theta" pitch, in degrees c 
Author: Tomas Coronado Gonzalez
Date: Augost 2020
*/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.141592654
#define T 1e-7
#define R 30                                                             // Wind turbine radius 
#define N 3                                                              // Number of blades
#define ELEM 20                                                          // Number of elements in which are divided the blades

double max(double num1, double num2){
    return (num1 > num2 ) ? num1 : num2;
}

double absC(double num){
    return sqrt(num*num);
}

int main()
{
    FILE *pf;

    // Data:
    int lambda = 7,v;
    double CD_CL = 0.01;
    double CL = 0.85,CD;
    double r[ELEM];
    for (v=0; v<ELEM; v++){
        r[v]=(v+1)*((double)1.0/ELEM)*R;
    }
    double c = 1.0;
    double flag_CLCD = 0.0;                                              // "flag_CLCD == 0" if approx. CL = 0.11*alpha;

    // Rest of variables:
    double a0 = 0.0, aprime0 = 0.0;
    double eps = 1e-12;
    double erraux = T + eps;
    double err[ELEM];
    double lambda_r[ELEM];
    for (v=0; v<ELEM; v++){
        lambda_r[v]=r[v]*lambda/R;
    } 
    int iter = 1;
    double a[ELEM],aprime[ELEM];
    for (v=0; v<ELEM; v++){
        a[v] = a0;
        aprime[v] = aprime0;
    }
    double alpha;
    double phi[ELEM], sigma[ELEM], theta[ELEM];
    double Cn[ELEM], Ct[ELEM];
    double aa[ELEM], aaprime[ELEM];

    // Aproximation for CL and CD (this is needed unless we import the xfoil data):
    if (flag_CLCD == 0){                                                 // Approx. if expressed in DEGREES "CL = 0.11 alpha"
        alpha = CL / (0.11 * 360/(2*PI) );                               // Approximate if expressed in DEGREES
        CD = CL * CD_CL;
    }                                          
    else if (flag_CLCD == 1){
        printf("flag_CLCD = 1 \n");
        exit(1);
    } 
    else{
        printf("Wrong choice \n");
        exit(1);
    } 

    // Algorithm:
    while (erraux > T){

        for (v=0; v<ELEM; v++){
            phi[v] = atan((1-a[v])/((1+aprime[v])*lambda_r[v]) + eps);   // Flow angle (in RADIANS!)(OBS!!: added small "eps" to avoid num NAN)
            
            Cn[v] = CL * cos(phi[v]) + CD * sin(phi[v]);
            Ct[v] = CL * sin(phi[v]) - CD * cos(phi[v]);

            sigma[v] = N * c / (2*PI*r[v]);

            aa[v] = 1 / (4*sin(phi[v])*sin(phi[v])/(sigma[v]*Cn[v])+1);  // Value of "a" from Blade element theory - momentum theory
            aaprime[v] = 1 / (4*sin(phi[v])*cos(phi[v])/(sigma[v]*Ct[v])-1);

            err[v] = max( absC(aa[v]-a[v])/(aa[v]) , absC(aaprime[v]-aprime[v])/(aaprime[v]) );
            a[v] = aa[v];
            aprime[v] = aaprime[v];    
        }

        erraux = max(err[0],err[1]);
        for (v=2; v<ELEM; v++){
            erraux = max(erraux,err[v]);
        }

        iter = iter + 1;  
    }  

    iter = iter - 1;
    for (v=0; v<ELEM; v++){
        theta[v] = (phi[v] - alpha) * 360/(2*PI);                        // Give result in DEGREES
        phi[v] = phi[v]* 360/(2*PI);                                     // Give result in DEGREES
    }
    
    printf("Creating file...\n"); 

    // Opening file:
    pf = fopen("output_P4.csv", "w");
    if (pf == NULL)
    {
        printf("Error: the file cannot be opened\n");
        exit(1);
    }
    else
    { 
        // Writting on the file:
        fprintf(pf,"r,a,aprime,theta,sigma\n");
        for (v=0; v<ELEM; v++)
        {
            fprintf(pf,"%f,%f,%f,%f,%f\n", r[v], a[v], aprime[v], theta[v], sigma[v]);
        }

        // Closing file:
        fclose(pf);
        printf("File with the solution (created and closed)\n");
    }
    if (ferror(pf))
    {
        printf("Error when creating the file\n");
        clearerr(pf);
    }
    
    return 0;
} 
    
