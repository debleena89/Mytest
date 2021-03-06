#include <malloc.h>
#define INVALID 300

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

int get_tmr_status(int chno, int common)
{
    return 1;
}

__CPROVER_bool get_dmr_status(unsigned char chnl, __CPROVER_bool onoff)
{
    return 1;
}
void verif_check_dips( int verift, unsigned char verifdips[], __CPROVER_bool onoff )
{
    int j;
    int verifbyte = 0, verifbit = 0, common = -1, get_dmr_value=0;
    unsigned char verifmask = 0x01;
   // verifbit = 0x01;
		/*get_dmr_value = get_dmr_status(verifdips[j + 1], onoff);
		verifbyte = verifdips[j + 1] / 8;
		verifbit = verifdips[j + 1] % 8;
		if (verifbit)	
			verifmask = verifmask << verifbit;*/
        
    __CPROVER_bool OK[15],*ptr, *okPtr, value=1, NOK[15], *nokPtr;
    
    okPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
    //nokPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
       
for (int i = 0, j = 1; i < verifdips[0]; i++, j = j + 2)
{
    verifmask = 0x01;
    get_dmr_value = get_dmr_status(verifdips[j + 1], onoff);
    verifbyte = verifdips[j + 1] / 8;
    verifbit = verifdips[j + 1] % 8;
    if (verifbit)
       verifmask = verifmask << verifbit;
	//__CPROVER_assume(get_dmr_value == 0 || get_dmr_value == 1);

     
   OK[2] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
             (lookup_status[0x00][verifdips[j + 1]].common !=	INVALID) && 
             (onoff == get_tmr_status(verifdips[j + 1],  lookup_status[0x00][verifdips[j + 1]].common));

   //OK[3] = (verifdips[j] == 0) && (lookup_status[0x00][verifdips[j + 1]].unit != INVALID) && 
               (lookup_status[0x00][verifdips[j + 1]].common == INVALID) && (get_dmr_value == 1); 

    
    //OK[4] = (verifdips[j] != 0) && onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

    //OK[7] = (verifdips[j] != 0) && !onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);
         
    
    okPtr[i] = (OK[2]);//|| (OK[4]) || (OK[7]) || (OK[3]);
    
	 //nokPtr[i] = (NOK[1])  || (NOK[5]) || (NOK[6]) || (NOK[3]);

   
}

 
    value = 1;

for (int i = 0 ; i < verifdips[0]; i++)
{
    value = value && *(okPtr+i);
}
    __CPROVER_assert (!value || (verift == 1), "Hey OK2 fails");

      /*value = 1;
  for (int i = 0 ; i < verifdips[0]; i++)
 {
       value = value && *(nokPtr+i);

 }
   __CPROVER_assert (!value || (verift == 0), "Hey OK3 fails");*/
    
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
					if (onoff == get_tmr_status(dips[j + 1], common))
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
        
    unsigned char rcsdips[] = {8, 3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14 };

    for (int i = 0; i <= 51; i++)
     {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
     }
	
     t = check_dips(rcsdips, 0);

     verif_check_dips( t, rcsdips, 0);

}
