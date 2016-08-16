#include <stdio.h>
#include <math.h>

double nondet_double();
unsigned int nondet_unsignedint();
unsigned int TimeoutFlag, HeaderValidFlag, chksumValidFlag, DataValid, mode_word,Pack_Fail,sflag;
unsigned int Switchover, Sys1FailFlag[3], Sys2FailFlag[3],Interstatus=0, overall_status =0;
double Sys1Count[3] = { 0 };
double Sys1Count_old[3] = { 0 };
double Sys2Count[3] = { 0 };
double Sys2Count_old[3] = { 0 };
 unsigned int DataInvalidCount, Sys1RelLimitFail[3],Sys1AbsLimitFail[3],Sys1DataInvalidCount[3];

/*********************** globals for modelling the assertions************************/
__CPROVER_bool AbsPermFailureX = 0;
__CPROVER_bool AbsPermFailureY = 0;
__CPROVER_bool AbsPermFailureZ = 0;
__CPROVER_bool PackageFail = 0;

__CPROVER_bool RelativeDataFailure_X = 0;
__CPROVER_bool RelativeDataFailure_Y = 0;
__CPROVER_bool RelativeDataFailure_Z = 0;

__CPROVER_bool AbsoluteDataIntermittentFailure_X = 0;
__CPROVER_bool AbsoluteDataIntermittentFailure_Y = 0;
__CPROVER_bool AbsoluteDataIntermittentFailure_Z = 0;

__CPROVER_bool PackageIntermittentFailure = 0;

__CPROVER_bool PackageModeValidity;
__CPROVER_bool DataCheckModeValidity;

/***************** end of global assertion variables ****************************/

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

		if (DataInvalidCount < 20)//bug//NOassertions failed
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
      
     	
		if( (Sys1AbsLimitFail[i] == 1) && (Pack_Fail == 0) && ((mode_word== 11) || (mode_word == 44)) )
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

void VerifCheckStatus( double Count1_old[], double Count2_old[])
{
	int i; 
	static int DataSumX = 0;
	static int DataSumY = 0;
	static int DataSumZ = 0;
	static int PackageSum = 0;
	__CPROVER_bool NewtoOld = 0;
	__CPROVER_bool Fail = 0;

	/***************** start of Package Validity Checks*****************************/

		/**** Appliacble modes for package validity checks*************/
		PackageModeValidity = (mode_word == 11 || mode_word == 44);

		/*** package intermittent failure conditions****************/

		PackageIntermittentFailure = (((TimeoutFlag == 1) || (HeaderValidFlag == 0) || (chksumValidFlag == 0) || (DataValid == 0)) && (PackageModeValidity));

		/********** asserts for package validity ****************/

		NewtoOld = ((Sys1Count[0] == Count1_old[0]) && (Sys1Count[1] == Count1_old[1]) && (Sys1Count[2] == Count1_old[2]) &&
		(Sys2Count[0] == Count2_old[0]) && (Sys2Count[1] == Count2_old[1]) && (Sys2Count[2] == Count2_old[2]));
	
		/*****  pacakge Intermittent failure assert***********/

		__CPROVER_assert(!PackageIntermittentFailure || NewtoOld, "Package- Intermittent Failure");

	/****** conditions for Package permanent Failures****************/
		if(PackageIntermittentFailure ==1)
		{
			PackageSum++;
			Fail = 1;
		}
		else
		{
			PackageSum = 0;
			Fail = 0;
		}

		if (PackageSum >= 20)
			PackageFail = 1;
		else
			PackageFail = 0;
	/***********assert  statements for package permanent failure********************/
		if (PackageFail == 1)
        {
                __CPROVER_assert(!PackageFail || ((overall_status & 0x0de0) == 0x0de0), "Package - Permanent Failure");
                __CPROVER_assert(!PackageFail || ((Sys1FailFlag[0]==1) && (Sys1FailFlag[1]==1) && (Sys1FailFlag[2] ==1) 
                                                        && (Sys2FailFlag[0]==1) && (Sys2FailFlag[1]==1) && (Sys2FailFlag[2]==1)),
                                                                                        "Package- Permanent Failure");
        }
	/********************* End of Package Validity checks************************/

    /********************* Start of Data validity checks************************/

        /**** Appliacble modes for data validity checks*************/
        DataCheckModeValidity = (mode_word == 11 || mode_word == 33 || mode_word == 44);

        /********* Relative limit check failure conditions in X, Y, Z directions ***********/
        RelativeDataFailure_X = (((fabs(Sys1Count[0] - Count1_old[0])) > 0.0489) && DataCheckModeValidity);
        RelativeDataFailure_Y = (((fabs(Sys1Count[1] - Count1_old[1])) > 0.0489) && DataCheckModeValidity);
        RelativeDataFailure_Z = (((fabs(Sys1Count[2] - Count1_old[2])) > 0.0489) && DataCheckModeValidity);

		/********* asserts for relative limit failure conditions represented in bit4(X), bit5(Y), bit6(Z) of Interstatus word *********/
        __CPROVER_assert(!RelativeDataFailure_X || ((Interstatus & 0x0010) == 0x0010), "Relative X Fails");
        __CPROVER_assert(!RelativeDataFailure_Y || ((Interstatus & 0x0020) == 0x0020), "Relative Y Fails");
        __CPROVER_assert(!RelativeDataFailure_Z || ((Interstatus & 0x0040) == 0x0040), "Relative Z Fails");


		/************Absolute limit intermittent failure x, y, z directions**************************/
		AbsoluteDataIntermittentFailure_X = ((Sys1Count[0] > 0.625 || Sys1Count[0] < -0.625) && DataCheckModeValidity && Switchover == 0);
		AbsoluteDataIntermittentFailure_Y = ((Sys1Count[1] > 0.625 || Sys1Count[1] < -0.625) && DataCheckModeValidity);
		AbsoluteDataIntermittentFailure_Z = ((Sys1Count[2] > 0.625 || Sys1Count[2] < -0.625) && DataCheckModeValidity);

        /********* asserts for absolute limit intermittent failure conditions represented in bit0(X), bit1(Y), bit2(Z) of Interstatus word *********/
       __CPROVER_assert(!AbsoluteDataIntermittentFailure_X || ((Interstatus & 0x0001) == 0x0001), "Absolute Intermittent X Fails");
       __CPROVER_assert(!AbsoluteDataIntermittentFailure_Y || ((Interstatus & 0x0002) == 0x0002), "Absolute Intermittent Y Fails");
       __CPROVER_assert(!AbsoluteDataIntermittentFailure_Z || ((Interstatus & 0x0004) == 0x0004), "Absolute Intermittent Z Fails");

	   /************Absolute data permanent failure conditions ************************/
		if ((AbsoluteDataIntermittentFailure_X == 1) && ( (Fail == 0 && ( mode_word == 11 || mode_word == 44)) || mode_word ==33))
					DataSumX++;
			else
					DataSumX = 0;

		if ((AbsoluteDataIntermittentFailure_Y == 1) && ( (Fail == 0 && ( mode_word == 11 || mode_word == 44)) || mode_word ==33))
					DataSumY++;
			else
					DataSumY = 0;

		if ((AbsoluteDataIntermittentFailure_Z == 1) && ( (Fail == 0 && ( mode_word == 11 || mode_word == 44)) || mode_word ==33))
					DataSumZ++;
			else
					DataSumZ = 0;

        if (DataSumX >= 10)
                AbsPermFailureX = 1;
		else
			AbsPermFailureX = 0;

        if (DataSumY >= 10)
                AbsPermFailureY = 1;
		else
			AbsPermFailureY = 0;

        if (DataSumZ >= 10)
                AbsPermFailureZ = 1;
		else
			AbsPermFailureZ = 0;

		/*********Asserts for absolute data limit permanent failure***********/	
		if (AbsPermFailureX == 1 || AbsPermFailureY == 1 || AbsPermFailureZ == 1)
		{
 			__CPROVER_assert(!AbsPermFailureX || ((overall_status & 0x0100) == 0x0100), "Absolute Permanent X Fails");
			__CPROVER_assert(!AbsPermFailureY || ((overall_status & 0x0400) == 0x0400), "Absolute Permanent Y Fails");
			__CPROVER_assert(!AbsPermFailureZ || ((overall_status & 0x0800) == 0x0800), "Absolute Permanent Z Fails");
			__CPROVER_assert(!AbsPermFailureX || (Sys1FailFlag[0]==1),"Absolute- Permanent X Failure");
			__CPROVER_assert(!AbsPermFailureY || (Sys1FailFlag[1]==1),"Absolute- Permanent Y Failure");
			__CPROVER_assert(!AbsPermFailureZ || (Sys1FailFlag[2]==1),"Absolute- Permanent Z Failure");
		}

	/*******resetting flags at the end of current run************/
		PackageIntermittentFailure = 0;
		AbsoluteDataIntermittentFailure_X == 0 ;
		AbsoluteDataIntermittentFailure_Y == 0 ;
		AbsoluteDataIntermittentFailure_Z == 0 ;
	
	/****************** old buffer needs to be updated with the current******************/
	for (i = 0; i < 3; i++)
	{
		Sys1Count_old[i] = Sys1Count[0];
		Sys2Count_old[i] = Sys2Count[0];
	}
	
	
}

void main()
{
	int i, k,j;
	double Count1_old[3]= {0};
	double Count2_old[3]= {0};

	mode_word = nondet_unsignedint(); // Mode value is assigned outside the for loop, to make it constant in consecutive runs
	
	__CPROVER_assume(mode_word == 11 || mode_word == 33 || mode_word == 44); // valid values for mode_word

	for(i = 0; i < 25; i++) // This loop is required to execute the code for more than 20 times as some asserts are valid only after 20 iterations. 
	{
		for (k = 0; k < 3; k++)
		{

			Sys1Count[k] = nondet_double();
			Sys2Count[k] = nondet_double();

			__CPROVER_assume(__CPROVER_isfinited(Sys1Count[k]));
			__CPROVER_assume(__CPROVER_isfinited(Sys2Count[k]));
		}

		Switchover = nondet_unsignedint();
		TimeoutFlag = nondet_unsignedint();
		HeaderValidFlag = nondet_unsignedint();
		chksumValidFlag = nondet_unsignedint();
		DataValid = nondet_unsignedint();

		for(j =0; j < 3; j++)
		{
			Count1_old[j] = Sys1Count_old[j]; // this is necessary as Sys1Count_old gets updated in the CheckStatus function
			Count2_old[j] = Sys2Count_old[j];
		}
		
		CheckStatus();
		VerifCheckStatus(Count1_old, Count2_old);
	}	
}

