#include <stdio.h>
#include <math.h>

double nondet_double();
unsigned int nondet_unsignedint();
unsigned int TimeoutFlag, HeaderValidFlag, chksumValidFlag, DataValid, mode_word,Pack_Fail,sflag;

unsigned int Switchover, Sys1FailFlag[3], Sys2FailFlag[3],Interstatus, overall_status;
double Sys1Count[3], Sys1Count_old[3];
 
unsigned int DataInvalidCount, Pack_fail, Sys1RelLimitFail[3],Sys1AbsLimitFail[3],Sys1DataInvalidCount[3],Sys2Count[3],Sys2Count_old[3];


void  CheckStatus()
{

	int i;
	
	if (((TimeoutFlag == 1) || (HeaderValidFlag == 0) || (chksumValidFlag == 0) || (DataValid == 0)) && ((mode_word == 11) || (mode_word == 44)))
	{
		for (i = 0; i<3; i++)
		{
			Sys1Count[i] = Sys1Count_old[i];
			Sys2Count[i] = Sys2Count_old[i];
		}

		DataInvalidCount++;

		Pack_Fail = 1;

		sflag = 1;

		if (DataInvalidCount >= 20)
		{
			overall_status = overall_status | 0x0100;// Sys1 failures
			overall_status = overall_status | 0x0400;
			overall_status = overall_status | 0x0800;

			overall_status = overall_status | 0x0020; // Sys2 failures        	
			overall_status = overall_status | 0x0040;
			overall_status = overall_status | 0x0080;

			Pack_Fail = 1;

			for (i = 0; i<3; i++)
			{
				Sys1FailFlag[i] = 1;
				Sys2FailFlag[i] = 1;
			}
		}
	}
	else
	{
		Pack_Fail = 0;

		if (DataInvalidCount < 20)
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
      		//Sys1Count_old[i] = Sys1Count[i]; 
      		if(Sys1DataInvalidCount[i] < 10)
      			Sys1DataInvalidCount[i] = 0;
		}      	      			
	}
        
}

void VerifCheckStatus()
{
	__CPROVER_bool RelativeDataValidityFailure_X = 0;
	__CPROVER_bool RelativeDataValidityFailure_Y = 0;
	__CPROVER_bool RelativeDataValidityFailure_Z = 0;

	__CPROVER_bool AbsoluteDataValidityIntermittentFailure_X = 0;
	__CPROVER_bool AbsoluteDataValidityIntermittentFailure_Y = 0;
	__CPROVER_bool AbsoluteDataValidityIntermittentFailure_Z = 0;

	__CPROVER_bool DataCheckModeValidity;

	/**** Appliacble modes for data validity checks*************/
	DataCheckModeValidity = (mode_word == 11 || mode_word == 33 || mode_word == 44);

	/********* Relative limit check failure conditions in X, Y, Z directions ***********/
	RelativeDataValidityFailure_X = (((fabs(Sys1Count[0] - Sys1Count_old[0])) > 0.0489) && DataCheckModeValidity);
	RelativeDataValidityFailure_Y = (((fabs(Sys1Count[1] - Sys1Count_old[1])) > 0.0489) && DataCheckModeValidity);
	RelativeDataValidityFailure_Z = (((fabs(Sys1Count[2] - Sys1Count_old[2])) > 0.0489) && DataCheckModeValidity);

   /********* asserts for relative limit failure conditions represented in bit4(X), bit5(Y), bit6(Z) of Interstatus word *********/
	__CPROVER_assert(!RelativeDataValidityFailure_X || ((Interstatus & 0x0010) == 0x0010), "Relative X Fails");
	__CPROVER_assert(!RelativeDataValidityFailure_Y || ((Interstatus & 0x0020) == 0x0020), "Relative Y Fails");
	__CPROVER_assert(!RelativeDataValidityFailure_Z || ((Interstatus & 0x0040) == 0x0040), "Relative Z Fails");

	/********* Absolute limit intermittent failure conditions in X, Y, Z directions ***********/
	AbsoluteDataValidityIntermittentFailure_X = ((Sys1Count[0] > 0.625 || Sys1Count[0] < -0.625) && DataCheckModeValidity);
	AbsoluteDataValidityIntermittentFailure_Y = ((Sys1Count[1] > 0.625 || Sys1Count[1] < -0.625) && DataCheckModeValidity);
	AbsoluteDataValidityIntermittentFailure_Z = ((Sys1Count[2] > 0.625 || Sys1Count[2] < -0.625) && DataCheckModeValidity);

	/********* asserts for absolute limit intermittent failure conditions represented in bit0(X), bit1(Y), bit2(Z) of Interstatus word *********/
	__CPROVER_assert(!AbsoluteDataValidityIntermittentFailure_X || ((Interstatus & 0x0001) == 0x0001), "Absolute Intermittent X Fails");
	__CPROVER_assert(!AbsoluteDataValidityIntermittentFailure_Y || ((Interstatus & 0x0002) == 0x0002), "Absolute Intermittent Y Fails");
	__CPROVER_assert(!AbsoluteDataValidityIntermittentFailure_Z || ((Interstatus & 0x0004) == 0x0004), "Absolute Intermittent Z Fails");

}

void main()
{
	
	Sys1Count[0] = nondet_double();
	Sys1Count_old[0] = nondet_double();

	mode_word = nondet_unsignedint();

	__CPROVER_assume( mode_word == 11 || mode_word == 33 || mode_word == 44);
	
	CheckStatus();
	VerifCheckStatus();
 
	
}

