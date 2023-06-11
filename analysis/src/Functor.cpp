#include "Functor.h"




TCanvas* Functor::c = nullptr;

Functor::Functor(string name_) : name(name_) {
    if (c==nullptr) c = new TCanvas("c","Canvas Functor", 1000, 1000);
}

double Functor::operator() (double& x) { return 0; }
void Functor::Draw(array<float,2> range, int npoints, array<string,2> titles) {

    // make graph object
    vector<double> x(npoints), y(npoints);
    double h = (range[1]-range[0])/(npoints-1);
    for (int i=0; i<npoints; ++i) {
    x[i] = range[0] + i*h;
    y[i] = this->operator()(x[i]);
    }
    TGraph *g = new TGraph(x.size(),x.data(), y.data());
    g->GetXaxis()->SetTitle(titles[0].data());
    g->GetYaxis()->SetTitle(titles[1].data());
    g->SetLineColor(kBlue-2);
    g->SetLineWidth(4);
    g->SetTitle(name.c_str());
    // save
    TCanvas c(("c_"+name).c_str(), "", 1000, 1000);
    g->Draw("AL");
    c.Update();
    c.SaveAs("graph.png","png");
}

