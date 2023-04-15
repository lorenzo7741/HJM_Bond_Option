//#ifndef FILENAME_H
//#define FILENAME_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <functional>

using namespace std;

double linear_interpolation(const vector<double> &x, const vector<double> &fx, double x0) 
{
    // check that x has at least 2 elements
    if (x.size() < 2) {
        throw invalid_argument("x vector must have at least 2 elements");
    }
    // check that x and f have the same length
    if (x.size() != fx.size()) {
        throw invalid_argument("x and fx vectors must have the same length");
    }
    // find the interval containing x0
    if (x0 < x.front()) {
        throw invalid_argument("x0 is less than the smallest x value");
    }
    else {

        auto it = upper_bound(x.begin(), x.end(), x0, [](double a, double b){return a <= b;});
        if (it == x.end()) {
            throw invalid_argument("x0 is greater than the largest x value");
        }
        size_t i = distance(x.begin(), it) - 1;
        // calculate the slope of the line between (x[i], f[i]) and (x[i+1], f[i+1])
        double slope = (fx[i+1] - fx[i]) / (x[i+1] - x[i]);
        // calculate the y-intercept of the line
        double y_intercept = fx[i] - slope * x[i];
        // calculate the value of the function at x_0 using the linear interpolation
        double fx0 = slope * x0 + y_intercept;
        return fx0;
    }
}

double integral(function<double (double)> f, double a, double b, int n) {
    double h = (b - a) / n; // Interval size
    double sum = 0.5 * (f(a) + f(b)); // Sum boundary points
    for (int i = 1; i < n; i++) {
        double x = a + i * h; // Compute x
        sum += f(x); // Add sum of function values
    }
    sum *= h; // Multiply by interval size
    return sum; // Return result
}

double cdf(double x) {
    return 0.5*(1+erf(x/sqrt(2.0)));
}

//#endif
