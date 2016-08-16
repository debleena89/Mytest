﻿/*Specifications of the sub-module “Check_DC_DC()”

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
int No_liu1_data = nondet_int();
int No_liu2_data = nondet_int();
int liu1_notok = nondet_int();
int liu2_notok = nondet_int();
int  chno = nondet_int();
int  common = nondet_int();

__CPROVER_bool MainDC_DC;
__CPROVER_bool StdbyDC_DC;
//__CPROVER_bool mstat, sstat, mcstat, scstat;

/****** End of global declarations ******/

/*void verifDc_Dc(int verifchno)
{
    unsigned char zero_array[16];

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
                                     
   // Assume on chnum -- channel number can be between 0 and 127 only 
    __CPROVER_assume(verifchno >= 0 && verifchno <= 127);

    __CPROVER_assert (!OK[0] || (MainDC_DC == 0), "Hey OK0 fails"); 
    __CPROVER_assert (!OK[1] || (MainDC_DC == 1), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (StdbyDC_DC == 0), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (StdbyDC_DC == 1), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (MainDC_DC == 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (MainDC_DC == 1), "Hey OK5 fails");
    __CPROVER_assert (!OK[6] || (StdbyDC_DC == 0), "Hey OK6 fails");
    __CPROVER_assert (!OK[7] || (StdbyDC_DC == 1), "Hey OK7 fails");
}*/

void verif_get_tmr( int verift )
{
    
    __CPROVER_bool SYS1OK, SYS2OK, SYS1NOK, SYS2NOK;
    __CPROVER_bool OK[14];
    __CPROVER_bool vmstat= 0, vsstat= 0, vmcstat=0, vscstat=0;
    int vbyte = 0,vbitinbyte = 0;
    unsigned char vmask = 0x01;

//Determine Main, Standby statuses
	vbyte      = chno / 8;
	vbitinbyte = chno % 8;
	
	if(vbitinbyte)
		vmask = vmask << vbitinbyte;

	if(survstatbuf[vbyte] & vmask)
		vmstat = 1;
	else
		vmstat = 0;

	if(survstatbuf2[vbyte] & vmask) 
		vsstat = 1;
	else	
		vsstat = 0;
	//Get common relay status from LIU1 & LIU2
	vmask = 0x01;		  
	vbyte      = common / 8;
	vbitinbyte = common % 8;	
	
	if(vbitinbyte) 
		vmask = vmask << vbitinbyte;

    	if(survstatbuf[vbyte] & vmask) 
		vmcstat = 1;
	else
		vmcstat = 0;

	if(survstatbuf2[vbyte] & vmask) 
		vscstat = 1;
	else	
		vscstat = 0;

    SYS1OK =  ((No_liu1_data == 0) && (liu1_notok == 0));
    SYS2OK =  ((No_liu2_data == 0) && (liu2_notok == 0));   
    SYS1NOK = ((No_liu1_data != 0) || (liu1_notok != 0));
    SYS2NOK = ((No_liu2_data != 0) || (liu2_notok != 0));

  
    OK[0] = SYS1OK && SYS2OK && ((vmstat && vsstat) || ((vmstat != vsstat) && (vmcstat && vscstat)));
    OK[1] = SYS1OK && SYS2NOK && (vmstat && vmcstat);
    OK[2] = SYS1NOK && SYS2OK  && (vsstat && vscstat);
 
  
    //OK[3] = SYS1OK && SYS2OK && (!mstat && !sstat) ;
    OK[3] = SYS1OK && SYS2OK && ((!vmstat && !vsstat) || ((vmstat != vsstat) && (!vmcstat && !vscstat)));;
    OK[4] = SYS1OK && SYS2NOK  && (!vmstat && !vmcstat); 
    OK[5] = SYS1NOK && SYS2OK  && (!vsstat && !vscstat); 
 
   
    OK[6] = SYS1OK && SYS2OK  && (vmstat == vscstat) && (vmcstat == vsstat) && (vmstat != vmcstat) ;
    OK[7] = SYS1OK && SYS2NOK && (vmstat != vmcstat);
    OK[8] = SYS1NOK && SYS2OK && (vsstat != vscstat);
                                    
    OK[9] = SYS1OK && SYS2OK && (vmstat == vmcstat) && (vsstat == vscstat) &&
            (vmstat != vsstat) && (StdbyDC_DC == 0) && (vmstat == 1);
    OK[10] = SYS1OK && SYS2OK && (vmstat == vmcstat) && (vsstat == vscstat) &&
            (vmstat != vsstat) && (MainDC_DC == 0) && (vsstat == 1);
    OK[11] = SYS1OK && SYS2OK && (vmstat == vmcstat) && (vsstat == vscstat) && 
            (vmstat != vsstat) && !((StdbyDC_DC == 0) && (vmstat == 1)) && !((MainDC_DC == 0) && (vsstat == 1));
    //OK[12] = SYS1OK && SYS2OK && (vmstat == vmcstat) && (vsstat == vscstat) && 
            //(vmstat != vsstat) && (((StdbyDC_DC == 1) && (vsstat == 0)) || ((MainDC_DC == 1) && (vmstat == 0)));
      OK[12] = SYS1OK && SYS2OK && (vmstat == vmcstat) && (vsstat == vscstat) && 
            (vmstat != vsstat) && (((StdbyDC_DC == 1) && (vsstat == 0)) || ((MainDC_DC == 0) && (vmstat == 1)));
   // OK[12] = SYS1NOK && SYS2NOK;
    OK[13] = SYS1OK && SYS2OK && (vmstat != vsstat) && (!vmcstat && !vscstat);
  
   
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chno >= 0 && chno <= 127);
    __CPROVER_assume(common >= 0 && common <= 127);

    //__CPROVER_assume(verift >= 0 && verift < 3);

    
    __CPROVER_assert (!OK[0] || (verift == 1), "Hey OK0 fails");
    __CPROVER_assert (!OK[1] || (verift == 1), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (verift == 1), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (verift == 0), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (verift == 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (verift == 0), "Hey OK5 fails");
    __CPROVER_assert (!OK[6] || (verift == 2), "Hey OK6 fails");
    __CPROVER_assert (!OK[7] || (verift == 2), "Hey OK7 fails");
    __CPROVER_assert (!OK[8] || (verift == 2), "Hey OK8 fails");
    __CPROVER_assert (!OK[9] || (verift == 1), "Hey OK9 fails");
    __CPROVER_assert (!OK[10] || (verift == 1), "Hey OK10 fails");
    __CPROVER_assert (!OK[11] || (verift == 2), "Hey OK11 fails");
    __CPROVER_assert (!OK[12] || (verift == 2), "Hey OK12 fails");
    __CPROVER_assert (!OK[13] || (verift == 0), "Hey OK13 fails");
    
  
}
 /********** start of the function under test **********/

void check_DC_DC(int dip)
{
    unsigned char zero_array[16];

    memset(zero_array, 0, 16); 

    if(dip < 96)
    { 
        if(memcmp (zero_array, survstatbuf, 12) == 0)
            MainDC_DC = false;
        else 
            MainDC_DC = true;

        if(memcmp (zero_array, survstatbuf2, 12) == 0) 
            StdbyDC_DC = false;
         else 
            StdbyDC_DC = true;
    } 
    else 
    { 
         if(memcmp(&zero_array[12], &survstatbuf[12], 4) == 0)
           MainDC_DC = false; 
         else 
           MainDC_DC = true; 

         if(memcmp(&zero_array[12], &survstatbuf2[12], 4) == 0) 
            StdbyDC_DC = false;
         else 
            StdbyDC_DC = true;
    } 

}


int get_tmr_status(int chno,int common)
{
	int byte = 0,bitinbyte = 0,ret=2;
	__CPROVER_bool mstat=0,mcstat=0,sstat=0,scstat=0;
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
			output=mstat; // mstat
	
		else if(mcstat == scstat)  // || (!mcstat && !scstat))
			output=mcstat; //mcstat
		else
		{
			if((mstat==mcstat) && (sstat==scstat))
			{
				check_DC_DC(chno);
                                         				
				if((StdbyDC_DC==false) && (mstat==1))   //case-1100
					output=true;
				else if((MainDC_DC==false)&&(sstat==1)) //case-0011
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

/******* main() start ********/

int main()
{
    //int  chno = nondet_int();
   // int  common = nondet_int();
    int  t = nondet_int();

    for (int i = 0; i <= 15; i++)
    {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
    }
    
    t = get_tmr_status (chno,common);

    verif_get_tmr( t );

    return 0;
}
