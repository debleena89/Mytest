#include <stdio.h>
#include <string.h>

#define false 0
#define true 1
/********** Global flags and buffers ******/
unsigned char nondet_unsignedchar();
unsigned char survstatbuf[16]; //buffer size will be 12 if the number of channels are 128
unsigned char survstatbuf2[16];
unsigned char zero_array[16];

unsigned int nondet_uint();

int nondet_int();

__CPROVER_bool MainDC_DC;
__CPROVER_bool StdbyDC_DC;
/****** End of global declarations ******/

void check_DC_DC(int dip)
{
     bzero(zero_array,16); 

     if(dip<96)//non-telemetry dip 
 
      { 

       if(memcmp(zero_array,survstatbuf,12)==0)

            MainDC_DC=false; 

       else 

            MainDC_DC=true; 

        if(memcmp(zero_array,survstatbuf2,12)==0) 

            StdbyDC_DC=false; 

        else

            StdbyDC_DC=true; 

       } 

     else 

      { 

          if(memcmp(&zero_array[12],&survstatbuf[12],4)==0)

           MainDC_DC=false; 

          else 

           MainDC_DC=true; 

          if(memcmp(&zero_array[12],&survstatbuf2[12],4)==0) 

            StdbyDC_DC=false;

          else 

            StdbyDC_DC=true;

      } 

   return;

  }

   int main()
  {

    __CPROVER_bool t ;
    __CPROVER_bool on_off;
    __CPROVER_bool S1M=0,S1C=0,S2S=0,S2C=0;
    __CPROVER_bool SYS1OK, SYS2OK, SYS1NOK, SYS2NOK;
    __CPROVER_bool OK[6];
    __CPROVER_bool NOK[7];
    //int x,y;
    unsigned char  chno = nondet_unsignedchar();

    for (int i = 0; i < 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }

    check_DC_DC(chno);

      /* intention is to check Miandc_dc is true */

     // y=memcmp(zero_array,survstatbuf,12);

      //x = (memcmp(zero_array,survstatbuf,12)==0);

      OK[0] = (chno<96) && (memcmp(zero_array,survstatbuf,12)==0);  
                                     
   
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chno >= 0 && chno <= 127);

    //__CPROVER_assume(t >= 0 && t <= 2);

   __CPROVER_assert (!OK[0] || (MainDC_DC==0), "Hey OK0 fails");
   
  
  }




