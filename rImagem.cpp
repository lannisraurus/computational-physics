#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


//Data structure for the pgm matrix and max colour value
struct pgm{
    std::vector<std::vector<int>> matrix;
    int maxValue;
};



// this function returns a matrix containing PGM information. // is this right?
pgm loadFile(std::string fileName){ 

    // Needed variables
    std::ifstream file(fileName);
    std::string line;
    pgm resultMatrix; // {{line1},{line2},{line3},...}
    std::stringstream stringStream;
    int wid,hei; // width and height of the image

    // Extract first line of the file - useless (it's just the image format)
    std::getline(file, line);

    // Extract second line of the file - width and height
    std::getline(file, line);
    stringStream << line;
    stringStream >> wid >> hei;
    stringStream.clear();

    // Extract third line of the file - max value
    std::getline(file, line);
    stringStream << line;
    stringStream >> resultMatrix.maxValue;
    stringStream.clear();

    // Extract matrix information
    std::vector<int> emptyRow = {};
    for(int i = 0; i < hei; i++){
        resultMatrix.matrix.push_back(emptyRow);
    }
    int row = 0, collumn = 0, pixel;
    while (row < hei){
        // Extract pixel value
        file >> pixel;
        resultMatrix.matrix[row].push_back(pixel);
        //Condition - go through all wid*hei combinations
        if (collumn == wid-1) {
            row++;
            collumn = 0;
        }else {collumn++;}
    }
    

    // Close file and return the newfound matrix
    file.close();
    return resultMatrix;
}


// this function creates a new PGM file with the information given (name and matrix).
void writeFile(pgm inputMatrix, std::string fileName){ 
    //needed variables
    std::ofstream file(fileName);
    //write file
    file << "P2\n";
    file << inputMatrix.matrix[0].size() << " " << inputMatrix.matrix.size() << "\n";
    file << inputMatrix.maxValue << "\n";
    for(int i = 0; i < inputMatrix.matrix.size(); i++){
        for(int j = 0; j < inputMatrix.matrix[0].size(); j++){
            file << inputMatrix.matrix[i][j] << " ";
        }
    }
}


// colour frequency vector
std::vector<int> colourFrequency(pgm image){
    //fill colour space vector
    std::vector<int> colourSpace;
    for (int i = 0; i <= image.maxValue; i++) colourSpace.push_back(0);
    // analyse image
    for (int i = 0; i < image.matrix.size(); i++){
        for (int j = 0; j < image.matrix[0].size(); j++){
            colourSpace[image.matrix[i][j]] += 1;
        }
    }
    return colourSpace;
}


// colour frequency vector IN PERCENTAGE
std::vector<int> colourFrequencyPercentage(pgm image){
    //get the frequency vector
    std::vector<int> colourSpace = colourFrequency(image);
    //calculate new vector
    int size = image.matrix.size()*image.matrix[0].size();
    for(int i = 0; i < colourSpace.size(); i++){
        colourSpace[i] *= 100/(float)size;
        std::cout << colourSpace[i] << " ";

    }

    return colourSpace;
}


// returns the maximum value of a int vector
int getMax(std::vector<int> vec){
    int max = vec[0];
    for (int i = 1; i < vec.size(); i++){
        if (vec[i] > max) max = vec[i];
    }
    return max;
}



int main(){

    pgm imagem = loadFile("input.pgm");
    std::vector<int> colours = colourFrequencyPercentage(imagem);
    std::cout << getMax(colours) << std::endl;
    writeFile(imagem,"output.pgm");

}