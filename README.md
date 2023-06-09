# Bond Option Pricing in a Heath-Jarrow-Morton Framework

This C++ code implements the Bond Option Pricing in a Heath-Jarrow-Morton (HJM) framework. The HJM model is a mathematical framework used for modeling the evolution of interest rates over time. In particular, this code calculates the price of an option on a Zero-Coupon-Bond (ZCB) using the HJM model.

The project contains:
- **utils.h**: a file with some utils.  It contains:
    * **linear_interpolation**: function to linearly interpolate a set of points
    * **integral**: function to integrate a function nn an interval
    * **cdf**: cumulative density funcion of a standard gaussian $N(0,1)$
- **main.cpp**: the main of the project. It contains:
    * **DiscountCurve**: a class that represents a Discount Curve with a linear interpolation
    * **HjmModel**: a class to manage an HJM framework with a two-factor volatility
    * **OptionOnZcb**: a class for an option on bond

#### DiscountCurve
The Discount Curve represents the relationship between the present value of a future cash flow and its maturity. The Discount Curve is represented as a vector of tenors and their corresponding values.
- Attributes:
    * tenor (vector double): a vector of doubles representing tenors
    * value (vector double): a vector of doubles representing the values of the tenors
- Methods:
    * at: evaluate the discount curve at a specific tenor with a linear interpoation

Example:
```cpp
int main(){
    // ZCB prices obtained from the market
    vector<double> zcb_tenor = {0, 1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {1, 0.97, 0.94, 0.91, 0.9, 0.88}; // T -> P_0(T)
    // Discount Curve Initialization
    DiscountCurve P(zcb_tenor, zcb_price);
    cout << P.at(0.5);
    return 0;
}
```

#### HjmModel
A class to collect the functions and the parameters of an HJM model with a two-factor volatility.
- Attributes:
    * sigma1 (double): instantaneous volatility
    * sigma2 (double): long-run time-dependent volatility
    * lambda (double): mean reversion parameter of sigma2
- Methods:
    * sigma_sq_mod: function that evaluates the instantaneous volatility depending on sigma1, sigma2 and lambda

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

#### OptionOnZcb
An OptionOnZcb represents an option on a Zero-Coupon Bond.
- Attributes
    * strike (double): the strike price of the option
    * option_ttm (double): the time to maturity of the option  
    * bond_ttm (double): the time to maturity of the underlying bond
    * iscall (bool): a boolean representing whether the option is a call option (true) or put option (false)
- Methods:
    * price: a method that evaluates the price of the option in a HJM framework

Example:
```cpp
int main(){
    // This is the conent of the main.cpp file
    // ZCB prices obtained from the market
    vector<double> zcb_tenor = {0, 1, 2, 3, 4, 5}; // T
    vector<double> zcb_price = {1, 0.97, 0.94, 0.91, 0.9, 0.88}; // T -> P_0(T)
    
    // HJM parameters
    double sigma1 = 0.3;
    double sigma2 = 0.2;
    double lambda = 2;

    // Options parameters
    double strike = 0.5;
    double option_maturity = 1.5; /* 1 year and half */
    double bond_maturity = 4.5; /* 5 years and half */
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
```
  
## HJM Model

In this section we recall the main results about the Heath–Jarrow–Morton (HJM) framework and we provide a closed formula for the pricing of an option on bond when the dynamic of the instantaneous forward rate is driven by a two-factor volatility.

The Heath Jarrow Morton (HJM) models consist in choosing a dynamic for the instantaneous forward rate $f_t(T)$, directly under the risk-neutral measure $\mathbb{Q}$. Therefore we choose processes $\alpha_t(T)$ and $\sigma_t(T)$ such that
```math
d f_t(T)= \alpha_t(T) dt +\sigma_t(T) dW_t
```
where $W_t$ is a Brownian motion under the risk-neutral measure. We  define directly the zero-coupon-bond as $P_t(T)$, the short rate $r_t$ and th bank account $B_t$ with the usual formula:
```math
r_t = f_t(t)
```
```math
B_t = e^{\int_0^t r_u du}
```
```math
P_t(T) = e^{\int_t^Tf_t(u) du}
```
 Under the risk-neutral measure the processes $\frac{P_t(T)}{B_t}$ must be martingales for all $T$. We recall the important theorem that shows the conditions on the processes $\alpha_t(T)$ and $\sigma_t(T)$, that guarantee that processes $\frac{P_t(T)}{B_t}$ are martingales:

The processes $\frac{P_t(T)}{B_t}$ are martingales if and only if
```math
	\alpha_t(T)= \sigma_t(T) \int_t^T \sigma_t(u) du
```


Once chosen an expression for the volatility $\sigma_t(T)$ we set
```math
df_t(T)=  \sigma_t(T) \int_t^T \sigma_t(u) du dt + \sigma_t(T) dW_t
```
where $f_0(T)$ are choosen in order to fit the the $f^*_0(T)$ instantaneous forward rates  observed in the market. This guarantee a model automatically calibrated to the bond prices observed in the market.

In practice $f^*_0(T)$ are not really observable in the market and often knowing them is not really necessary. Usually the curve observed in the market is $T \rightarrow P_0(T)$. 

### Closed Formula for a Vanilla Option on Bond
In this section we will suppose the following framework for the HJM volatility. Given $\sigma_1, \sigma_2$ and $ \lambda$ real numbers we define a two factor volatility
```math
\sigma_t(T) = ( \sigma_1, \sigma_2 e^{-\lambda(T-t)})
```

Let consider a call option with maturity $S$, strike $K$ and with underlying a zero-coupon-bond (ZCB) with maturity $T$. In the HJM framework, with the two factor volatility, the price of the call option is given by the following formula 
```math
C_t(S,T)= P_t(T) \Phi(d_1) - K P_t(S) \Phi(d_2)$
```
```math
d_1 = \frac{\log( \frac{P(T)}{P(S) K}) + \frac{1}{2} \Sigma^2_{S, T}(t)}{\sqrt{\Sigma^2_{S, T}(t)}}
```
```math
d_2 = d_1 - \sqrt{\Sigma^2_{S, T}(t)}
```
```math
\Sigma^2_{S, T}(t) = \int_t^S|| \int_S^T\sigma_s(u) du||^2 ds
```
where:

$P_t(T)$: Discount factor at time T (the price at $t$ of a ZCB with maturity $T$)

$P_t(S)$: Discount factor at time S (the price at $t$ of a ZCB with maturity $S$)

$K$: Strike price of the option on bond

$t$: evaluation time

$\Phi$: Cumulative distribution function of the standard normal distribution

This formula calculates the price of an option on a zero-coupon-bond based on the HJM model parameters and the discount factors from a given discount curve.