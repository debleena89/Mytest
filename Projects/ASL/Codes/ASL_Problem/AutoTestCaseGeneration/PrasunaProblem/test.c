/*******************English Specifications*********************************

Input Parameters : Flag 
Output Parametrs : TempFailure, PermFailure
Logic:
Temporary Failure: If "Flag" value is 1, declare temporary Failure and make TempFailure =1
Permanent Failure: If Temporary Failure happens for more than or equal to 4 times 
					then declare permanent Failure and make PermFailure = 1*/


#include <stdio.h>

unsigned int Flag;
unsigned int PermFailure = 0, TempFailure =0; 
unsigned int FailureCount;



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
  			FailureCount = 1;			
  	}    
	
   	
}

void main()
{
	CheckStatus();

}

