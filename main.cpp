#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "utils.h"

using namespace std;

// Heat-Jarrow-Morton model for the instantaneous forward rate
double h(double t, double T, double r, double sigma1, double sigma2, double lambda) {
    return exp(-lambda*(T-t))*r + (1-exp(-lambda*(T-t)))*(sigma1 + sigma2*(1-exp(-lambda*(T-t))))*sqrt((1-exp(-2*lambda*(T-t)))/(2*lambda));
}

double sigma_sq_mod_old(double sigma1, double sigma2, double lambda, double t, double T)
{
    double exp_factor = exp(-lambda * (T - t));
    double volatility = sqrt(
        pow(sigma1, 2) +
        pow(sigma2 * exp_factor, 2)
    );
    return volatility;
}

double sigma_sq_mod(double sigma1, double sigma2, double lambda, double t, double S, double T)
{
    double v1 = - sigma1 * (T - S);
    double v2 = - sigma2 * exp (lambda * t) * (exp (-lambda * S) - exp (-lambda *T));
    return pow(v1, 2) + pow(v2, 2);
}


// Cumulative normal distribution function
double cdf(double x) {
    return 0.5*(1+erf(x/sqrt(2.0)));
}

// Gaussian option pricing formula for a bond option
double bond_option_price(double t, double S, double T, double K, double sigma1, double sigma2, double lambda, bool call, '') {
    double d1 = (1.0/(sigma1*sqrt(T-t)))*log(h(t, T, r, sigma1, sigma2, lambda)/K) + 0.5*sigma1*sqrt(T-t);
    double d2 = d1 - sigma1*sqrt(T-t);
    double option_price;
    if(call) {
        option_price = S*h(t, T, r, sigma1, sigma2, lambda)*cdf(d1) - K*exp(-r*(T-t))*cdf(d2);
    } else {
        option_price = K*exp(-r*(T-t))*(1-cdf(d2)) - S*h(t, T, r, sigma1, sigma2, lambda)*(1-cdf(d1));
    }
    return option_price;
}

// Function for calibrating the two-term volatility model to the volatility surface
void calibrate_volatility_surface(vector<double> bond_maturities, vector<double> bond_volatilities, double lambda, double& sigma1, double& sigma2) {
    int n = bond_maturities.size();

    // Construct matrix A and vector b for least-squares problem
    vector<double> T(n);
    vector<vector<double>> A(n, vector<double>(2));
    vector<double> b(n);
    for(int i=0; i<n; i++) {
        T[i] = bond_maturities[i];
        A[i][0] = 1.0;
        A[i][1] = (1-exp(-lambda*(T[i])))*sqrt((1-exp(-2*lambda*(T[i])))/(2*lambda));
        b[i] = bond_volatilities[i];
    }

    // Solve the least-squares problem
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            double a = A[j][i];
            for(int k=0; k<2; k++) {
                A[j][k] -= a*A[i][k]/A[i][i];
            }
            b[j] -= a*b[i]/A[i][i];
        }
    }
    sigma1 = b[n-1]/A[n-1][n-1];
    sigma2 = b[n-2] - A[n-2][n-1]*sigma1;
    sigma2 /= A[n-2][0];
}

// Function for calibrating the two-term volatility model to bond prices
void calibrate_bond_prices(double S, vector<pair<double,double>> bond_prices, double r, double lambda, double& sigma1, double& sigma2, const vector<double>& zcb_tenor, const vector<double>& zcb_curve○) {
    int n = bond_prices.size();
    vector<double> bond_maturities(n);
    vector<double> bond_volatilities(n);

    // Compute bond maturities
    for(int i=0; i<n; i++) {
        bond_maturities[i] = bond_prices[i].first;
    }

    // Initialize sigma2 to 1
    sigma2 = 1;

    // Loop to solve for sigma1
    double sigma1_old = 0;
    double eps = 1e-10;
    while(fabs(sigma1-sigma1_old) > eps) {
        sigma1_old = sigma1;
        double error = 0;
        for(int i=0; i<n; i++) {
            double T = bond_maturities[i];
            double P = bond_prices[i].second;
            double F = S/exp(-h(0, T, r, sigma1, sigma2, lambda)*(T-0));
            double P_model = F*h(0, T, r, sigma1, sigma2, lambda)*exp(-F*(exp(-h(0, T, r, sigma1, sigma2, lambda)*(T-0))-1));
            error += pow(P-P_model, 2);
        }
        sigma1 += 1e-5;
        double error_new = 0;
        for(int i=0; i<n; i++) {
            double T = bond_maturities[i];
            double P = bond_prices[i].second;
            double F = S/exp(-h(0, T, r, sigma1, sigma2, lambda)*(T-0));
            double P_model = F*h(0, T, r, sigma1, sigma2, lambda)*exp(-F*(exp(-h(0, T, r, sigma1, sigma2, lambda)*(T-0))-1));
            error_new += pow(P-P_model, 2);
        }
        if(error_new > error) {
            sigma1 -= 1e-5;
            break;
        }
    }

    // Calibrate sigma2 with least squares
    calibrate_volatility_surface(bond_maturities, bond_volatilities, lambda, sigma1, sigma2);
}

int main() {
    // Discount Curve
    vector<double> zcb_tenor = {1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {0.97, 0.95, 0.91, 0.9, 0.88}; // T -> P_0(T)
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

    auto int_sigma = [&](double s) -> double { 
        return sigma_sq_mod(sigma1, sigma2, lambda, s, S, T);} ;

    res = integral(int_sigma, t, S, n);

    // Compute and output option price
    double option_price = h(t, T, r, sigma1, sigma2, lambda);
    // cout << "Option price is: " << option_price ;
    return 0;
}