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
int MainDC_DC  = nondet_int();
int StdbyDC_DC = nondet_int();
/****** End of global declarations ******/


  void bzero(void *s1, unsigned n)
{
    register char *t = (char *) s1;

    while (n != 0) {
        *t++ = 0;
         n--;
    }
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned char u1, u2;

    for (; n-- ; s1++, s2++) {
        u1 = * (unsigned char *) s1;
        u2 = * (unsigned char *) s2;
        if ( u1 != u2) {
            return (u1-u2);
        }
    }
    return 0;
}

  void check_DC_DC(int dip)
{
     bzero(zero_array, 16); 

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
int get_tmr_status(int chno,int common,bool on_off)
{
	int byte = 0,bitinbyte = 0,ret=2;
	bool mstat=0,mcstat=0,sstat=0,scstat=0;
	unsigned char mask = 0x01;
	bool output, output_undef=false;
    
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

    __CPROVER_bool t ;
    __CPROVER_bool on_off;
    __CPROVER_bool S1M=0,S1C=0,S2S=0,S2C=0;
    __CPROVER_bool SYS1OK, SYS2OK, SYS1NOK, SYS2NOK;
    __CPROVER_bool OK[6];
    __CPROVER_bool NOK[7];
     
     int verifbyte = 0,verifbitinbyte = 0;
     unsigned char verifMask = 0x01;
     unsigned char chno = nondet_unsignedchar();
     unsigned char common = nondet_unsignedchar();


      //Determine Main, Standby statuses
	verifbyte      = chno / 8;
	verifbitinbyte = chno % 8;
	
	if(verifbitinbyte)
		verifMask = verifMask << verifbitinbyte;

	if(survstatbuf[verifbyte] & verifMask)
		S1M = 1;
	else
		S1M = 0;

	if(survstatbuf2[verifbyte] & verifMask) 
		S2S = 1;
	else	
		S2S = 0;
	//Get common relay status from LIU1 & LIU2
	verifMask = 0x01;		  
	verifbyte      = common / 8;
	verifbitinbyte = common % 8;	
	
	if(verifbitinbyte) 
		verifMask = verifMask << verifbitinbyte;

    if(survstatbuf[verifbyte] & verifMask) 
		S1C = 1;
	else
		S1C = 0;

	if(survstatbuf2[verifbyte] & verifMask) 
		S2C = 1;
	else	
		S2C = 0;

    
      
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

   /* Both systems ok, intention is to check for ON,either both S1M and S1C should be 1 or any of S1M and S2S is one and S1C and S2C should be 1 */
   OK[0] = SYS1OK && SYS2OK && on_off && ((S1M && S2S) || ((S1M || S2S) && (S1C && S2C)));

  /* only system1 ok, intention is to check for ON, both S1M and S1C should be 1 */
   OK[1] = SYS1OK && SYS2NOK && on_off && (S1M && S1C);

  /* nly system2 ok, intention is to check for ON, both S2S and S2C should be 1 */
   OK[2] = SYS1NOK && SYS2OK && on_off && (S2S && S2C);

  /* Both systems ok, intention is to check for off, either both S1M and S1C should be 0 or any of S1M and S2S is one and S1C and S2C should be 0 */
    OK[3] = SYS1OK && SYS2OK && !on_off && ((!S1M && !S2S) || ((S1M != S2S) && (!S1C && !S2C)));
    
   /* Only system1 ok, intention is to check for off,both S1M and S1C should be 0*/
    OK[4] = SYS1OK && SYS2NOK && !on_off && (!S1M && !S1C); 
  
   /* Only system2 ok, intention is to check for off,both S2S and S2C should be 0*/
    OK[5] = SYS1NOK && SYS2OK && !on_off && (!S2S && !S2C); 
 
    /* Both systems ok, intention is to check for undefined,S1M IS NOT EQUAL TO S2C AND S1C IS NOT EQUAL TO S2S */
    OK[6] = SYS1OK && SYS2OK  && ((S1M == S2C) && (S1C == S2S));

   /* Only system1 ok, intention is to check for undefined,S1M IS NOT EQUAL TO S1C*/
    OK[7] = SYS1OK && SYS2NOK && (S1M != S1C);
   
   /* Only system1 ok, intention is to check for undefined,S2S IS NOT EQUAL TO S2C */
    OK[8] = SYS1OK && SYS2NOK && (S2S != S2C);
                                    
   
   /* Assume on chnum -- channel number can be between 0 and 127 only */
    __CPROVER_assume(chno >= 0 && chno <= 127);

    __CPROVER_assume(t >= 0 && t <= 2);

    
    __CPROVER_assert (!OK[0] || (t != 0), "Hey OK0 fails");
    __CPROVER_assert (!OK[1] || (t != 0), "Hey OK1 fails");
    __CPROVER_assert (!OK[2] || (t != 0), "Hey OK2 fails");
    __CPROVER_assert (!OK[3] || (t == 0), "Hey OK3 fails");
    __CPROVER_assert (!OK[4] || (t == 0), "Hey OK4 fails");
    __CPROVER_assert (!OK[5] || (t == 0), "Hey OK5 fails");
    __CPROVER_assert (!OK[6] || (t == 2), "Hey OK6 fails");
    __CPROVER_assert (!OK[7] || (t == 2), "Hey OK7 fails");
    __CPROVER_assert (!OK[8] || (t == 2), "Hey OK8 fails");


  }




