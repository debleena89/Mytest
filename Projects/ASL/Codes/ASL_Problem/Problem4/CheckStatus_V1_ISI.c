#include <stdio.h>
#include <math.h>


unsigned int TimeoutFlag, HeaderValidFlag, chksumValidFlag, DataValid, mode_word,Pack_Fail,sflag;

unsigned int Sys1Count[3], Sys1Count_old[3], Switchover, Sys1FailFlag[3], Sys2FailFlag[3],Interstatus, overall_status;
 
unsigned int DataInvalidCount, Pack_fail, Sys1RelLimitFail[3],Sys1AbsLimitFail[3],Sys1DataInvalidCount[3],Sys2Count[3],Sys2Count_old[3];

__CPROVER_bool OK[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
            OK[2] = (TimeoutFlag == 1) || (HeaderValidFlag == 0) || (chksumValidFlag == 0) || 
                    (DataValid == 0) && (mode_word== 11) || (mode_word == 44) && (DataInvalidCount >= 20);
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

              OK[5] = (fabs(Sys1Count[0] - Sys1Count_old[0]) > 0.0489);
   
              OK[6] = (fabs(Sys1Count[1] - Sys1Count_old[1]) > 0.0489);

              OK[7] = (fabs(Sys1Count[2] - Sys1Count_old[2]) > 0.0489);
           

									
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


                        OK[8] = (Sys1Count[0] > 0.625) ;
   
              		OK[9] = (Sys1Count[1] > 0.625) ;

              		OK[10] = (Sys1Count[2] > 0.625) ;							   						
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
                   OK[4] = (Sys1Count[i] > 0.625) || (Sys1Count[i] < -0.625) && (mode_word == 11) || (mode_word == 44)
                     && (Sys1DataInvalidCount[i] >= 10);
                   OK[11] = (Sys1Count[0] > 0.625) || (Sys1Count[0] < -0.625) && (mode_word == 11) || (mode_word == 44)
                     && (Sys1DataInvalidCount[0] >= 10); 
		   OK[12] = (Sys1Count[1] > 0.625) || (Sys1Count[1] < -0.625) && (mode_word == 11) || (mode_word == 44)
                     && (Sys1DataInvalidCount[1] >= 10); 
                   OK[13] = (Sys1Count[2] > 0.625) || (Sys1Count[2] < -0.625) && (mode_word == 11) || (mode_word == 44)
                     && (Sys1DataInvalidCount[2] >= 10); 
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
	
  				OK[11] = (mode_word == 33) && (Sys1AbsLimitFail[i] == 1) && (Sys1DataInvalidCount[0] >= 10);
				OK[12] = (mode_word == 33) && (Sys1AbsLimitFail[i] == 1) && (Sys1DataInvalidCount[1] >= 10);
				OK[13] = (mode_word == 33) && (Sys1AbsLimitFail[i] == 1) && (Sys1DataInvalidCount[2] >= 10);		
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

void VerifCheckStatus()
{
   //__CPROVER_bool OK[5] = {0,0,0,0,0};

   int i;
   
   //OK[1] = (TimeoutFlag == 1) || (HeaderValidFlag == 0) || (chksumValidFlag == 0) || (DataValid == 0)
     //      && (mode_word == 11) || (mode_word == 44);
   OK[3] = (Sys1Count[i] > 0.625) || (Sys1Count[i] < -0.625) && (mode_word == 11) || (mode_word == 44)
           || (mode_word == 33);
  

   for (i = 0 ; i < 3 ; i++)
    OK[1] = OK[1] && (Sys1Count[i] == Sys1Count_old[i]);

  // __CPROVER_assert (!OK[1] || (Interstatus == 0), "Hey OK1 fails");
   __CPROVER_assert (!OK[2] || (overall_status == 0), "Hey OK2 fails");
   __CPROVER_assert (!OK[3] || (Interstatus == 0), "Hey OK3 fails");

   __CPROVER_assert (!OK[4] || (Interstatus == 0), "Hey OK4 fails");

   __CPROVER_assert (!OK[5] || (Interstatus ==  0xffef), "Hey OK5 fails");
   __CPROVER_assert (!OK[6] || (Interstatus ==  0xffdf), "Hey OK6 fails");
   __CPROVER_assert (!OK[7] || (Interstatus ==  0xffbf), "Hey OK7 fails");


   __CPROVER_assert (!OK[8] || (Interstatus ==  0xfffe), "Hey OK8 fails");
   __CPROVER_assert (!OK[9] || (Interstatus ==  0xfffd), "Hey OK9 fails");
   __CPROVER_assert (!OK[10] || (Interstatus ==  0xfffb), "Hey OK10 fails");


   __CPROVER_assert (!OK[11] || (overall_status ==  0x0100), "Hey OK11 fails");
   __CPROVER_assert (!OK[12] || (overall_status ==  0x0400), "Hey OK12 fails");
   __CPROVER_assert (!OK[13] || (overall_status ==  0x0800), "Hey OK13 fails");
   




}

void main()
{
	CheckStatus();
	printf("over");

        VerifCheckStatus();
}

