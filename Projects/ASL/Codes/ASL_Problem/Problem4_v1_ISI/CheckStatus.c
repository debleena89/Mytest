#include <stdio.h>
#include <math.h>


unsigned int TimeoutFlag, HeaderValidFlag, chksumValidFlag, DataValid, mode_word,Pack_Fail,sflag;

unsigned int Sys1Count[3], Sys1Count_old[3], Switchover, Sys1FailFlag[3], Sys2FailFlag[3],Interstatus, overall_status;
 
unsigned int DataInvalidCount, Pack_fail, Sys1RelLimitFail[3],Sys1AbsLimitFail[3],Sys1DataInvalidCount[3],Sys2Count[3],Sys2Count_old[3];


void  CheckStatus()
{


	int i;
if(( (TimeoutFlag == 1) || (HeaderValidFlag == 0) || (chksumValidFlag == 0) || (DataValid == 0) ) && ((mode_word== 11) || (mode_word == 44)))
	{
		for(i=0;i<3;i++)
		{
			Sys1Count[i] = Sys1Count_old[i];
    		Sys2Count[i] = Sys2Count_old[i];			
		}
	
		DataInvalidCount++;
		
		Pack_Fail = 1;
		
		sflag = 1;		
	
		if(DataInvalidCount >= 20)
		{		
			overall_status = overall_status | 0x0100;// Sys1 failures
			overall_status = overall_status | 0x0400;
			overall_status = overall_status | 0x0800;					
				
        	overall_status = overall_status | 0x0020; // Sys2 failures        	
        	overall_status = overall_status | 0x0040;
        	overall_status = overall_status | 0x0080;	
        	
			Pack_Fail = 1;        	
													
			for(i=0;i<3;i++)
			{
        		Sys1FailFlag[i] = 1;
        		Sys2FailFlag[i] = 1;	        				
			}
		}
	}
	else
	{
		Pack_Fail = 0;
				
  		if(DataInvalidCount < 20)
  			DataInvalidCount = 0;			
  	}    
	
   	for(i=0; i<3; i++)
   	{
     	if(fabs(Sys1Count[i] - Sys1Count_old[i]) > 0.0489) // for jerk
     	{
			Interstatus = Interstatus | (0x0001 << (i+4) );	
			Sys1RelLimitFail[i] = 1;
     	}
     	else
     	{
			Sys1RelLimitFail[i] = 0;
			
			if(i==0)
				Interstatus = Interstatus & 0xffef;
			else if(i==1)
				Interstatus = Interstatus & 0xffdf;
			else if(i==2)
				Interstatus= Interstatus & 0xffbf;
									
     	}
   	    
   	    if( ((Sys1Count[i] > 0.625) || (Sys1Count[i] < -0.625)) && ((mode_word == 11) || (mode_word == 44)) ) 
      	{
       		Interstatus		    = Interstatus | (0x0001 << i);
       		
       		if( (i==0) && (Switchover == 0) )
       			Sys1AbsLimitFail[0] = 1; 
       		else if(i!=0)
       			Sys1AbsLimitFail[i] = 1;
     	}
     	
     	else if( ((Sys1Count[i] > 0.625) || (Sys1Count[i] < -0.625)) && (mode_word == 33) )
     	{
       		Interstatus		    = Interstatus | (0x0001 << i);
       		
       		if( (i==0) && (Switchover == 0) )
       			Sys1AbsLimitFail[0] = 1; 
       		else if(i!=0)
       			Sys1AbsLimitFail[i] = 1;
     	}
     	
     	else
     	{
			Sys1AbsLimitFail[i] = 0;  
			
			if(i==0)
				Interstatus = Interstatus & 0xfffe;
			else if(i==1)
				Interstatus = Interstatus & 0xfffd;
			else if(i==2)
				Interstatus = Interstatus & 0xfffb;								   						
     	}
      
     	
		if( (Sys1AbsLimitFail[i] == 1) && (Pack_Fail==0) && ((mode_word== 11) || (mode_word == 44)) )
		{
			sflag = 1;
			
			Sys1Count[i]  = Sys1Count_old[i];								
			Sys1DataInvalidCount[i]++;
			
			if(Sys1DataInvalidCount[i] >= 10)
			{				
				if(i==0)
					overall_status = overall_status | 0x0100;
				else if(i==1)
					overall_status = overall_status | 0x0400;
				else if(i==2)
					overall_status = overall_status | 0x0800;					
				
				Sys1FailFlag[i] = 1;
			}
		}
		
		else if((mode_word == 33) && (Sys1AbsLimitFail[i] == 1) )
		{
			sflag = 1;
			
			Sys1Count[i]  = Sys1Count_old[i];
			Sys1DataInvalidCount[i]++;
			
			if(Sys1DataInvalidCount[i] >= 10)
			{			
				if(i==0)
					overall_status = overall_status | 0x0100;
				else if(i==1)
					overall_status = overall_status | 0x0400;
				else if(i==2)
					overall_status = overall_status | 0x0800;					
				
				Sys1FailFlag[i] = 1;				
			}			
		}
		
		else
		{
      		Sys1Count_old[i] = Sys1Count[i]; 
      		if(Sys1DataInvalidCount[i] < 10)
      			Sys1DataInvalidCount[i] = 0;
		}		      	      			      			
   	}
}

void main()
{
	CheckStatus();
	printf("over");

}

