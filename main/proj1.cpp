/*
Project developed by João Camacho and Duarte Tavares, IST 2023

This programme aims to remove noise from a noisy image.
*/

// Includes may vary between operating systems and IDE's / text editors.
#include <iostream>
#include "rImagem.h"

// we will not be using any other namespaces.
using namespace std;

// For aesthetic purposes :)
void separate(){
    std::cout << "------------------------\n";
}

// Main program
int main(){

    // load up the image
    printf("Loading Image into pgm struct.\n");
    pgm image = loadFile("proj1/imagem.pgm");

    // exercises 2,3
    separate();
    auto colours = colourFrequency(image);
    auto coloursPercent = colourFrequencyPercentage(image);

    for(int i = 1; i <= 3; i++){
        auto max = getMax(colours);
        std::cout << "Max colour #" << i << " is the colour " << max[0] << " with " << max[1] << " pixels\n";
        for(int i = 0; i < colours.size(); i++) if(i==max[0]) colours[i] = -1;
    }
    for(int i = 1; i <= 3; i++){
        auto max = getMax(coloursPercent);
        std::cout << "Max colour #" << i << " is the colour " << max[0] << " with " << max[1] << " %\n";
        for(int i = 0; i < coloursPercent.size(); i++) if(i==max[0]) coloursPercent[i] = -1;
    }
    
    cout << "Image average colour - " << averageColour(image) << "\n";
    cout << "Image standard deviation - " << standardDeviation(image) << "\n";


    // exercise 4
    separate();
    pgm imageSum = invertImage(image);
    writeFile(imageSum,"proj1/peixe_noise10_inverted.ascii.pgm");
    for(int i = 0; i < imageSum.hei; i++){
        for(int j = 0; j < imageSum.wid; j++){
            imageSum.matrix[i][j] += image.matrix[i][j];
        }
    }
    cout << "Image+Invert Sum - " << averageColour(imageSum) << "\n";
    writeFile(imageSum,"proj1/peixe_image_inverted_sum.pgm");


    // exercice 6
    separate();
    pgm reducedNoise = removeNoiseExclusiveImage(image);
    writeFile(reducedNoise,"proj1/peixe_reduced_noise.ascii.pgm");
    cout << "Reduced Noise Avg colour - " << averageColour(reducedNoise) << "\n";
    cout << "Reduced Noise standard deviation - " << standardDeviation(reducedNoise) << "\n";


    // exercice 7
    separate();
    pgm boxFilter9 = removeNoiseInclusiveImage(image);
    writeFile(boxFilter9,"proj1/peixe_box_blur.ascii.pgm");
    cout << "Box filter Avg colour - " << averageColour(boxFilter9) << "\n";
    cout << "Box filter standard deviation - " << standardDeviation(boxFilter9) << "\n";


    // PART B
    separate();
    pgm meanSquareFilter = squareMeanFilter(image,2);
    writeFile(meanSquareFilter,"proj1/peixe_filtro_mediana_quadrado.ascii.pgm");
    cout << "Mediana square filter average colour - " << averageColour(meanSquareFilter) << "\n";
    cout << "Mediana square filter standard deviation - " << standardDeviation(meanSquareFilter) << "\n";


    // PART B
    separate();
    pgm meanDiamondFilter = diamondMeanFilter(image,2);
    writeFile(meanDiamondFilter,"proj1/peixe_filtro_mediana_diamante.ascii.pgm");
    cout << "Mediana diamond filter average colour - " << averageColour(meanDiamondFilter) << "\n";
    cout << "Mediana diamond filter standard deviation - " << standardDeviation(meanDiamondFilter) << "\n";
    separate();


}
