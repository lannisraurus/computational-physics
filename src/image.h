/*

Here we have all the implemented functions from Homework 1.

We have decided to approach this problem using structs and functions.
All the functions take arguments by copy, meaning that types are "static" in the sense that
to change them one must set them equal to something else.

*/

///////////////////////////////////////////////////////////////////////// INCLUDE DIRECTORIES

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>

// We are not using any other namespaces.
using namespace std; 

///////////////////////////////////////////////////////////////////////// BASIC UTILITIES

// Data structure for the pgm matrix and max colour value
struct pgm{
    vector<vector<int>> matrix;
    int maxValue, wid, hei;
};


// Opens a file, returning a matrix containing PGM information.
pgm loadFile(string fileName){ 

    // Needed variables
    ifstream file(fileName);
    string line;
    stringstream stringStream;
    pgm resultMatrix;

    // Extract first line of the file - useless (it's just the image format)
    getline(file, line);

    // Extract second line of the file - width and height
    getline(file, line);
    stringStream << line;
    stringStream >> resultMatrix.wid >> resultMatrix.hei;
    stringStream.clear();

    // Extract third line of the file - max value
    getline(file, line);
    stringStream << line;
    stringStream >> resultMatrix.maxValue;
    stringStream.clear();

    // Fills the matrix with a *height* number of rows (empty).
    resultMatrix.matrix = vector<vector<int>>(resultMatrix.hei);

    // Fills the matrix with pixel data
    int row = 0, collumn = 0, pixel;
    while (row < resultMatrix.hei){
        file >> pixel;
        resultMatrix.matrix[row].push_back(pixel);
        if (collumn == resultMatrix.wid-1) {
            row++;
            collumn = 0;
        }else{
            collumn++;
        }
    }
    
    // Close file and return the newfound matrix
    file.close();
    return resultMatrix;
}


// Creates a new pgm file from the pgm matrix struct
void writeFile(pgm inputMatrix, string fileName){ 

    //needed variables
    ofstream file(fileName);
    
    //write file
    file << "P2\n";
    file << inputMatrix.wid << " " << inputMatrix.hei << "\n";
    file << inputMatrix.maxValue << "\n";
    for(int i = 0; i < inputMatrix.hei; i++){
        for(int j = 0; j < inputMatrix.wid; j++){
            file << inputMatrix.matrix[i][j] << " ";
        }
    }
    file.close();

}

///////////////////////////////////////////////////////////////////////// STATISTICS

// Returns a maxValue size vector; each position corresponds to a colour, and v[i] = number of pixels with colour value i.
vector<int> colourFrequency(pgm image){

    // needed variables
    vector<int> colourSpace(image.maxValue+1);

    // Fill vector
    for (int i = 0; i < image.hei; i++){
        for (int j = 0; j < image.wid; j++){
            colourSpace[image.matrix[i][j]] += 1;
        }
    }

    return colourSpace;
}


// colour frequency vector *IN PERCENTAGE*
vector<float> colourFrequencyPercentage(pgm image){

    //get the frequency vector
    vector<int> colourSpace = colourFrequency(image);
    vector<float> result;

    //calculate new vector
    int size = image.wid*image.hei;
    for(auto &colourN: colourSpace){
        result.push_back(round((colourN*100)/(float)size));
    }

    return result;
}

// Returns the average colour
int averageColour(pgm image){

    // get the frequency vector
    vector<int> colourSpace = colourFrequency(image);

    // sum over all colours with their respective number of pixels
    float sum = 0;
    for(int i = 0; i < colourSpace.size(); i++){
        sum += colourSpace[i]*i;
    }

    return round(sum/(float(image.wid*image.hei)));
}

// Intermidiary step to get standard deviation
float colourVariance(pgm image){

    // needed variables
    vector<int> colourSpace = colourFrequency(image);
    int mean = averageColour(image);
    float var = 0;

    // sum over all colours with their respective number of pixels
    for(int i = 0; i < colourSpace.size(); i++){
        int sub = i - mean; // spares 2 i-mean operations
        var += colourSpace[i]*sub*sub;
    }

    return round(var/float(image.wid*image.hei - 1));
}

// Returns the measure of standard deviation.
float standardDeviation(pgm image){
    return sqrt(colourVariance(image));
}

// returns the pair position-maximum_value of a float vector.
array<float,2> getMax(vector<float> vec){
    
    // needed variables
    float max = vec[0];
    float pos = 0; // We choose pos as a float to be returned in the array.

    // find max
    for (int i = 1; i < vec.size(); i++){
        if (vec[i] > max) {
            pos = i;
            max = vec[i];
        };
    }

    return {pos,max};
}


// returns the pair position-maximum_value of an int vector.
array<int,2> getMax(vector<int> vec){
    
    // needed variables
    int max = vec[0];
    int pos = 0; // We choose pos as a float to be returned in the array.

    // find max
    for (int i = 1; i < vec.size(); i++){
        if (vec[i] > max) {
            pos = i;
            max = vec[i];
        };
    }

    return {pos,max};
}

int median(vector<int> v){
    sort(v.begin(), v.end());
    if(v.size()%2==0){
        return round(((float)(v[v.size()/2]+v[v.size()/2-1]))/2.);
    }else{
        return v[v.size()/2];
    } 
}


///////////////////////////////////////////////////////////////////////// IMAGE PROCESSING

// inverted image - inverts the colours of the image in relation to the image maximum value
pgm invertImage(pgm inputMatrix){
    for(int i = 0; i<inputMatrix.hei; i++){
        for(int j = 0; j<inputMatrix.wid; j++){
            inputMatrix.matrix[i][j] = inputMatrix.maxValue - inputMatrix.matrix[i][j];
        }
    }
    return inputMatrix;
}


/*
filters an image taking into account neighboring pixels. WARNING - we are checking each step with 4 if statements which is inefficient
however we consider that for the task at hand, it is enough and spares some lines of code.
*/
pgm boxFilterImage(pgm input, vector<vector<float>> filter){

    // needed variables
    pgm output = input;
    int xStart, yStart, xMax, yMax;
    float sum, weightSum;

    /*
    Go through the pixels, taking into account the borders of the image. We ignore pixels that would have been "outside the border" - would be a segfault.
    For each pixel we change it *in the output* taking into account neighboring pixels according to the weight matrix (filter).
    There is also a correcting factor - weightSum - to correct for the sums of filters with pixels that don't add up to 1, and more importantly
    to correct for border and corner pixels, where the pixels would get progressively darker and darker with successive applications of
    boxFilters to the image. This factor prevents it (To test it out, try applying a boxFilter 100 times - the edges will remain crisp).
    */ 
    for(int i = 0; i < input.hei; i++){
        for(int j = 0; j < input.wid; j++){

            // Initialize variables and bind the pixel sweeper to prevent a segfault.
            weightSum = 0; sum = 0; xStart = -1; yStart = -1; xMax = 1; yMax = 1;
            if(i==0) xStart+=1;
            if(j==0) yStart+=1;
            if(j==input.wid-1) yMax-=1;
            if(i==input.hei-1) xMax-=1;

            // Correcting factor for borders, corners and matrices that don't add up to 1.
            for(int x = xStart; x <= xMax; x++){
                for(int y = yStart; y <= yMax; y++){
                    weightSum += filter[x+1][y+1];
                }
            }

            // Calculates the targeted pixel taking into account weightSum correcting factor
            for(int x = xStart; x <= xMax; x++){
                for(int y = yStart; y <= yMax; y++){
                    sum += filter[x+1][y+1]*input.matrix[i+x][j+y];
                }
            }
            output.matrix[i][j] = round(sum/weightSum);                   
        }
    }

    return output;
}



// Uses the box filter function with the parameters being all surrounding pixels, except for the center one.
pgm removeNoiseExclusiveImage(pgm inputMatrix){
    return boxFilterImage(inputMatrix,{{1,1,1},{1,0,1},{1,1,1}});
}


// Uses the box filter function with the parameters being all surrounding pixels, including the center one, with equal weights per pixel.
pgm removeNoiseInclusiveImage(pgm inputMatrix){
    return boxFilterImage(inputMatrix,{{1,1,1},{1,1,1},{1,1,1}});
}




///////////////////////////////////////////////////////////////////////// PART B FUNCTIONS


// This filter works for any 2*r side square if 2r+1<=image.wid. It sets a pixel[i][j] to the median of surrounding pixels in the square.
pgm squareMeanFilter(pgm input, int r){
    // needed variables
    pgm output = input;
    int xStart, yStart, xMax, yMax, wid=input.wid, hei=input.hei;

    // Go through the matrix
    for(int i = 0; i < hei; i++){
        for(int j = 0; j < wid; j++){

            // Initialize variables and bind the pixel sweeper to prevent a segfault.
            xStart = -r; yStart = -r; xMax = r; yMax = r;
            if(i<r) xStart+=r-i;
            if(j<r) yStart+=r-j;
            if((hei-i)<=r) xMax-=r-(hei-i)+1;
            if((wid-j)<=r) yMax-=r-(wid-j)+1;

            // Collect all surrounding pixels
            vector<int> collect;
            for(int x = xStart; x <= xMax; x++){
                for(int y = yStart; y <= yMax; y++){
                    collect.push_back(input.matrix[i+x][j+y]);
                }
            }
            
            // median calculation (takes into account for odd and even numbered vectors)
            output.matrix[i][j] = median(collect);
        }
    }

    return output;
}



// This filter works for any 2*r side square if 2r+1<=image.wid. It sets a pixel[i][j] to the median of surrounding pixels in a diamond inscribed in the square.
pgm diamondMeanFilter(pgm input, int r){
    
    // needed variables
    pgm output = input;
    int xStart, yStart, xMax, yMax, wid=input.wid, hei=input.hei;

    // go through the matrix
    for(int i = 0; i < hei; i++){
        for(int j = 0; j < wid; j++){

            // Initialize variables and bind the pixel sweeper to prevent a segfault.
            xStart = -r; yStart = -r; xMax = r; yMax = r;
            if(i<r) xStart+=r-i;
            if(j<r) yStart+=r-j;
            if((hei-i)<=r) xMax-=r-(hei-i)+1;
            if((wid-j)<=r) yMax-=r-(wid-j)+1;

            // Collect all surrounding pixels
            vector<int> collect;
            for(int x = xStart; x <= xMax; x++){
                for(int y = yStart; y <= yMax; y++){
                    // Check the pixels within the diamond - we use the equation of a diamond
                    if((abs(x)+abs(y))<=r){
                        collect.push_back(input.matrix[i+x][j+y]);
                    }
                }
            }

            // median calculation (takes into account for odd and even numbered vectors)
            output.matrix[i][j] = median(collect);
        }
    }

    return output;
}
