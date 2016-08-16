#include <stdio.h>
#include <string.h>

#define false 0
#define true 1
/********** Global flags and buffers ******/
unsigned char nondet_unsignedchar();
unsigned char survstatbuf[16]; //buffer size will be 12 if the number of channels are 128
unsigned char survstatbuf2[16];

int nondet_int();
int No_liu1_data = nondet_int();
int No_liu2_data = nondet_int();
int liu1_notok = nondet_int();
int liu2_notok = nondet_int();

unsigned char zero_array[16];
__CPROVER_bool mstat, sstat, mcstat, scstat;
__CPROVER_bool MainDC_DC;
__CPROVER_bool StdbyDC_DC;
/****** End of global declarations ******/




  void check_DC_DC(int dip)
{
     memset(zero_array, 0, 16); 

     if(dip < 96)
     { 
         if (memcmp (zero_array, survstatbuf, 12) == 0)
            MainDC_DC = false; 
         else 
            MainDC_DC = true; 

         if(memcmp (zero_array, survstatbuf2, 4) == 0) 
            StdbyDC_DC = false;

          else 
            StdbyDC_DC = true;
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
     
}

/********** start of the function under test **********/

int get_tmr_status(int chno,int common,__CPROVER_bool on_off)
{
	int byte = 0,bitinbyte = 0,ret=2;
	//__CPROVER_bool mstat=0,mcstat=0,sstat=0,scstat=0;
	unsigned char mask = 0x01;
	__CPROVER_bool output, output_undef=false;
    
	//Determine Main, Standby statuses
	byte      = chno / 8;
	bitinbyte = chno % 8;
	
	if(bitinbyte)
		mask = mask << bitinbyte;

	if(survstatbuf[byte] & mask)
		mstat = 1;
	else
		mstat = 0;

	if(survstatbuf2[byte] & mask) 
		sstat = 1;
	else	
		sstat = 0;
	//Get common relay status from LIU1 & LIU2
	mask = 0x01;		  
	byte      = common / 8;
	bitinbyte = common % 8;	
	
	if(bitinbyte) 
		mask = mask << bitinbyte;

    if(survstatbuf[byte] & mask) 
		mcstat = 1;
	else
		mcstat = 0;

	if(survstatbuf2[byte] & mask) 
		scstat = 1;
	else	
		scstat = 0;


	if( (!No_liu1_data && !liu1_notok) && (!No_liu2_data && !liu2_notok))
	{
		if(mstat==sstat) 
			output=mstat;
	
		else if(mcstat==scstat)
			output=mcstat;
		else
		{
			if((mstat==mcstat) && (sstat==scstat))
			{
				check_DC_DC(chno);
				
				if((StdbyDC_DC==false) && (mstat==1))   //case-1100
					output=true;
				else if((MainDC_DC==false)&&(sstat==1))//case-0011
					output=true;
				else output_undef=true;//DC_DC working cases
			}
			else output_undef=true;//cases---0110 & 1001
		}
	}
    else if(!No_liu1_data && !liu1_notok) // If only LIU1 OK
	{
		if(mstat==mcstat)
			output=mstat;
		else			
			output_undef=true;
	}
	else if(!No_liu2_data && !liu2_notok) //If only LIU2 OK
	{
		if(sstat==scstat)
			output=sstat;
		else		
			output_undef=true;
	}
	else
		output_undef=true;// If both LIUs  not OK.

	if(output_undef==true)
	{
		ret=2;
		
	}
	else 
	{
		if(output==true)
			ret=1;
		else 	
			ret=0;
	}
	return ret;
}

/**********End of the function under test**********/
 
 


   int main()
  {

   int t =  nondet_int();
    __CPROVER_bool on_off;
    __CPROVER_bool SYS1OK, SYS2OK, SYS1NOK, SYS2NOK;
    __CPROVER_bool OK[15];
    __CPROVER_bool NOK[7];
     
     unsigned char chno = nondet_unsignedchar();
     unsigned char common = nondet_unsignedchar();
     
         for (int i = 0; i < 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }
    
    t = get_tmr_status (chno,common,on_off);

    SYS1OK =  ((No_liu1_data == 0) && (liu1_notok == 0));
    SYS2OK =  ((No_liu2_data == 0) && (liu2_notok == 0));   
    SYS1NOK = ((No_liu1_data != 0) || (liu1_notok != 0));
    SYS2NOK = ((No_liu2_data != 0) || (liu2_notok != 0));

   /* Both systems ok, intention is to check for ON,either both mstat and mcstat should be 1 or any of mstat and sstat is one and mcstat and scstat should be 1 */
   OK[0] = SYS1OK && SYS2OK && ((mstat && sstat) || ((mstat || sstat) && (mcstat && scstat)));

  /* only system1 ok, intention is to check for ON, both mstat and mcstat should be 1 */
   OK[1] = SYS1OK && SYS2NOK && (mstat && mcstat);

  /* nly system2 ok, intention is to check for ON, both sstat and scstat should be 1 */
   OK[2] = SYS1NOK && SYS2OK  && (sstat && scstat);
 
  /* Both systems ok, intention is to check for off, either both mstat and mcstat should be 0 or any of mstat and sstat is one and mcstat and scstat should be 0 */
    OK[3] = SYS1OK && SYS2OK && (!mstat && !sstat);
    
   /* Only system1 ok, intention is to check for off,both mstat and mcstat should be 0*/
    OK[4] = SYS1OK && SYS2NOK  && (!mstat && !mcstat); 
  
   /* Only system2 ok, intention is to check for off,both sstat and scstat should be 0*/
    OK[5] = SYS1NOK && SYS2OK  && (!sstat && !scstat); 
 
    /* Both systems ok, intention is to check for undefined,mstat IS NOT EQUAL TO scstat AND mcstat IS NOT EQUAL TO sstat */
    OK[6] = SYS1OK && SYS2OK  && (mstat == scstat) && (mcstat == sstat) && (mstat != mcstat) ;

   /* Only system1 ok, intention is to check for undefined,mstat IS NOT EQUAL TO mcstat*/
    OK[7] = SYS1OK && SYS2NOK && (mstat != mcstat);
   
   /* Only system2 ok, intention is to check for undefined,sstat IS NOT EQUAL TO scstat */
    OK[8] = SYS1NOK && SYS2OK && (sstat != scstat);
                                    
    OK[9] = SYS1OK && SYS2OK && (mstat == mcstat) && (sstat == scstat) && (mstat != sstat) && (StdbyDC_DC == 0) && (mstat == 1);
    OK[10] = SYS1OK && SYS2OK && (mstat == mcstat) && (sstat == scstat) && (mstat != sstat) && (MainDC_DC == 0) && (sstat == 1);

    OK[11] = SYS1OK && SYS2OK && (mstat == mcstat) && (sstat == scstat) && (mstat != sstat) && !((StdbyDC_DC == 0) && (mstat == 1)) && !((MainDC_DC == 0) && (sstat == 1));
   
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chno >= 0 && chno <= 127);

    __CPROVER_assume(t >= 0 && t < 3);

    
    __CPROVER_assert (!OK[0] || (t == 1), "Hey OK0 fails");
    __CPROVER_assert (!OK[1] || (t == 1), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (t == 1), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (t == 0), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (t == 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (t == 0), "Hey OK5 fails");
    __CPROVER_assert (!OK[6] || (t == 2), "Hey OK6 fails");
    __CPROVER_assert (!OK[7] || (t == 2), "Hey OK7 fails");
    __CPROVER_assert (!OK[8] || (t == 2), "Hey OK8 fails");
    __CPROVER_assert (!OK[9] || (t == 1), "Hey OK9 fails");
    __CPROVER_assert (!OK[10] || (t == 1), "Hey OK10 fails");
    __CPROVER_assert (!OK[11] || (t == 2), "Hey OK11 fails");


  }




