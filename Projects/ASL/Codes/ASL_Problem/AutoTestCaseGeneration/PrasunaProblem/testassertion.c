#include <stdio.h>

unsigned int Flag;
unsigned int PermFailure = 0, TempFailure =0; 
unsigned int FailureCount =0;

unsigned int nondet_unsignedint();
__CPROVER_bool VTempFail = 0, VPermFail= 0;
unsigned int VCount =0;

void  CheckStatus()
{
	if(Flag == 1)
	{
		
		FailureCount++;
		TempFailure = 1;
		
		if(FailureCount >= 4)
		{		
			PermFailure = 1;
		}
	}
	else
	{
		TempFailure = 0;
				
  		if(FailureCount < 4)
  			FailureCount = 0;			
  	}    
	
   	
}

Verif_fn()
{

	if (Flag ==1)
	{
		VTempFail =1;
		VCount++;
		if(VCount >= 4)
			VPermFail = 1;
	}
	else
	{

		VTempFail =0;
		VCount = 0;
	}

	__CPROVER_assert( VTempFail == TempFailure, "Temp Failure");
	__CPROVER_assert( VPermFail == PermFailure, "Permanent Failure");
}


void main()
{
	int i ;	
	//Modelling the Input Environment
	 for( i = 0; i < 5; i++) // loop is required to run the code atleast for 4 times in order to verify permanent failure
	 {
		Flag = nondet_unsignedint();

		CheckStatus();
		Verif_fn();
	}

}

