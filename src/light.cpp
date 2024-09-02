#include "light.h"


// vector methods

array<float,3> VecDif(array<float,3> Point1, array<float,3> Point2) {
    return {Point1[0]-Point2[0],Point1[1]-Point2[1],Point1[2]-Point2[2]};
}

array<float,3> VecMult(array<float,3> Point1, float a) {
    return {Point1[0]*a,Point1[1]*a,Point1[2]*a};
}

float dotProd(array<float,3> Point1, array<float,3> Point2) {
    return Point1[0]*Point2[0]+Point1[1]*Point2[1]+Point1[2]*Point2[2];
}

float dist(array<float,3> Point1, array<float,3> Point2) {
    array<float,3> sub = VecDif(Point1,Point2);
    return sqrt(dotProd(sub,sub));
}


// lightmap methods

void lightmap::initGrid(array<int,2> nCellsXY, array<float,2> size){
    
    
    // Store grid dimensions
    cellGridX = nCellsXY[0];
    cellGridY = nCellsXY[1];
    cellGridWid = size[0];
    cellGridHei = size[1];

    // Store normal vector
    normal = {0.,0.,1.};

    // Define cell dimensions
    xCell = size[0]/float(nCellsXY[0]);
    yCell = size[1]/float(nCellsXY[1]);
    float area = xCell*yCell;
    float cellRadiusX = xCell/2.f, cellRadiusY = yCell/2.f;

    // Fill the cell grid
    for(int i = 0; i < nCellsXY[1]; i++){

        cellGrid.push_back(vector<cell>());
        float yPos = cellRadiusY + i*yCell;

        for(int j = 0 ; j < nCellsXY[0]; j++){

            cell c;
            c.area = area;
            c.pos = { cellRadiusX + j*xCell, yPos, 0};
            c.normalVector = normal;
            cellGrid[i].push_back(c);
            
        }
    }
}

lightmap::lightmap(array<int,2> nCellsXY, array<float,2> size){
    initGrid(nCellsXY,size);
}

lightmap::lightmap(lightsource source, array<int,2> nCellsXY, array<float,2> size){
    initGrid(nCellsXY,size);
    AddLightSource(source);
}

pair<int,int> lightmap::GetCellIndex(float x, float y) const {
    return {floor(x/xCell),floor(y/yCell)};
}

pair<float,float> lightmap::GetCellCoo(int x, int y) const {
    return {cellGrid[y][x].pos[0],cellGrid[y][x].pos[1]};
}

double lightmap::GetCellPower(int x, int y) const {
    return cellGrid[y][x].power;
}

double lightmap::GetCellPower(float x, float y) const {
    return cellGrid[GetCellIndex(x,y).second][GetCellIndex(x,y).first].power;
}

int lightmap::GetCellNx() const {
    return cellGridX;
}

int lightmap::GetCellNy() const {
    return cellGridY;
}

const vector<vector<cell>>& lightmap::GetCells() const {
    return cellGrid;
}

vector<vector<cell>>& lightmap::GetCells() {
    return cellGrid;
}

array<float,3> lightmap::GetNormal() {
    return normal;
}

void lightmap::AddLightSource(lightsource S) {
    lightSources.push_back(S);
    for(int i = 0; i < cellGridY; i++){
        for(int j = 0 ; j < cellGridX; j++){
            cellGrid[i][j].power = CellPower(cellGrid[i][j]);           
        }
    }
}

float lightmap::Irradiance(array<float,3> Point) {
    float irradiance = 0.f;
    for(auto &s: lightSources){
        float distance = dist(Point,s.pos);
        array<float,3> sourceToPointNorm = VecMult(VecDif(Point,s.pos),1.f/distance);
        irradiance += (s.power/(4.f*M_PI)) * (dotProd(sourceToPointNorm,normal)/(distance*distance));
    }
    return irradiance;
}

float lightmap::CellPower(const cell& C) {
    return Irradiance(C.pos)*C.area;
}

vector<cell> lightmap::CellsSorted() const {
    vector<cell> cells;
    for(auto &row: cellGrid) for(auto &elem: row) cells.push_back(elem);
    sort(cells.begin(), cells.end(), [](const cell& a, const cell& b)->bool{ return a.power < b.power; } );
    return cells;
}

const cell& lightmap::GetMaxCell() const {
    return CellsSorted()[cellGridX*cellGridY-1];
} 

const cell& lightmap::GetMinCell() const {
    return CellsSorted()[0];
}  

float lightmap::CellMedian() const {
    vector<cell> v = CellsSorted();
    if(v.size()%2==0){
        return ((float)(v[v.size()/2].power+v[v.size()/2-1].power))/2.;
    }else{
        return v[v.size()/2].power;
    } 
}

float lightmap::TotalPower() const {
    float sum = 0;
    vector<cell> v = CellsSorted();
    for(auto &a: v) sum+=a.power;
    return sum;
}

float lightmap::CellAverage() const {
    return TotalPower()/(cellGridX*cellGridY);
}

float lightmap::GetWid() const{
    return cellGridWid;
}

float lightmap::GetHei() const{
    return cellGridHei;
}