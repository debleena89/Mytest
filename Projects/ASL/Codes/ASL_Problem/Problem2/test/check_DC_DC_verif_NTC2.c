/*Specifications of the sub-module “Check_DC_DC()”

1) If the channel number (chno) is less than 96, and if 
survstatbuf[0] to survstatbuf[11] is 0, then the flag “MainDC_DC” should be 0, otherwise it should be 1.

2) If the channel number (chno) is less than 96, and if
 survstatbuf2[0] to survstatbuf2[11] is 0, then the flag “StdbyDC_DC” should be 0, otherwise it should be 1.

3) If the channel number (chno) is greater than or equal to 96, and if
survstatbuf[12] to survstatbuf[15] is 0, then the flag “MainDC_DC” should be 0, otherwise it should be 1.

4) If the channel number (chno) is greater than or equal to 96, and if 
survstatbuf2[12] to survstatbuf[15] is 0, then the flag “StdbyDC_DC” should be 0, otherwise it should be 1.*/


#include <stdio.h>
#include <string.h>

#define false 0
#define true 1

/********** Global flags and buffers ******/
unsigned char nondet_unsignedchar();
unsigned char survstatbuf[16];
unsigned char survstatbuf2[16];
int nondet_int();
__CPROVER_bool MainDC_DC;
__CPROVER_bool StdbyDC_DC;

/****** End of global declarations ******/

void verifDc_Dc(int verifchno)
{
    unsigned char zero_array[16];
	//bzero(zero_array,16);
    memset(zero_array, 0, 16); 
    __CPROVER_bool OK[8];

    OK[0] = (verifchno < 96) && (memcmp(zero_array, survstatbuf, 12) == 0);
    OK[1] = (verifchno < 96) && !(memcmp(zero_array, survstatbuf, 12) == 0);

    OK[2] = (verifchno < 96) && (memcmp(zero_array, survstatbuf2, 12) == 0);
    OK[3] = (verifchno < 96) && !(memcmp(zero_array, survstatbuf2, 12) == 0);

    OK[4] = (verifchno >= 96) && (memcmp(&zero_array[12], &survstatbuf[12], 4) == 0);
    OK[5] = (verifchno >= 96) && !(memcmp(&zero_array[12], &survstatbuf[12], 4) == 0);

    OK[6] = (verifchno >= 96) && (memcmp(&zero_array[12], &survstatbuf2[12], 4) == 0);
    OK[7] = (verifchno >= 96) && !(memcmp(&zero_array[12], &survstatbuf2[12], 4) == 0);
                                     
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(verifchno >= 0 && verifchno <= 127);

	
    __CPROVER_assert (!OK[0] || (MainDC_DC == 0), "Hey OK0 fails"); 
    __CPROVER_assert (!OK[1] || (MainDC_DC == 1), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (StdbyDC_DC == 0), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (StdbyDC_DC == 1), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (MainDC_DC == 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (MainDC_DC == 1), "Hey OK5 fails");
    __CPROVER_assert (!OK[6] || (StdbyDC_DC == 0), "Hey OK6 fails");
    __CPROVER_assert (!OK[7] || (StdbyDC_DC == 1), "Hey OK7 fails");
}

void check_DC_DC(int dip)
{
    unsigned char zero_array[16];

    memset(zero_array, 0, 16); 
	//bzero(zero_array, 16);
    if(dip < 96)
    { 
        if(memcmp (zero_array, survstatbuf, 12) == 0)
            MainDC_DC =false;
        else 
            MainDC_DC = true;

        if(memcmp (zero_array, survstatbuf2, 12) == 0) 
            StdbyDC_DC = false;
         else 
            StdbyDC_DC = true;
    } 
    else 
    { 
         if(memcmp(&zero_array[12], &survstatbuf[12], 3) == 0)  //bug inserted here, number of bytes changed to 3, instead of 4
           MainDC_DC = false; 
         else 
           MainDC_DC = true; 

         if(memcmp(&zero_array[12], &survstatbuf2[12], 4) == 0) 
            StdbyDC_DC = false;
         else 
            StdbyDC_DC = true;
    } 

}

int main()
{
    int  chno = nondet_int();

    for (int i = 0; i <= 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }

    check_DC_DC (chno);
    verifDc_Dc (chno); // calling the verif function

    return 0;
}
