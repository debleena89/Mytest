/***Bug is at line no.-44,  assertions for Package Validity did not Fail***/
/*****************************************************************************/


#include <stdio.h>
#include <math.h>

double nondet_double();
unsigned int nondet_unsignedint();
unsigned int TimeoutFlag, HeaderValidFlag, chksumValidFlag, DataValid, mode_word,Pack_Fail,sflag;
unsigned int Switchover, Sys1FailFlag[3], Sys2FailFlag[3],Interstatus, overall_status;
double Sys1Count[3] = { 0, 0, 0 };
double Sys1Count_old[3] = { 0, 0, 0 };
double Sys2Count[3] = { 0, 0, 0 };
double Sys2Count_old[3] = { 0, 0, 0 };
static unsigned int DataInvalidCount, Pack_fail, Sys1RelLimitFail[3],Sys1AbsLimitFail[3],Sys1DataInvalidCount[3];

/*********************** globals for modelling the assertions************************/
__CPROVER_bool AbsPermFailureX = 0;
__CPROVER_bool AbsPermFailureY = 0;
__CPROVER_bool AbsPermFailureZ = 0;
__CPROVER_bool PackageFail = 0;

__CPROVER_bool RelativeDataFailure_X = 0;
__CPROVER_bool RelativeDataFailure_Y = 0;
__CPROVER_bool RelativeDataFailure_Z = 0;

__CPROVER_bool AbsoluteDataIntermittentFailure_X[25] = {0};
__CPROVER_bool AbsoluteDataIntermittentFailure_Y[25] = {0};
__CPROVER_bool AbsoluteDataIntermittentFailure_Z[25] = {0};

__CPROVER_bool PackageIntermittentFailure[25] = {0};

__CPROVER_bool PackageModeValidity;
__CPROVER_bool DataCheckModeValidity;

/***************** end of global assertion variables ****************************/

void  CheckStatus()
{

	int i;
	
	if (((TimeoutFlag == 1) || (HeaderValidFlag == 1) || (chksumValidFlag == 0) || (DataValid == 0)) && ((mode_word == 11) || (mode_word == 44)))//bug
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
      
     	
		if( (Sys1AbsLimitFail[i] == 1) && ((mode_word== 11) || (mode_word == 44)) )
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

void VerifCheckStatus(int cycle)
{
	int i; 
	static int DataSumX = 0;
	static int DataSumY = 0;
	static int DataSumZ = 0;
	static int PackageSum = 0;
	__CPROVER_bool NewtoOld = 0;

	/***************** start of Package Validity Checks*****************************/

		/**** Appliacble modes for package validity checks*************/
	PackageModeValidity = (mode_word == 11 || mode_word == 44);

	/*** package validity conditions for intermittent failure****************/

	PackageIntermittentFailure[cycle] = ((TimeoutFlag == 1) && (HeaderValidFlag == 0) && (chksumValidFlag == 0) && (DataValid == 0) && (PackageModeValidity));

	/********** asserts for package validity ****************/

	NewtoOld = ((Sys1Count[0] == Sys1Count_old[0]) && (Sys1Count[1] == Sys1Count_old[1]) && (Sys1Count[2] == Sys1Count_old[2]) &&
		(Sys2Count[0] == Sys2Count_old[0]) && (Sys2Count[1] == Sys2Count_old[1]) && (Sys2Count[2] == Sys2Count_old[2]));
	
	/*****  pacakge Intermittent failure assert***********/
	__CPROVER_assert(!PackageIntermittentFailure[cycle] || NewtoOld, "Package- Intermittent Failure");

	/*** package validity conditions for permanent failure****************/
	if (PackageIntermittentFailure[cycle] == 1)
		PackageSum++;
	else
		PackageSum = 0;

	if (PackageSum >= 20)
		PackageFail = 1;
	/*****  pacakge permanent failure assert bits 5(sys2 X),6(sys2 Y),7(sys2 Z),8(sys1 X),10(sys1 Y),11(sys1 Z)***********/
	if (cycle >= 19)
	{
		__CPROVER_assert(!PackageFail || ((overall_status & 0x0de0) == 0x0de0), "Package - Permanent Failure");
		__CPROVER_assert(!PackageFail || ((Sys1FailFlag[0]==1) && (Sys1FailFlag[1]==1) && (Sys1FailFlag[2]==1) 
											&& (Sys2FailFlag[0]==1) && (Sys2FailFlag[1]==1) && (Sys2FailFlag[2]==1)),
											"Package- Permanent Failure");

	}
	/********************* End of Package Validity checks************************/
	
	/********************* Start of Data validity checks************************/

	/**** Appliacble modes for data validity checks*************/
	DataCheckModeValidity = (mode_word == 11 || mode_word == 33 || mode_word == 44);

	

	/**** conditions for absolute limit permannet failure**************/
	
	/********************* End of Data Validity checks************************/
	/****************** old buffer needs to be updated with the current******************/
	for (i = 0; i < 3; i++)
	{
		Sys1Count_old[i] = Sys1Count[0];
		Sys2Count_old[i] = Sys1Count[0];
	}
	
	
}

void main()
{
	int i, k;

	for (i=0; i<24; i++) // this is to run the code and assertions for 25 cycles to check the consecutive failures. 
						//The array size of the buffers used in modelling assertions is based on this loop control variable
	{
		for (k = 0; k < 3; k++)
		{

			Sys1Count[k] = nondet_double();
			Sys2Count[k] = nondet_double();

			__CPROVER_assume(__CPROVER_isfinited(Sys1Count[k]));
			__CPROVER_assume(__CPROVER_isfinited(Sys2Count[k]));
		}

		mode_word = nondet_unsignedint();
		Switchover = nondet_unsignedint();
		TimeoutFlag = nondet_unsignedint();
		HeaderValidFlag = nondet_unsignedint();
		chksumValidFlag = nondet_unsignedint();
		DataValid = nondet_unsignedint();

		__CPROVER_assume(mode_word == 11 || mode_word == 33 || mode_word == 44);

		CheckStatus();
		VerifCheckStatus(i);
	}
	
}

