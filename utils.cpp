#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <functional>
#include <bits/stdc++.h>
#include "utils.h" 
using namespace std;

class DiscountCurve {
    // Access specifier
    public:
    // Data  Members
    vector<double> tenor;
    vector<double> value;
    
    DiscountCurve(vector<double> _tenor, vector<double> _value)
    {
        tenor = _tenor;
        value = _value;
    }
    // Member Functions()
    void print() {
        cout << "Discount Curve:" << endl;
        for(int i=0; i<tenor.size(); i++){
            cout << tenor[i] << ": " << value[i] << " "; 
        }
    }
    double at(double T){
        double P_T;
        P_T = linear_interpolation(tenor, value, T);
        return P_T;
    }
};

int main(){
    vector<double> zcb_tenor = {1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {0.97, 0.95, 0.91, 0.9, 0.88}; // T -> P_0(T)

    // Discount Curve
    DiscountCurve P(zcb_tenor, zcb_price);
    P.print();
    double t0 = 1.1;
    double d1 = P.at(t0);
    double d2 = P.at(3.1);
    double d3 = P.at(5);
    cout << endl << d1 << endl << d1 << endl << d3;
    return 0;
};