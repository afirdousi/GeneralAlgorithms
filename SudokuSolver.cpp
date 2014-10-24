// SudokoSolver.cpp : Defines the entry point for the console application.
//
#include<stdio.h>
#include<conio.h>
#include <stdlib.h>
#include <string.h>



#define EMPTY 0

 //Independant Functions:

		//001 :Finds next available/unassigned location in the puzzle
		int IsLocationAvailable(int sudoku[9][9], int &row, int &col);

		//002 :Checks if numToAssign is legal for current Row
		int IsCorrectRow(int sudoku[9][9],int row,int numToAssign);

		//003 :Checks if numToAssign is legal for current Column
		int IsCorrectCol(int sudoku[9][9],int col,int numToAssign);

		//004 :Checks if numToAssign is legal within the 3x3 portion/box it lies
		int IsCorrectBox(int sudoku[9][9],int boxStartX,int boxStartY, int numToAssign);

		//005 : Reads the input file and load from it
		int LoadDataFromFile(char fileName[200],int sudoku[9][9]);

		//006 : Writes the output to file
		void LoadDataToFile(int sudoku[9][9],int IsSolved);


 //Dependant Functions:

		//Checks if numToAssign is legal in this location 
		//This is a facade over Independant Functions 002,003,004
		int IsCorrectEntry(int sudoku[9][9],int row,int col,int numToAssign);

		//Fills up Sodoku table
		//Checks for Duplication Across Rows/Columsn/3x3 Boxes
		int SolveSudoku(int sudoku[9][9]);

//Utlity Functions:

		//Prints a 9 x 9 grid
		void Print(int sudoku[9][9]);


//The Sudoku Solver Application Driver

	int main(void) 
	{
		printf("\n\nINSIGHT DATA ENGINEERING FELLOWS PROGRAM \n");
		printf("**********************************************\n\n");

		int sudoku[9][9];
		/*
		Direct Input for Test Purposes:

		sudoku[9][9]={
							{0,3,5,2,9,0,8,6,4},
							{0,8,2,4,1,0,7,0,3},
							{7,6,4,3,8,0,0,9,0},
							{2,1,8,7,3,9,0,4,0},
							{0,0,0,8,0,4,2,3,0},
							{0,4,3,0,5,2,9,7,0},
							{4,0,6,5,7,1,0,0,9},
							{3,5,9,0,2,8,4,1,7},
							{8,0,0,9,0,0,5,2,6}
						};*/

		char fileName[200];
		puts("Please enter file to read Sudoku Puzzle");
		while(LoadDataFromFile(gets(fileName),sudoku)!=1){
			puts("File doesnot exist. Please enter correct file name.");
		}

		
		printf("\n\nOriginal Sodoku Problem\n");
		printf("****************************\n\n");

		Print(sudoku);

		if(SolveSudoku(sudoku))
		{
			printf("\n\nSodoku Solved\n");
			printf("******************\n\n");
			Print(sudoku);
			LoadDataToFile(sudoku,1);
		}
		else
		{
			printf("\nSodoku Unsolvable");
			LoadDataToFile(sudoku,0);
		}		

		getche();						
		return 0;	
	}


int SolveSudoku(int sudoku[9][9])
{
	int row,col;

	if(IsLocationAvailable(sudoku,row,col)!=1)
	{
		return 1;
	}
	//Filler = Number to fill in available space : Allowed Filler 1 - 9
	for(int Filler=1;Filler<10;Filler++)
	{
		//Try each filler value until one works
		if(IsCorrectEntry(sudoku,row,col,Filler)==1)
		{
			sudoku[row][col]=Filler; // Prelimenary Filler
				
			//Recursive Call
			if(SolveSudoku(sudoku))
			{
				return true; // Returns true and confirms the Filler was correct
			}

			//Setting up Backtrack. Unassigning Filler
			sudoku[row][col]=EMPTY;
		}
	}
	//Backtrack and check previous Filler values assigned to previous empty/available locations.
	return false;

}

int IsLocationAvailable(int sudoku[9][9], int &row, int &col)
{	
	for(row=0;row<9;row++)
	{
		for(col=0;col<9;col++)
		{
			if(sudoku[row][col]==EMPTY)
			{
				return 1;//Location available at current [row][col]
			}
		}
	}

	return 0; //All boxes are filled
}

int IsCorrectEntry(int sudoku[9][9],int row,int col,int numToAssign)
{
	return 
		IsCorrectRow(sudoku,row,numToAssign) && // numToAssign allowed in Row
		IsCorrectCol(sudoku,col,numToAssign) && // numToAssign allowed in Column
		IsCorrectBox(sudoku,row-row%3,col-col%3,numToAssign); // numToAssign allowed in Box
}

int IsCorrectRow(int sudoku[9][9],int row,int numToAssign)
{
	for(int col=0;col<9;col++)
	{
		if(sudoku[row][col]==numToAssign)
		{
			return false; // 'numToAssign' already exist in current row
		}
	}
	return true;
}

int IsCorrectCol(int sudoku[9][9],int col,int numToAssign)
{
	for(int row=0;row<9;row++)
	{
		if(sudoku[row][col]==numToAssign)
		{
			return false; // 'numToAssign' already exist in current column
		}
	}
	return true;

}

int IsCorrectBox(int sudoku[9][9],int boxStartX,int boxStartY, int numToAssign)
{
	for(int row=0;row<3;row++)
	{
		for(int col=0;col<3;col++)
		{	
			//Check if 'numToAssign' alraedy used in the 3x3 matrix it lies in
			if(sudoku[row+boxStartX][col+boxStartY]==numToAssign)
			{
				return 0;
			}
		}
	}

	return 1; //'numToAssign' does not already exist in it's 3x3 matrix
}

void Print(int sudoku[9][9])
{
	for(int row=0;row<9;row++)
	{
		for(int col=0;col<9;col++){
			printf("%3d",sudoku[row][col]);
		}
		printf("\n");

	}
	
}

int LoadDataFromFile(char fileName[200],int sudoku[9][9])
{
	int IsFileLoadedCorrectly=1;

	FILE *fp = fopen(fileName, "r") ;
	if ( fp != NULL ) 
    { 
	 
	 char buf[1024];
	 char *tok ;
	 int row=0; 
	 while (fgets(buf, sizeof(buf), fp)) {	
	 
			tok = strtok(buf, ",");
			for(int col=0;col<9;col++)
			{
				//printf("%d\n",atoi(tok));
				sudoku[row][col]=atoi(tok);
				tok = strtok(NULL, ",");
			}
			row++;
						
		}
	}
	else
	{
		IsFileLoadedCorrectly=0;
	}

	return IsFileLoadedCorrectly;
}

void LoadDataToFile(int sudoku[9][9],int IsSolved)
{
	FILE *fp = fopen("output.txt", "w+") ;
	if ( fp != NULL ) 
    {

		if(IsSolved)
		{
			fprintf(fp, "Solved Sudoku Puzzle \n");			
		}
		else
		{
			fprintf(fp, "Un Solvable Sudoku Puzzle \n");		
		}


		fprintf(fp, "******************************** \n\n");

		

		for(int row=0;row<9;row++)
		{
			for(int col=0;col<9;col++)
			{
				fprintf(fp, "%2d", sudoku[row][col]);
			}
			fprintf(fp, "\n");
		}
	}
	else
	{
		printf("There was error opening file!. The output file 'output.txt' cannot be created at this time.\n ");
	}

}


