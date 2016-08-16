
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

int get_tmr_status(int chno, int common);
bool get_dmr_status(unsigned char chnl, bool onoff);


int check_dips(unsigned char dips[], bool onoff)
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

	unsigned char rcsdips[] = {8, 3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14 };
	
	check_dips(rcsdips, 0);
}