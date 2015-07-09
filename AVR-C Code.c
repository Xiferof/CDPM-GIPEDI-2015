/*
Anirudh Bisht
Assumptions

the figures are 1:10 scale

*/
#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h> // Needed for USRART
#include<math.h>// Square and root

const int timeg = 20; // 1000 for testing 20 for motors

const float R=0.01;// Radii of Pulley
// Radii of pulley must have effect on pulling torque and Effectiveness of the motor to pull

const float Ht=10;// height of Standing Pole


// position variable c: current n: next d: 
int cpos[4]={30,30,30,30},dpos[4]={0,0,0,0};
int *npos; //Pointer due to being referenced by a function

struct OCS
{
	int x,y; 
};
int square (int x)
{
return x*x;
}

const int limit =20;// Limit of board 
//Coordinate conversion Function
//Basically Pythagorus theorem in 3D Plane
int* cordconv(struct OCS o)
{
	static int temp[4];
	temp[0]=sqrt(square(o.x)+square(o.y));
	temp[1]=sqrt(square(o.x)+square(o.y-limit));
	temp[2]=sqrt(square(o.x-limit)+square(o.y-limit));
	temp[3]=sqrt(square(o.x-limit)+square(o.y));
	return temp;
}


//Converts the distance of rope to angle of motor rotation required.
// This is Dynamic analysis of system without feedback
//Ideally encoders should be used to find the angle that motor has rotated
float dist2angle(int d)
{
	return ((float)d/R)*(180/3.14);
}



//Movement of motor
// This function moves the motor
// It takes in 2 parameters. 
// First is k which gives the number of the motor to be rotated.
// The second parameter is D which gives the distance or rope to be pulled or thrown
// D is integer with positive value meaning pull and negative meaning throw.


int movM(uint8_t k, int d)// PHysical motor movement function
{
	uint8_t i,j;
	printf("%d",d);
	int poles;
	if(d>0) //forward rotation
	{
	 float angle= dist2angle(d);// angle of rotation required to achive string lenght
	 //Note bipolar stepper motors were cheap market motors with given step angle of 1.8 varies with each motor
	 poles=angle/1.8;// each pole change causes 1.8" change in angle

	}
	//Negative rotation simply changes the order of excitation of poles of the stepper
	if(d<0)// Reverse Rotation
	{
		float angle=dist2angle(-d);// absolute angle of rotation
		poles=angle/1.8;
	
	}
	printf("%d",poles);
	return poles;
}

int main()
{
	
	printf("Program Starts \n");
	 uint8_t i,j; 
	 int k;
	 
	


//Coordinate system for new location.
struct OCS nu;
    
	
	char flag;
	//USARTWriteChar('D');
	int h;
	
	//Create movement array for test case
	
	//Test case movement is of grid block with 4x4 blocks movement required to center of each
	/*
	
	|X|X|X|X|
	_________
	|X|X|X|X|
	_________
	|X|X|X|X|
	_________
	|X|X|X|X|
	
	*/
	//Reuse h as Counter for Tracking which block to move too.
	
	h=-1;
	while(1) // Main loop infinite
	{
		h++;
		
		if (h==15)// This is end of Test case Start again for loop 
		  h=0;
		
		flag=0;
		
		//Check if any motor has to move. ie if all motor movements required=0
		// Then flag =0
		for(i=0;i<4;i++)
		{
			if(dpos[i]!=0)
			{flag=1;break;}
		}
		// If no movement required then	
		if(flag==0)
		{
			
			// Find coordinates of New position by getting from new h
			//Write to Port a
			//USARTWriteChar('X');
			nu.x=Te[h].x;
			//USARTWriteChar(nu.x+48);
			//USARTWriteChar('Y');
			
			nu.y=Te[h].y;
			//USARTWriteChar(nu.y+48);
			
			//Get new position of movement in terms of length coordinate system
			npos=cordconv(nu);
			
			//Find difference in position  and convert in terms of Coordinate system
			for(i=0;i<4;i++)
			{
				dpos[i]=*(npos+i)-cpos[i];
				//USARTWriteChar(*(npos+i)+48);
				//USARTWriteChar('*');
			}
			//USARTWriteChar('*');
		}
		//MOTOR MOVEMENT
		//Move all motors 
		for(i=0;i<4;i++)
		{
			if(dpos[i]!=0)
			{
				movM(i,dpos[i]/abs(dpos[i]));
				dpos[i]-=dpos[i]/abs(dpos[i]);
				cpos[i]+=dpos[i]/abs(dpos[i]);
			}
		}
		//************************		
		
	}
	return 0;
}
