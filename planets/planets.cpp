#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <algorithm>


#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLatex.h"




#define     PI                  M_PI            // unitless
#define     G                   6.6743E-11      // SI units
#define     AU                  1.50E11         // m
#define     SOLAR_MASS          1.989E30        // kg
#define     SUN_TEMPERATURE     5772            // K
#define     SUN_RADIUS          6.9634E8        // m
#define     EARTH_YEAR          3.1536E7        // s
#define     EARTH_MASS          5.97219E24      // kg
#define     EARTH_RADIUS        6.371E6         // m
#define     EARTH_DAY           86400           // s




using namespace std;



class vec2{
private:
    double x,y;
public:
    vec2(): x(0), y(0) {}
    vec2(double X,double Y): x(X), y(Y) {}
    double operator* (const vec2& v) { return x*v.x + y*v.y; }
    vec2 operator+ (const vec2& v) { return vec2(x+v.x,y+v.y); }
    vec2 operator- ( const vec2& v) { return vec2(x-v.x,y-v.y); }
    friend vec2 operator* (const double& k, const vec2& v); 
    double size() { return sqrt((*this)*(*this)); }
    double X() { return x; }
    double Y() { return y; }
    vec2 normalized() { double k = size(); return (1/k)*(*this) ; }
};
vec2 operator* (const double& k, const vec2& v){ return vec2(k*v.x,k*v.y); }



class body{
private:
    // planet info
    string name;
    double mass, radius;
    vec2 position, velocity, acceleration;
    // rotation
    double angularVelocity, angle;
public:
    // planet info
    body() = default;
    body(double M, double R, vec2 pos, vec2 vel, vec2 acc, string n)
    : mass(M), radius(R), position(pos), velocity(vel), acceleration(acc), name(n), angularVelocity(0), angle(0)
    {}
    double getMass() { return mass; }
    double getRadius() { return radius; }
    vec2 getPosition () { return position; }
    vec2 getVelocity () { return velocity; }
    vec2 getAcceleration () { return acceleration; }
    string getName () { return name; }
    // rotation
    void setDay(double day){ angularVelocity = (2.*PI)/day; }
    void setAngularVelocity(double w) { angularVelocity = w; }
    void setAngle(double a) { angle = a; }
    double getAngle () { return angle; }
    double getAngularVelocity() { return angularVelocity; }
    void updateAngle(double dT){ angle += dT*angularVelocity; while (angle>2*PI) angle-=2*PI; while (angle < 0) angle+= 2*PI; }
    vec2 getRotationPoint () { return position + radius*vec2(sin(angle),cos(angle));}
    double getAlignmentAngle() { return atan2(position.Y(),position.X()); }
};



class sys{
private:
    double starTemperature;
    vector<body> bodies;
    vector<body> originalBodies;
    vector<double> times;
    vector<vector<double>> temperature;
    vector<vector<double>> distToPrev;
    vector<vector<double>> distToStar;
    vector<vector<double>> orbitalSpeed;
    vector<vector<double>> dayNight;
    vector<vector<double>> angles;
    vector<vector<double>> alignment;
    vector<vector<double>> orbitalAccel;

public:
    sys(double mass, double radius, string name, double temperature) :starTemperature(temperature) { bodies.push_back(body(mass,radius,vec2(),vec2(),vec2(),name)); };
    ~sys() = default;
    void linkBody(double mass, double radius, double distance, double angle, double day, string name){
        double speed = sqrt( (G*bodies.back().getMass()) / distance );
        vec2 pos = bodies.back().getPosition() + distance*vec2(cos(angle),sin(angle));
        vec2 vel = bodies.back().getVelocity() + speed*vec2(-sin(angle),cos(angle));
        bodies.push_back( body(mass, radius, pos, vel, vec2(), name) );
        bodies.back().setDay(day);
        originalBodies = bodies;
    }
    void solve(double T, double dT){
        // clean-up
        bodies = originalBodies;
        times.clear();
        temperature.clear();
        distToPrev.clear();
        distToStar.clear();
        orbitalSpeed.clear();
        dayNight.clear();
        angles.clear();
        alignment.clear();
        orbitalAccel.clear();
        // SOLVING
        cout << "|| Solving system ...\n";
        // Allocate Space
        cout << "Allocating space\n";
        temperature.resize(bodies.size());
        distToPrev.resize(bodies.size());
        distToStar.resize(bodies.size());
        dayNight.resize(bodies.size());
        orbitalSpeed.resize(bodies.size());
        orbitalAccel.resize(bodies.size());
        angles.resize(bodies.size());
        alignment.resize(bodies.size());
        cout << "Calculating trajectories\n";
        for(double t = 0; t < T; t += dT){
            // Euler
            vector<body> newBodies (bodies.size());
            vector<vec2> accels (bodies.size() , vec2());
            vec2 vel, pos;
            for(int i = 0; i < bodies.size(); i++){
                for(int j = i+1; j < bodies.size(); j++){
                    vec2 dist = bodies[j].getPosition() - bodies[i].getPosition();
                    vec2 r = dist.normalized();
                    double d = pow(dist.size(),2);
                    accels[i] = accels[i] + (bodies[j].getMass() / d)*r;
                    accels[j] = accels[j] - (bodies[i].getMass() / d)*r;
                }
                accels[i] = G*accels[i];
                vel = bodies[i].getVelocity() + dT*accels[i];
                pos = bodies[i].getPosition() + dT*vel;
                newBodies[i] = body(bodies[i].getMass(),bodies[i].getRadius(),pos,vel,accels[i],bodies[i].getName());
                newBodies[i].setAngularVelocity(bodies[i].getAngularVelocity());
                newBodies[i].setAngle(bodies[i].getAngle());
                newBodies[i].updateAngle(dT);
            }
            // Trajectory Update
            bodies = newBodies;
            // Data Extraction
            times.push_back(t);
            for(int i = 1; i < bodies.size(); i++){
                distToStar[i].push_back((bodies[i].getPosition() - bodies[0].getPosition()).size());
                distToPrev[i].push_back((bodies[i].getPosition() - bodies[i-1].getPosition()).size());
                temperature[i].push_back( starTemperature*pow(0.25,0.25)*pow(bodies[0].getRadius()/distToStar[i].back(),0.5) );
                orbitalSpeed[i].push_back(bodies[i].getVelocity().size());
                dayNight[i].push_back( (bodies[i].getRotationPoint() - bodies[0].getPosition()).size() - (bodies[i].getPosition() - bodies[0].getPosition()).size() );
                angles[i].push_back(abs(bodies[i].getAlignmentAngle()));
                double align = abs(bodies[i].getAlignmentAngle()-bodies[i-1].getAlignmentAngle()); if (align>0.01) align=0.01; alignment[i].push_back(align);
            }
        }
        // Data analysis from data extraction
        cout << "Extracting extra data\n";
        for(int i = 1; i < bodies.size(); i++){
            orbitalAccel[i].push_back(0);
            for (int j = 1; j < orbitalSpeed[i].size(); j++){
                orbitalAccel[i].push_back( (orbitalSpeed[i][j]-orbitalSpeed[i][j-1]) / dT );
            }
        }
        // DONE
        cout << "Done!\n";
    }

    void saveGraphs(string append="", string time_units="s", string distance_units="m", double time_convert=1., double distance_convert=1.){
        // SAVING
        cout << "|| Saving graph data ...\n";
        // Initialize canvas and grid style
        TCanvas canvas ("","",1000,2000);
        gStyle->SetGridStyle(0);
        gStyle->SetGridColor(17); 
        // Converted time vector
        vector<double> T, D;
        for (const auto& i: times) T.push_back(i/time_convert);
        // Create directories and store data
        for(int i = 1; i < bodies.size(); i++){

            cout << "Saving " << bodies[i].getName() << "\n";
            
            filesystem::create_directory(bodies[i].getName());

            canvas.Divide(1,8);

            TLatex text(.01,.99,bodies[i].getName().c_str());
            text.SetNDC();
            text.SetTextSize(0.02);
            text.Draw();

            canvas.cd(1);
            for (const auto& j: distToStar[i]) D.push_back(j/distance_convert);
            TGraph DS(T.size(),T.data(),D.data());
            D.clear();
            DS.SetLineColor(93);
            DS.SetTitle(("Distance to "+bodies[0].getName()+";time ["+time_units+"];distance ["+distance_units+"]").c_str());
            DS.Draw("AL"); gPad->SetGrid();

            canvas.cd(2);
            for (const auto& j: distToPrev[i]) D.push_back(j/distance_convert);
            TGraph DP(T.size(),T.data(),D.data());
            D.clear();
            DP.SetLineColor(53);
            DP.SetTitle(("Distance to "+bodies[i-1].getName()+";time ["+time_units+"];distance ["+distance_units+"]").c_str());
            DP.Draw("AL"); gPad->SetGrid();

            canvas.cd(3);
            TGraph ET(T.size(),T.data(),temperature[i].data());
            ET.SetLineColor(98);
            ET.SetTitle(("Effective Temperature;time ["+time_units+"];Temperature [K]").c_str());
            ET.Draw("AL"); gPad->SetGrid();

            canvas.cd(4);
            TGraph V(T.size(),T.data(),orbitalSpeed[i].data());
            V.SetLineColor(66);
            V.SetTitle(("Orbital Speed;time ["+time_units+"];velocity [ms^-1]").c_str());
            V.Draw("AL"); gPad->SetGrid();

            canvas.cd(5);
            TGraph A(T.size(),T.data(),orbitalAccel[i].data());
            A.SetLineColor(59);
            A.SetTitle(("Orbital Acceleration;time ["+time_units+"];acceleration [ms^-2]").c_str());
            A.Draw("AL"); gPad->SetGrid();

            canvas.cd(6);
            for (const auto& j: dayNight[i]) D.push_back(j/distance_convert);
            TGraph DN(T.size(),T.data(),D.data());
            D.clear();
            DN.SetLineColor(44);
            DN.SetTitle(("Day Night Cycle [day: >0, night <0];time["+time_units+"];Dist(C.M.,Star)-Dist(Point,Star) ["+distance_units+"]").c_str());
            DN.Draw("AL"); gPad->SetGrid();

            canvas.cd(7);
            TGraph AL(T.size(),T.data(),alignment[i].data());
            AL.SetLineColor(31);
            AL.SetTitle(("Alignment with "+bodies[i-1].getName()+" [0 = eclipse, when applicable];time ["+time_units+"];Abs Angle difference between planets cutoff at 0.01rad [rad]").c_str());
            AL.Draw("AL"); gPad->SetGrid();

            canvas.cd(8);
            TGraph YE(T.size(),T.data(),angles[i].data());
            YE.SetLineColor(50);
            YE.SetTitle(("Alignment with "+bodies[0].getName()+" [0 = Solar Year];time ["+time_units+"];Angle with the Ox axis [rad]").c_str());
            YE.Draw("AL"); gPad->SetGrid();


            canvas.Modified();
            canvas.Update();
            canvas.SaveAs((bodies[i].getName()+"/"+bodies[i].getName()+append+".pdf").c_str(),"pdf");
            canvas.Clear();

        }
        // DONE
        cout << "Done!\n";
    }
    void saveData() {
        // SAVING
        cout << "|| Saving statistical data ...\n";
        ofstream save;
        for(int i = 1; i < bodies.size(); i++){
            cout << "Analysing " << bodies[i].getName() << "\n";
            save.open(bodies[i].getName()+"/data.txt");

            // Average Effective Temperature
            double temp = 0;
            for (const auto& a: temperature[i]) temp += a;
            temp /= temperature[i].size();
            save << "Average Effective Temperature: " << temp << " [K]\n";

            // Average Orbital Speed
            double speed = 0;
            for (const auto& a: orbitalSpeed[i]) speed += a;
            speed /= orbitalSpeed[i].size();
            save << "Average Orbital Speed: " << speed << " [ms^-1]\n";

            // Year Time (in relation to previous body)
            for(int i = 1; i < distToPrev)

            // Year Time (in relation to star)
            
            // Day Time

            // Eclipse Frequency

            // Eclipse Duration


            save.close();
        }
        // DONE
        cout << "Done!\n";
    }

};



int main(){
    // SETUP
    sys system(0.75*SOLAR_MASS, 0.9*SUN_RADIUS, "Sol-K", 4900 );
    system.linkBody(14.*EARTH_MASS, 45.*EARTH_RADIUS, 0.8*AU, 0, 0.1*EARTH_DAY, "Grandiose");
    system.linkBody(0.9*EARTH_MASS, 0.85*EARTH_RADIUS, 0.01*AU, 0, 0.7*EARTH_DAY, "Lua");
    // Graphs
    system.solve(EARTH_DAY, 1);
    system.saveGraphs("_Earth_Day","s","m",1.,1.);
    system.solve(EARTH_DAY*7., 10);
    system.saveGraphs("_Earth_Week","days","m",EARTH_DAY,1.);
    system.solve(EARTH_DAY*30., 3600);
    system.saveGraphs("_Earth_Month","days","m",EARTH_DAY,1.);
    system.solve(EARTH_DAY*30.*6., 3600);
    system.saveGraphs("_Earth_6_Months","days","m",EARTH_DAY,1.);
    system.solve(EARTH_YEAR, 3600);
    system.saveGraphs("_Earth_Year","days","m",EARTH_DAY,1.);
    system.solve(EARTH_YEAR*10., 3600);
    system.saveGraphs("_Earth_10_Years","days","m",EARTH_DAY,1.);
    system.solve(EARTH_YEAR*100., EARTH_DAY);
    system.saveGraphs("_Earth_100_Years","days","m",EARTH_DAY,1.);
    system.solve(EARTH_YEAR*1000., EARTH_DAY);
    system.saveGraphs("_Stability","years","m",EARTH_YEAR,1.);
    // General Data
    system.solve(5.*EARTH_YEAR, 60);
    system.saveData();
}
