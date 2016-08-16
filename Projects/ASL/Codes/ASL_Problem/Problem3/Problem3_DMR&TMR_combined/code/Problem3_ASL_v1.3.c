#include <malloc.h>
#include <string.h>

#define INVALID 300
#define false 0
#define true 1

/*** global flags and data structures ***************/
typedef struct status
{
	char name[30];
	unsigned short  unit;
	unsigned short  channel;	
	unsigned short  common;	
	unsigned short type;
}STATUSPARMS;

unsigned short nondet_ushort();
STATUSPARMS lookup_status[4][128];
unsigned char survstatbuf[52];
unsigned char survstatbuf2[52];
__CPROVER_bool nondet_bool();

int nondet_int();
unsigned char nondet_unsignedchar();
int No_liu1_data = nondet_int();
int No_liu2_data = nondet_int();
int liu1_notok = nondet_int();
int liu2_notok = nondet_int();

__CPROVER_bool MainDC_DC;
__CPROVER_bool StdbyDC_DC;
/********** end of global flags and data structures **************/

/********** start of sub-modules *****************/
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
	
		else if(mcstat == scstat)  
			output=mcstat; 
		else
		{
			if((mstat==mcstat) && (sstat==scstat))
			{
				check_DC_DC(chno);
                                         				
				if((StdbyDC_DC==false) && (mstat==1))   //case-1100
					output=true;
				else if((MainDC_DC==false)&&(sstat==1)) //case-0011
					output=true;
				else output_undef=true;
			}
			else output_undef=true;
		}
	}
    else if(!No_liu1_data && !liu1_notok) 
	{
		if(mstat==mcstat)
			output=mstat;
		else			
			output_undef=true;
	}
	else if(!No_liu2_data && !liu2_notok) 
	{
		if(sstat==scstat)
			output=sstat;
		else		
			output_undef=true;
	}
	else
		output_undef=true;

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

__CPROVER_bool get_dmr_status(unsigned char chnum, __CPROVER_bool onoff)
{
    unsigned char mask = 0x01;
    __CPROVER_bool output, output_undef = false, ret;
    int bit = chnum % 8, byte = chnum / 8;
    mask =  mask << bit;

    /////////////detect output///////////////////////
    if((!No_liu1_data && !liu1_notok) && (!No_liu2_data && !liu2_notok))
    {
        if((survstatbuf[byte] & mask) || (survstatbuf2[byte]&mask))
            output = true;
        else
            output = false;
    }
    else if (!No_liu1_data && !liu1_notok)
        output = survstatbuf[byte] & mask;
    else if(!No_liu2_data && !liu2_notok)
        output = survstatbuf2[byte] & mask;
    else 
        output_undef = true;

    ///////////////set ret value/////////////////////
    if (output_undef) 
        ret = false;
    else if (onoff == output) 
        ret = true;
    else 
        ret = false;     

    /////////////////////////////////////////////////
    return ret;

}

/************** end of sub-modules ****************/

/*************** start of verification function************************/

void verif_check_dips( int verift, unsigned char verifdips[], __CPROVER_bool onoff )
{
    int j, i;
    int verifbyte = 0, verifbit = 0, get_dmr_value=0;
    unsigned char verifmask ;
           
     __CPROVER_bool OK[5] ={0,0,0,0,0}, *okPtr, OK_value = 1, NOK_value = 0, NOK[5] = {0,0,0,0,0}, *nokPtr;
    
     okPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
     nokPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
       
	for (i = 0, j = 1; i < verifdips[0]; i++, j = j + 2)
	{
        	verifmask = 0x01;
	    	verifbyte = verifdips[j + 1] / 8;
		verifbit = verifdips[j + 1] % 8;
		if (verifbit)	
			verifmask = verifmask << verifbit;

		get_dmr_value = get_dmr_status(verifdips[j + 1], onoff);

		
	        NOK[1] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
               		(lookup_status[0x00][verifdips[j + 1]].common !=	INVALID) && 
                        (onoff != get_tmr_status(verifdips[j + 1],  lookup_status[0x00][verifdips[j + 1]].common));

     		OK[1] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
		        (lookup_status[0x00][verifdips[j + 1]].common !=	INVALID) && 
                        (onoff == get_tmr_status(verifdips[j + 1],  lookup_status[0x00][verifdips[j + 1]].common));

	        OK[2] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
                        (lookup_status[0x00][verifdips[j + 1]].common == INVALID) && (get_dmr_value == 1); 

	        NOK[2] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
                         (lookup_status[0x00][verifdips[j + 1]].common == INVALID) && (get_dmr_value == 0); 
   
                OK[3] = (verifdips[j] != 0) && onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

                NOK[3] = (verifdips[j] != 0) && onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

  		NOK[4] = (verifdips[j] != 0) && !onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

                OK[4] = (verifdips[j] != 0) && !onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

		okPtr[i] =  (OK[1]) || (OK[2]) || (OK[3]) || (OK[4]);
   
                nokPtr[i] = (NOK[1]) || (NOK[2]) || (NOK[3]) || (NOK[4]);

		OK[1] = 0; OK[2] = 0; OK[3] = 0; OK[4] = 0;
		NOK[1] = 0; NOK[2] = 0; NOK[3] = 0; NOK[4] =0;
	}

  for (i = 0 ; i < verifdips[0]; i++)
  {
       OK_value = OK_value && okPtr[i];

  }

   __CPROVER_assert (!OK_value || (verift == 1), "Hey OK fails");

  for (i = 0 ; i < verifdips[0]; i++)
  {
       NOK_value = NOK_value || nokPtr[i];

  }

    __CPROVER_assert (!NOK_value || (verift == 0), "Hey NOK fails");   
    
}
/***************** end of vefifiaction function ******************/

/*********  Start of function under test ******************/

int check_dips(unsigned char dips[], __CPROVER_bool onoff)
{
	int i, j;
	int byte = 0, bit = 0, common = -1;
	int ret = 1, ret_anded = 1;
	int strno = 0;
	unsigned char mask = 0x01;
			
	for (i = 0, j = 1; i < dips[0]; i++, j = j + 2)
	{
		mask = 0x01;
		common = 0;
		byte = dips[j + 1] / 8;
		bit = dips[j + 1] % 8;
		if (bit)	
			mask = mask << bit;

		if (dips[j] == 0)
		{
			if (lookup_status[0x00][dips[j + 1]].unit != INVALID)		//do not check common dips
			{
				if (lookup_status[0x00][dips[j + 1]].common !=	INVALID)//has common chno--TMR
				{
					common = lookup_status[0x00][dips[j + 1]].common;
					if (onoff != get_tmr_status(dips[j + 1], common)) 
					{
						ret = 0; 
					}
				}
				else
				{
					ret = get_dmr_status(dips[j + 1], onoff);
				}
			}
		}
		else
		{
			if (onoff) //end
			{
				if (!(survstatbuf[12 * dips[j] + byte + 4] & mask))
				{
					ret = 0;
				}
			}
			else
			{
				if (survstatbuf[12 * dips[j] + byte + 4] & mask)
				{
					ret = 0;
				}
			}
		}
		ret_anded = ret && ret_anded;
	}
	
	return ret_anded;
}

/**************** end of function under test *********************/

int main()
{
        int t ;  
	int i, j ;    
	__CPROVER_bool onoff = nondet_bool();
        //unsigned char rcsdips[] = {8, 3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14 }; //all dips other than unit0 dips 
	//unsigned char rcsdips[] = {3,0 , 7, 0, 8, 0, 9}; //only unit0 dips
       unsigned char rcsdips[] = {8, 3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 2, 12, 0, 13, 1, 14 }; //combination of all units  dips
        for (i = 0; i <= 51; i++)
        {
        	survstatbuf[i] = nondet_unsignedchar();
        	survstatbuf2[i] = nondet_unsignedchar();
     	}

     for(i = 0; i<4; i++)	
	for(j = 0; j<128; j++)
	{
 		lookup_status[i][j].unit = nondet_ushort();
		lookup_status[i][j].channel = nondet_ushort();
		lookup_status[i][j].common = nondet_ushort();
	}

	for(j= 0; j< 128; j++)
	{
	__CPROVER_assume( lookup_status[0][j].unit == 0 || lookup_status[0][j].unit == 1 || lookup_status[0][j].unit == 2 || lookup_status[0][j].unit == 3 || lookup_status[0][j].unit == 300);
	__CPROVER_assume( (lookup_status[0][j].channel >=0 && lookup_status[0][j].channel <= 127 ) || lookup_status[0][j].channel== 300 );
	__CPROVER_assume( (lookup_status[0][j].common >=0 &&  lookup_status[0][j].common <= 127 ) || lookup_status[0][j].common== 300 );
	}

     t =check_dips(rcsdips, onoff);
     verif_check_dips( t, rcsdips, onoff);

}
