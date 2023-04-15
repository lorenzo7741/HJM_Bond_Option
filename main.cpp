#include <iostream>
#include <cmath>
#include <vector>
#include "utils.h" 
using namespace std;

class DiscountCurve {
    /*
    The Discount Curve represents the relationship between the present value of a future cash flow and its maturity. The Discount Curve is represented as a vector of tenors and their corresponding values.
    Attributes:
    - tenor (<vector> double): a vector of doubles representing tenors
    - value (<vector> double): a vector of doubles representing the values of the tenors
    Methods:
    - at: evaluate the discount curve at a specific tenor with a linear interpoation
    */
    public:
    vector<double> tenor;
    vector<double> value;

    
    DiscountCurve(vector<double> _tenor, vector<double> _value)
    {
        tenor = _tenor;
        value = _value;
    }
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

class HjmModel{
    /* 
    A class to collect the functions and the parameters of an HJM model with a two-factor volatility.
    - Attributes:
        * sigma1 (double): instantaneous volatility
        * sigma2 (double): long-run volatility
        * lambda (double): mean reversion parameter of sigma2
    - Methods:
        * sigma_sq_mod: function that evaluates the instantaneous volatility depending on both sigma1, sigma2 and lambda

    Example:
    ```cpp
    int main(){
        // HJM parameters
        double sigma1 = 0.3;
        double sigma2 = 0.2;
        double lambda = 2;

        // Discount Curve Initialization
        HjmModel hjm(sigma1, sigma2, lambda);
        hjm.print();
        return 0;
    }
    ```
    */
    public:
    // Data  Members
    double sigma1;
    double sigma2;
    double lambda;
    HjmModel(double _sigma1, double _sigma2, double _lambda) {
        sigma1 = _sigma1;
        sigma2 = _sigma2;
        lambda = _lambda;
    }

    double sigma_sq_mod(double t, double S, double T){
        double v1 = - sigma1 * (T - S);
        double v2 = - sigma2 * exp (lambda * t) * (exp (-lambda * S) - exp (-lambda * T)) / lambda;
        return pow(v1, 2) + pow(v2, 2);
    }

    void print() {
        cout << "HJM Parameters:" << endl << 
        "Sigma 1: " << sigma1 << endl <<  
        "Sigma 2: " << sigma2 << endl <<
        "Lambda: " << lambda << endl;
    }

};

class OptionOnZcb{
    /*An OptionOnZcb represents an option on a Zero-Coupon Bond.
    - Attributes
        * strike (double): the strike price of the option
        * option_ttm (double): the time to maturity of the option  
        * bond_ttm (double): the time to maturity of the underlying bond
        * iscall (bool): a boolean representing whether the option is a call option (true) or put option (false)
    - Methods:
        * price: a method that evaluates the price of the option in a HJM framework
    */

    public:
    double strike;
    double option_ttm;
    double bond_ttm;
    bool iscall;

    OptionOnZcb(double _strike, double _option_ttm, double _bond_ttm, bool _iscall){
        strike = _strike;
        option_ttm = _option_ttm; // Option Time to maturity
        bond_ttm = _bond_ttm; // Bond Time to maturity
        iscall = _iscall;
    }

    void print() {
        cout << "Options on Bond Parameters:" << endl << 
        "Strike " << strike << endl <<  
        "Option Maturity: " << option_ttm << endl <<
        "Bond Maturity: " << bond_ttm << endl <<
        "Is Call: " << iscall << endl;
    }

    double price(DiscountCurve P, HjmModel hjm) {
        double opt_price, d1, d2, res;
        double s1 = hjm.sigma1;
        double s2 = hjm.sigma2;
        double lam = hjm.lambda;
        double S = option_ttm; // Option Time to maturity
        double T = bond_ttm; // Bond Time to maturity
        double n = 1000;

        auto int_sigma = [&](double u) -> double {return hjm.sigma_sq_mod(u, S, T); };
        res = integral(int_sigma, 0, S, n);
        d1 = (log(P.at(T) / (P.at(S) * strike)) + 0.5 * res) / sqrt(res);
        d2 = (log(P.at(T) / (P.at(S) * strike)) - 0.5 * res) / sqrt(res);;
        opt_price = P.at(T) * cdf(d1) - strike * P.at(S) * cdf(d2);

        return opt_price;
    }
};

int main(){
    // ZCB prices obtained from the market
    vector<double> zcb_tenor = {0, 1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {1, 0.97, 0.94, 0.91, 0.9, 0.88}; // T -> P_0(T)
    
    // HJM parameters
    double sigma1 = 0.3;
    double sigma2 = 0.2;
    double lambda = 2;

    // Options parameters
    double strike = 0.5;
    double option_maturity = 1.5; /* 2 years */
    double bond_maturity = 4.5; /* 2 years and half */
    double iscall = true;
    double opt_price;

    // Discount Curve Initialization
    DiscountCurve P(zcb_tenor, zcb_price);

    // HJM Model Initialization
    HjmModel hjm(sigma1, sigma2, lambda);

    // Option Initialization
    OptionOnZcb opt(strike, option_maturity, bond_maturity, iscall);

    P.print();
    hjm.print();
    opt.print();
    opt_price = opt.price(P, hjm);
    cout << "Price: " << opt_price;
    return 0;
};