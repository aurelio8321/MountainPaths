#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;


void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {
    min = elevations[0][0];
    max = elevations[0][0];
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            if(elevations[i][j] < min){
                min = elevations[i][j];
            }

            if(elevations[i][j] > max){
                max = elevations[i][j];
            }
        }
    }
    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;
}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
    
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            if(!inData.eof()){
                

                inData >> elevations[i][j];
                
                cout << elevations[i][j] << endl;
                
            }
            else{
                cout << i << " " << j << endl;
                if((i*j) < (rows*cols)){
                    cout << "Error: End of file reached prior to getting all the required data." << endl;
                    exit(1);
                }

                break;

            }

          
        }
    }
    
    


}

int scaleValue(int value, int max, int min) {
    double shade = 0.0; 
    shade = (1.0*(value-min)/(max-min))*255;

    if(fmod(shade,1.0) >= 0.5){
        shade = shade-fmod(shade,1.0);
        shade += 1;

    }
    else{
        shade = shade-fmod(shade,1.0);
    }

    int greyVal = shade;
    return greyVal;
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int max, int min) {

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            image[i][j].red = scaleValue(elevations[i][j],max, min);
            image[i][j].green = scaleValue(elevations[i][j],max,min);
            image[i][j].blue = scaleValue(elevations[i][j],max,min);
        }
    }

}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
    outData << "P3" << endl;
	outData << cols << " " << rows << endl;
	outData << "255" << endl;
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            outData << image[i][j].red << " ";
            outData << image[i][j].green << " ";
            outData << image[i][j].blue << " ";
        }
        cout << endl;
    }
}

void checkFile(int rows, int cols,istream& inData){

    int k = 0;
    string piece = "";

    while(inData >> piece){
        k += 1;

        if(isalpha(piece[0])){//checks if data is alphabetic
            cout << "Error: Read a non-integer value." << endl;
            exit(1);
        }

        if(ispunct(piece[0])){//checks if data is punctuation
            cout << "Error: Read a non-integer value." << endl;
            exit(1);
        }

    }

    if( (k) > (rows*cols)){//Checks if too many data points
        cout << "Error: Too many data points." << endl;
        exit(1);
    }
    


}

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row){
    int pathLength = 0;
    //*************************** SETS FIRST PIXEL OF ROW TO COLOR
    image[start_row][0].red = color.red;
    image[start_row][0].green = color.green;
    image[start_row][0].blue = color.blue;
    //---------------------------------------

    //************************DIFFERENCE IN ELEVATION IN 3 DIRECTIONS
    int NEast;
    int East;
    int SEast;

    int currRow = start_row;

    for(int j = 0; j < cols-1; ++j){
        if(currRow == 0){// we can only go East or SE
            East = abs(elevations[currRow][j] - elevations[currRow][j+1]);
            SEast = abs(elevations[currRow][j] - elevations[currRow+1][j+1]);

            if(East <= SEast){//WE GO EAST

                pathLength += abs(elevations[currRow][j] - elevations[currRow][j+1]);

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
            }
            else{//WE GO SOUTH EAST

                pathLength += abs(elevations[currRow][j] - elevations[currRow+1][j+1]);
                currRow += 1;

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
            }


        }
        else if((currRow+1) == rows){//we can only go East or NE
            NEast = abs(elevations[currRow][j] - elevations[currRow-1][j+1]);//ERROR IF ROW =0
            East = abs(elevations[currRow][j] - elevations[currRow][j+1]);

            if(East <= NEast){// WE GO EAST

                pathLength += abs(elevations[currRow][j] - elevations[currRow][j+1]);

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
            }
            else{//WE GO NORTH EAST

                pathLength += abs(elevations[currRow][j] - elevations[currRow-1][j+1]);

                currRow -= 1;

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
            }
        }
        else{
            NEast = abs(elevations[currRow][j] - elevations[currRow-1][j+1]);//ERROR IF ROW =0
            East = abs(elevations[currRow][j] - elevations[currRow][j+1]);
            SEast = abs(elevations[currRow][j] - elevations[currRow+1][j+1]);//ERROR IF LAST ROW

            if(East <= NEast && East <= SEast){//we go east

                pathLength += abs(elevations[currRow][j] - elevations[currRow][j+1]);

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;

            }
            else if (SEast <= NEast){//we go south east

                pathLength += abs(elevations[currRow][j] - elevations[currRow+1][j+1]);

                currRow += 1;

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
                
            }
            else{//we go NorthEast

                pathLength += abs(elevations[currRow][j] - elevations[currRow-1][j+1]);

                currRow -= 1;

                image[currRow][j+1].red = color.red;
                image[currRow][j+1].green = color.green;
                image[currRow][j+1].blue = color.blue;
            } 
        }


       

        

    }
    
    return pathLength;

    //-------------------------
}
