#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
//Matrix of ROWS & COLUMNS
const int ROWS = 7;
const int COLUMNS = 7;
//Directions the robot can go and their probabilities
float STRAIGHT = 0.75;
float RIGHT = 0.10;
float LEFT = 0.15;
//Probabilities for
//Correctly detecting obstacle
float detectObstacle = 0.90;
//Doesn’t detect the obstacle
float missDetectObstacle = 0.10;
//Correctly detects an open square
float correctOpen = 0.95;
//Mistake open square for an obstacle
float missOpen = 0.05;
//Matrix to hold probabilities During filtering
float matrixProb[ROWS][COLUMNS];
//Matrix to hold Motion probabilities
float motionMatrix[ROWS][COLUMNS];
//Maze, 1 represents Obstacle, 0 represents open square
int maze[ROWS][COLUMNS] = {
	{1, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 1},
	{1, 1, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 1, 1},
	{1, 0, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 0, 1},
};
//Function to calculate sensing 
float sense(int s[4], int i, int j);
//Function to calculate motion
void motion(int facing);
//Function to calculate the conditional probability during sensing
void filter(int s[4]);
//Function to print matrices to screen
void print(float myMaze[ROWS][COLUMNS]);

int main() {
//Initialize the probability matrix. 19 possible open squares, each 1/19 probability of 
//starting. Else that square is blocked
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (maze[i][j] == 0) {matrixProb[i][j] = (1.0 / 19);}
			else {matrixProb[i][j] == -1;}}}
	print(matrixProb);
	cout << endl;
//array: [West, North, East, South]
//1: Blocks, 0: Open
	int sense1[4] = { 1, 0, 1, 0 };
//Call filter function with newly created array of values
	filter(sense1);
	cout << "Filtering after Evidence [1, 0, 1, 0]" << endl;
	print(matrixProb);
	cout << endl;
//Move North
	motion(1);
	cout << "Prediction after Action N" << endl;
	print(matrixProb);
	cout << endl;
	int sense2[4] = { 0, 0, 0, 0 };
//Call filter function with newly created array of values
	filter(sense2);
	cout << "Filtering after Evidence [0, 0, 0, 0]" << endl;
	print(matrixProb);
	cout << endl;
//Move East
	motion(2);
	cout << "Prediction after Action E" << endl;
	print(matrixProb);
	cout << endl;
	int sense3[4] = { 0, 1, 0, 1 };
//Call filter function with newly created array of values
	filter(sense3);
	cout << "Filtering after Evidence [ 0, 1, 0, 1]" << endl;
	cout << endl;
	print(matrixProb);
	cout << endl;
	cout << "Prediction after Action E" << endl;
//Move East
	motion(2);
	print(matrixProb);
	cout << endl;
	int sense4[4] = { 0, 0, 1, 1 };
//Call filter function with newly created array of values
	filter(sense4);
	cout << "Filtering after Evidence [0, 0, 1, 1]" << endl;
	print(matrixProb);
	cout << endl;
	cout << "Prediction after Action N" << endl;
//Move North
	motion(1);
	print(matrixProb);
	cout << endl;
	int sense5[4] = { 1, 0, 1, 0 };
//Call filter function with newly created array of values
	filter(sense5);
	cout << "Filtering after Evidence [1, 0, 1, 0]" << endl;
	print(matrixProb);
	cout << endl;
	return 0;}

//Function to perform sensing calculations for given sense array
float sense(int s[4], int i, int j) {
//Initalize values to store each direction and multiply together at end
float s0, s1, s2, s3;
//S: array of evidence. [West, North, East, South]
// Calculate West
	if (s[0] == 1) {
//Detect Obstacle
		if (maze[i][j - 1] == 1) { s0 = detectObstacle;}
//Miss Open Square believing it is an obstacle
		else { s0=  missOpen;}
//Displayed for debugging purposes
		    //cout << "s0: " << s0 << endl;
}
	else {
//Doesn’t detect the obstacle
		if (maze[i][j - 1] == 1) { s0=  missDetectObstacle;}
//Correctly Detect an Open Square
		else { s0=  correctOpen;}
		    //cout << "s0: " << s0 << endl;
}
// Calculate North
	if (s[1] == 1) {
//Detect Obstacle
	    if(maze[i - 1][j] == 1) { s1=  detectObstacle;}
//Miss Open Square believing it is an obstacle
		else { s1=  missOpen;}
		    //cout << "s1: " << s1 << endl;
	}
	else {
//To ensure doesn’t loop around for value if at beginning: Doesn’t detect the obstacle
	    if(i==0){s1 = missDetectObstacle;}
//Doesn’t detect the obstacle
		else if (maze[i - 1][j] == 1) { s1=  missDetectObstacle;}
//Correctly Detect an Open Square
		else { s1=  correctOpen;}
		    //cout << "s1: " << s1 << endl;
}
//Calculate East
	if (s[2] == 1) {
//Detect Obstacle
		if (maze[i][j + 1] == 1) { s2=  detectObstacle;}
//Miss Open Square believing it is an obstacle
		else { s2=  missOpen;}
	    		    //cout << "s2: " << s2 << endl;
	}
	else {
//Doesn’t detect the obstacle
		if (maze[i][j + 1] == 1) { s2=  missDetectObstacle;}
//Correctly Detect an Open Square
		else { s2=  correctOpen;}
		    //cout << "s2: " << s2 << endl;
}
//Calculate South
	if (s[3] == 1) {
//Detect Obstacle
		if (maze[i + 1][j] == 1) { s3=  detectObstacle;}
//Miss Open Square believing it is an obstacle
		else { s3 =  missOpen;}
		    //cout << "s3: " << s3 << endl;
}
	else {
//Doesn’t detect the obstacle
	    if(i == 6){s3 = missDetectObstacle;}
		else if (maze[i + 1][j] == 1) { s3=  missDetectObstacle;}
//Correctly Detect an Open Square
		else { s3=  correctOpen;}
		    //cout << "s3: " << s3 << endl;
}
//Multiply probabilities together to send back to be multiplied by probability matrix
float prob = s0*s1*s2*s3;
//cout << prob << endl;
return prob;
}

//Motion Function after robot moves in a direction
//Facing parameter represents the direction the robot is facing at the time
void motion(int facing) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (maze[i][j] != 1) {
//Initialize a variable to hold the calculated information
				float totalProbability = 0.0;
//If Facing West
				if (facing == 0) {
//If square on Left is not blocked, multiply  Left value by 0.15 and add total to total probability
					if (maze[i + 1][j] != 1) {totalProbability += matrixProb[i + 1][j] * LEFT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * LEFT;}
//If square in front of square is not blocked, do nothing
					if (maze[i][j - 1] != 1) {totalProbability += matrixProb[i][j - 1] * 0;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * STRAIGHT;}
//If square to the right is not blocked, multiply that square by 0.10
					if (maze[i - 1][j] != 1) {totalProbability += matrixProb[i - 1][j] * RIGHT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * RIGHT;}
//If square behind current is not blocked, multiply that value by 0.75
					if (maze[i][j + 1] != 1) {totalProbability += matrixProb[i][j + 1] * STRAIGHT;}
//Else is it blocked and bounces back, do nothing
					else {totalProbability += matrixProb[i][j] * 0;}}
//If Facing North
				if (facing == 1) {
//If square to the Left is not blocked, multiply that square by 0.15
				    if (maze[i][j - 1] != 1) {totalProbability += matrixProb[i][j - 1] * LEFT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * LEFT;}
//If square behind current is not blocked, multiply that value by 0.75
				    if(i == 0){ totalProbability += matrixProb[i][j] * STRAIGHT;}
//Else is it blocked and bounces back, do nothing
					else if (maze[i - 1][j] != 1) {totalProbability += matrixProb[i - 1][j] * 0;}
//Else is it blocked and bounces back, must multiply self by 0.75
					else {totalProbability += matrixProb[i][j] * STRAIGHT;}
//If square to the right is not blocked, multiply that square by 0.10
					if (maze[i][j + 1] != 1) {totalProbability += matrixProb[i][j + 1] * RIGHT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * RIGHT;}
//If square behind current is not blocked, multiply that value by 0.75
					if (maze[i + 1][j] != 1) {totalProbability += matrixProb[i + 1][j] * STRAIGHT;}
//Else is it blocked and bounces back, do nothing
					else {totalProbability += matrixProb[i][j] * 0;}}
//If Facing East
				if (facing == 2) {
//If square to the Left is not blocked, multiply that square by 0.15
					if (maze[i - 1][j] != 1) {totalProbability += matrixProb[i - 1][j] * LEFT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * LEFT;}
//If square behind current is not blocked, multiply that value by 0.75
					if (maze[i][j + 1] != 1) {totalProbability += matrixProb[i][j + 1] * 0;}
//Else is it blocked and bounces back, must multiply self by 0.75
					else {totalProbability += matrixProb[i][j] * STRAIGHT;}
//If square to the right is not blocked, multiply that square by 0.10
					if (maze[i + 1][j] != 1) {totalProbability += matrixProb[i + 1][j] * RIGHT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * RIGHT;}
//If square behind current is not blocked, multiply that value by 0.75
					if (maze[i][j - 1] != 1) {totalProbability += matrixProb[i][j - 1] * STRAIGHT;}
//Else is it blocked and bounces back, do nothing
					else {totalProbability += matrixProb[i][j] * 0;}}
//If Facing South
				if (facing == 3) {
//If square to the Left is not blocked, multiply that square by 0.15
					if (maze[i][j + 1] != 1) {totalProbability += matrixProb[i][j + 1] * LEFT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * LEFT;}
//Else is it blocked and bounces back, do nothing
					if (maze[i + 1][j] != 1) {totalProbability += matrixProb[i + 1][j] * 0;}
//Else is it blocked and bounces back, must multiply self by 0.75
					else {totalProbability += matrixProb[i][j] * STRAIGHT;}
//If square to the right is not blocked, multiply that square by 0.10
					if (maze[i][j - 1] != 1) {totalProbability += matrixProb[i][j - 1] * RIGHT;}
//Else is it blocked and bounces back
					else {totalProbability += matrixProb[i][j] * RIGHT;}
//If square behind current is not blocked, multiply that value by 0.75
					if (maze[i - 1][j] != 1) {totalProbability += matrixProb[i - 1][j] * STRAIGHT;}
//Else is it blocked and bounces back, do nothing
					else {totalProbability += matrixProb[i][j] * 0;}}
//Assign motion matrix total probability values
				motionMatrix[i][j] = totalProbability;}
//else is blocked
			else {motionMatrix[i][j] = -1;}}}
//iterate through array to assign probability matrix values for the motion
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {matrixProb[i][j] = motionMatrix[i][j];}}}

//Function to perform filtering during sensing stage, completes conditional probability
//Calculations
void filter(int s[4]) {
	float d = 0.00;
//Iterate through matrix
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
//If not blocked
			if (maze[i][j] != 1) {
			    //cout << "Matrix values before calling function" << endl;
//Call Sense function to to perform first stage of conditional probability calculations
			    float currentProb = sense(s, i, j);
			   // cout << "after calling sense: " << currentProb << endl;
//Multiply value in current matrix position by sense probabilities
			    matrixProb[i][j] = matrixProb[i][j] * currentProb;
			    //cout << "Prob times previous matrix value: " << matrixProb[i][j] << endl;
}    
		}}
//Iterate through matrix
			for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (maze[i][j] != 1) {
//Sum matrix values
        d += matrixProb[i][j];
        //cout << "d: " << d << endl;
        }}}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (maze[i][j] != 1) {
//Calculate each matrix value over the summation to complete conditional probability for each square
				float answer = matrixProb[i][j] / d;
//Assign new values to probability matrix
				matrixProb[i][j] = answer;
}}}}

//Function to print matrix to screen
void print(float myMaze[ROWS][COLUMNS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
//If square is blocked, display 4 # signs
			if (myMaze[i][j] == -1) {cout << "#### ";}
///Else if initial values were 0.00 pound signs (for blocked squares)
			else if (myMaze[i][j] == 0.00) {cout << "#### ";}
//Else display the value to 2 decimal places rounded
			else {cout <<  fixed << setprecision(2) << myMaze[i][j] * 100 << " ";}}
		cout << endl;}}
