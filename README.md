# Bond Option Pricing in a Heat-Harrow-Morton Framework
This C++ code implements the Bond Option Pricing in a Heat-Harrow-Morton (HJM) framework. The HJM model is a mathematical framework used for modeling the evolution of interest rates over time. In particular, this code calculates the price of an option on a Zero-Coupon Bond (ZCB) using the HJM model.

## HJM Model
The HJM model is defined by three parameters:

sigma1 - volatility of the short rate
sigma2 - volatility of the long rate
lambda - mean reversion parameter
The HJM model also defines a function sigma_sq_mod(t, S, T), which calculates the square of the volatility between times S and T. The formula for sigma_sq_mod is:

```
pow(-sigma1*(T-S), 2) + pow(-sigma2*exp(lambda*t)*(exp(-lambda*S)-exp(-lambda*T))/lambda, 2)
```
Discount Curve
The Discount Curve represents the relationship between the present value of a future cash flow and its maturity. The Discount Curve is represented as a vector of tenors and their corresponding values. The class DiscountCurve has two data members:

tenor - a vector of doubles representing tenors
value - a vector of doubles representing the values of the tenors
OptionOnZcb
An OptionOnZcb represents an option on a Zero-Coupon Bond. The OptionOnZcb class has four data members:

strike - a double representing the strike price of the option
option_ttm - a double representing the time to maturity of the option
bond_ttm - a double representing the time to maturity of the underlying bond
iscall - a boolean representing whether the option is a call option (true) or put option (false)
The price of an OptionOnZcb is calculated using the Black-Scholes formula:

scss
Copy code
```
d1 = (ln(P(T)/P(S) * Strike) + 0.5 * sigma_sq_mod(S,T))/sqrt(sigma_sq_mod(S,T))
d2 = d1 - sqrt(sigma_sq_mod(S,T))
opt_price = P(T) * cdf(d1) - Strike * P(S) * cdf(d2)
```
where $P(T)$ and $P(S)$ are the discount factors for the bond maturity and option maturity, respectively, and cdf is the cumulative distribution function of the standard normal distribution.

## Main Function
The main function initializes the market ZCB prices, HJM model parameters, and option parameters. The DiscountCurve, HjmModel, and OptionOnZcb objects are initialized with their corresponding parameters. The option price is calculated using the price() function of the OptionOnZcb class. Finally, the option price is printed to the console.

