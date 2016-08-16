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
unsigned short nondet_unsignedshort();


void verif_check_dips( int verift, unsigned char verifdips[], __CPROVER_bool onoff )
{
    int j, i;
    int verifbyte = 0, verifbit = 0, common = -1, get_dmr_value=0;
    unsigned char verifmask = 0x01;
    
    __CPROVER_bool OK[2] = {0,0},*nokPtr, *okPtr, okvalue=1, nokvalue = 1, NOK[2]= {0,0 } ;
    
    okPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
    nokPtr = (__CPROVER_bool*)malloc(verifdips[0]*sizeof(__CPROVER_bool));
       
for ( i = 0, j = 1; i < verifdips[0]; i++, j = j + 2)
{

	verifmask = 0x01;
    verifbyte = verifdips[j + 1] / 8;
    verifbit = verifdips[j + 1] % 8;

    if (verifbit)	
	verifmask = verifmask << verifbit; 

     OK[0] = (verifdips[j] != 0) && onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

   //  NOK[0] = (verifdips[j] != 0) && onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

   //  NOK[1] = (verifdips[j] != 0) && !onoff && (survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);

     OK[1] = (verifdips[j] != 0) && !onoff && !(survstatbuf[12 * verifdips[j] + verifbyte + 4] & verifmask);
         

     okPtr[i]=   (OK[0]) || (OK[1]) ;
    
    // nokPtr[i] =  (NOK[0]) || (NOK[1]) ;

      OK[0] = 0; OK[1] = 0;

}
 
  
	for (i = 0 ; i < verifdips[0]; i++)
	{
		okvalue = okvalue && *(okPtr+i);
	}

	__CPROVER_assert (!okvalue || (verift == 1), "Hey OK2 fails");

     
/*	 for (i = 0 ; i < verifdips[0]; i++)
	 {
		  nokvalue = nokvalue && *(nokPtr+i);
	  }

  __CPROVER_assert (!nokvalue || (verift == 0), "Hey OK3 fails");*/
    
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


		if (dips[j] != 0)///Obc Dips
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
	int i, j;

    __CPROVER_bool onoff = nondet_bool() ;
        
    unsigned char rcsdips[] = {8, 0, 7, 3, 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14 };
//	unsigned char rcsdips[] = {8, 3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14 };
    for (i = 0; i <= 51; i++)
     {
        survstatbuf[i] = nondet_unsignedchar();
        survstatbuf2[i] = nondet_unsignedchar();
     }

	
	t=check_dips(rcsdips, onoff);

        verif_check_dips( t, rcsdips, onoff);

}
