#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <functional>
#include "utils.h"

using namespace std;

double sigma_sq_mod(double sigma1, double sigma2, double lambda, double t, double S, double T)
{
    double v1 = - sigma1 * (T - S);
    double v2 = - sigma2 * exp (lambda * t) * (exp (-lambda * S) - exp (-lambda *T));
    return pow(v1, 2) + pow(v2, 2);
}

int main(){
    // Discount Curve
    vector<double> zcb_tenor = {0, 1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {1, 0.97, 0.95, 0.91, 0.9, 0.88}; // T -> P_0(T)
    double r = 0;
    double K = 98; // strike price
    double S = 0.5; // option maturity
    double T = 0.8; // option maturity
    
    double t = 0; // current time
    double sigma1 = 0.2; // first term of volatility
    double sigma2 = 0.1; // second term of volatility
    double lambda = 1; // exponential decay parameter
    double n = 1000;
    bool call = true; // option type
    double res;
    double resl;
    double x0 = 0.5;

    auto int_sigma = [&](double s) -> double { 
        return sigma_sq_mod(sigma1, sigma2, lambda, s, S, T);} ;
//    resl = linear_interpolation(zcb_tenor, zcb_price, x0) ;
    res = integral(int_sigma, t, S, n);
    cout << res << endl;
//    cout << resl << endl;
    
    return 0;
}

