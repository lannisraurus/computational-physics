// Necessary includes.
#include <array>
#include <utility>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>



// We will not be using conflitcting namespaces.
using namespace std;



// vector functions - we could have used a class, but we considered that for the task at hand there is no need (there aren't many calculations that need it)
array<float,3> VecDif(array<float,3> Point1, array<float,3> Point2);
array<float,3> VecMult(array<float,3> Point1, float a);
float dotProd(array<float,3> Point1, array<float,3> Point2);
float dist(array<float,3> Point1, array<float,3> Point2);



// Holds information about a cell that receives radiation in the form of light.
struct cell {
    array<float,3> pos;          // Cell center (cm,cm,cm)
    array<float,3> normalVector; // Vector normal to the cell (defines orientation)
    float area;                  // Size of the cell (cm^2)
    float power;                 // Power inciding in the cell (W)
};



// Holds information about a single-point light-source.
struct lightsource{
    array<float,3> pos; // Light-source center (cm,cm,cm)
    float power;        // Power radiating from the light-source (W)
};



// Used for calculating the interactions between the light sources and cells.
class lightmap{

private:

    // Variables
    float xCell, yCell;               // holds the size of a single cell.
    int cellGridX, cellGridY;         // holds the dimensions of the cell grid - number of grids.
    float cellGridWid, cellGridHei;   // holds the dimensions for the cell grid - cm.
    vector<vector<cell>> cellGrid;    // holds all the cells that form the object.
    vector<lightsource> lightSources; // holds all the light-sources that radiate upon the object.
    array<float,3> normal;            // normal vector to plane

    // private utility functions
    void initGrid(array<int,2> nCellsXY, array<float,2> size); // Initializes the grid in the constructors

public:
    
    // Constructors
    lightmap() = default;                                                     // Default constructor
    lightmap(array<int,2> nCellsXY, array<float,2> size);                     // nCellsXY -> number of horizontal.X.vertical cells. size -> total size of the panel
    lightmap(lightsource source, array<int,2> nCellsXY, array<float,2> size); // Same as before, but automatically adds a light source.
    
    // Getters
    pair<int,int> GetCellIndex(float, float) const;         // Given x,y coordinates, returns cell index in the matrix.
    pair<float,float> GetCellCoo(int, int) const;           // Given entry (i,j) in the matrix, returns the cell coordinates
    double GetCellPower(int, int) const;                    // Given entry (i,j), returns cell incident power.
    double GetCellPower(float, float) const;                // Given coordinates x,y , returns cell incident power.
    int GetCellNx() const;                                  // Returns number of cells along x
    int GetCellNy() const;                                  // Returns number of cells along y
    const vector<vector<cell>>& GetCells() const;           // Returns a constant reference to the grid
    vector<vector<cell>>& GetCells();                       // Returns the cell grid BY REFERENCE
    array<float,3> GetNormal();                             // Returns the vector perpendicular to the grid
    vector<cell> CellsSorted() const;                       // Sorts cells by ascending absorbed power.
    vector<cell> CellsSortedDesc() const;                   // Sorts cells by descending absorbed power.
    

    // Adders
    void AddLightSource(lightsource S);
    
    // Numeric tools
    float Irradiance(array<float,3>);   // Calculates irradiance in a point.
    float CellPower(const cell&);       // Updates a cell's power according to the light source(s)

    // Self-implemented tools
    float CellMedian() const;                               // Return the median value of power in the cells
    float CellAverage() const;                              // Return the average value of power in the cells   
    float TotalPower() const;                               // Return the total power in the sheet.
    const cell& GetMaxCell() const;                         // Return cell with maximum power.   
    const cell& GetMinCell() const;                         // Return cell with minimum power.   
    float GetWid() const;                                   // Returns sheet's width
    float GetHei() const;                                   // Returns sheet's height

    vector<lightsource> GetLightsource() const;             // Returns lightsources vector

    pair<float,float> TrapezoidalRule();  

};