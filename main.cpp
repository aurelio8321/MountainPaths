#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

// Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size. We'll learn
//   later how to put these arrays on the heap instead. Regardless, for now
//   we'll use these and treat them as if they were declared in main. So
//   when used in functions, we'll pass them as we would any other array.
static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
    //********************************************   Gets num of rows and columns
	int numRows = 0;
	int numCols = 0;
	cout << "Enter number of rows: ";
	cin >> numRows;
    if(numRows <= 0 || numRows > 1000){ 
        cout << "Error: Problem reading in rows and columns." << endl;
        return 1;
    }
	cout << endl;

	cout << "Enter number of columns: ";
	cin >> numCols;
    if(numCols <= 0 || numCols > 1000){
        cout << "Error: Problem reading in rows and columns." << endl;
        return 1;
    }
	cout << endl;
	//------------------------------------------


	//******************************  gets file Name
	string fileName = "";
	cout << "Enter file name: ";
	cin >> fileName;
	ifstream inFS;
	inFS.open(fileName);
	if(!inFS.is_open()){//checks if file was opened successfuly
		cout << "Error: Unable to open file <filename>." << endl;
		return 1;
	}
	
	//---------------------------------

	//******************************************checks and loads file data
	checkFile(numRows,numCols,inFS);
	inFS.close();

	inFS.open(fileName);
	loadData(elevations, numRows, numCols, inFS);
	//-----------------------------------------------------
	
	//**********************************************Finds min and max
	int min = 0;
	int max = 0;

	findMaxMin(elevations, numRows, numCols, max, min);
	//-------------------------------------------------------
	

	//********************************************************load greyscale

	loadGreyscale(image,elevations,numRows,numCols,max,min);

	//------------------------------------------

	inFS.close();

	//**********************************************OUTPUTS TO PPM FILE
	ofstream oFS;
	fileName = fileName + ".ppm";
	oFS.open(fileName);


	if(!oFS.is_open()){
		cout << "Error: Unable to open file <filename>." << endl;
		return 1;
	}

	
	
    //-----------------------------------------------------

    //***********************************************   COLORS THE PATHS
    int startRow = 0;
    /*cout << "Enter start row: ";
    cin >> startRow;
    */


   /* Pixel red;
    red.red = 252;
    red.green = 25;
    red.blue = 63;
	*/
	
	Pixel red;
	red.red = 252;
	red.green = 25;
	red.blue = 63;
	
	

    Pixel green;
    green.red = 31;
    green.green = 253;
    green.blue = 13;

	
	
    int shortestPath = colorPath(elevations, image, numRows, numCols, red, startRow);
    int shortestPathIndex = 0;
    int pathDistance = 0;

    for(int i = startRow; i < numRows; ++i){

        pathDistance = colorPath(elevations, image, numRows, numCols, red, i);
        //cout << "The distance: " << pathDistance << endl;

        if(pathDistance < shortestPath){
            shortestPath = pathDistance;
            shortestPathIndex = i;
        }

    }

   //cout << "Shortest path: " << shortestPath << endl;
   //cout << "Shortest path Index: " << shortestPathIndex << endl;

    colorPath(elevations,image,numRows,numCols,green, shortestPathIndex);

	outputImage(image,numRows,numCols, oFS);

	//cout << image[0][0].red << endl;


	oFS.close();
}
