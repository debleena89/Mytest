#include <malloc.h>
#define INVALID 300
#define false 0
#define true 1

typedef struct status
{
    char name[30];
    unsigned short  unit;
    unsigned short  channel;	
    unsigned short  common;	
    unsigned short type;
}STATUSPARMS;


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
void verif_check_dips( int verift, unsigned char verifdips[], __CPROVER_bool onoff )
{
    int j;
    int verifbyte = 0, verifbit = 0, common = -1, get_dmr_value=0;
    unsigned char verifmask = 0x01;
    __CPROVER_bool OK[15],*ptr, *okPtr, value = 1, value1 = 1, value2 = 1, NOK[15], *nokPtr;
    
    ptr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
    okPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
    nokPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
       
for (int i = 0, j = 1; i < verifdips[0]; i++, j = j + 2)
{

    verifmask = 0x01;
    verifbit = 0x01;
    get_dmr_value = get_dmr_status(verifdips[j + 1], onoff);
    verifbyte = verifdips[j + 1] / 8;
    verifbit = verifdips[j + 1] % 8;
    if (verifbit)	
        verifmask = verifmask << verifbit;

    NOK[1] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
               (lookup_status[0x00][verifdips[j + 1]].common !=	INVALID) && 
               (onoff != get_tmr_status(verifdips[j + 1],  lookup_status[0x00][verifdips[j + 1]].common));

    OK[2] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
             (lookup_status[0x00][verifdips[j + 1]].common !=	INVALID) && 
             (onoff == get_tmr_status(verifdips[j + 1],  lookup_status[0x00][verifdips[j + 1]].common));

    OK[3] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
               (lookup_status[0x00][verifdips[j + 1]].common == INVALID) && (get_dmr_value == 1); 

    NOK[3] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
                  (lookup_status[0x00][verifdips[j + 1]].common == INVALID) && (get_dmr_value == 0); 
    
    OK[4] = (verifdips[j] != 0) && onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

    NOK[5] = (verifdips[j] != 0) && onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

    NOK[6] = (verifdips[j] != 0) && !onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

    OK[7] = (verifdips[j] != 0) && !onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);
         

    okPtr[i] =  (OK[2]) || (OK[4]) || (OK[7]) || (OK[3]);
    
    nokPtr[i] = (NOK[1]) || (NOK[5]) || (NOK[6]) || (NOK[3]);
   
}

    value1 = 1;
for (int i = 0 ; i < verifdips[0]; i++)
{
    value1 = value1 && okPtr[i];
}

    __CPROVER_assert (!value1 || (verift == 1), "Hey OK1 fails");

    value2 = 1;
for (int i = 0 ; i < verifdips[0]; i++)
{
    value2 = value2 && nokPtr[i];
}
     
    __CPROVER_assert (!value2 || (verift == 0), "Hey OK2 fails");    
  
}

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

		if (dips[j] == 0)//LIU
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
				else//DMR
				{
					ret = get_dmr_status(dips[j + 1], onoff);
				}
			}
		}
		else//Obc Dips
		{
			if (onoff)
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


int main()
{
    int t = nondet_int();     
        
    unsigned char rcsdips[] = {8, 0, 7, 0, 8, 1, 9, 1, 10, 0, 11, 2, 12, 3, 13, 3, 14 };

for (int i = 0; i <= 51; i++)
{
    survstatbuf[i] = nondet_unsignedchar();
    survstatbuf2[i] = nondet_unsignedchar();
}
	
    t = check_dips(rcsdips, 0);

    verif_check_dips( t, rcsdips, 0);

}
