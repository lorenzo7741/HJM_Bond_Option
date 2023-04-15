# Bond Option Pricing in a Heath-Jarrow-Morton Framework

  

This C++ code implements the Bond Option Pricing in a Heath-Harrow-Morton (HJM) framework. The HJM model is a mathematical framework used for modeling the evolution of interest rates over time. In particular, this code calculates the price of an option on a Zero-Coupon Bond (ZCB) using the HJM model.

  

  

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

```
d1 = (ln(P(T)/P(S) * Strike) + 0.5 * sigma_sq_mod(S,T))/sqrt(sigma_sq_mod(S,T))
d2 = d1 - sqrt(sigma_sq_mod(S,T))
opt_price = P(T) * cdf(d1) - Strike * P(S) * cdf(d2)
where $P(T)$ and $P(S)$ are the discount factors for the bond maturity and option maturity, respectively, and cdf is the cumulative distribution function of the standard normal distribution.

  

## Main Function
The main function initializes the market ZCB prices, HJM model parameters, and option parameters. The DiscountCurve, HjmModel, and OptionOnZcb objects are initialized with their corresponding parameters. The option price is calculated using the price() function of the OptionOnZcb class. Finally, the option price is printed to the console.

  
```
## HJM Model

In this section we recall the main results about the Heath–Jarrow–Morton (HJM) framework and we provide a closed formula for the pricing of an option on bond when the dynamic of the instantaneous forward rate is driven by a two-factor volatility.

The Heath Jarrow Morton (HJM) models consist in choosing a dynamic for the instantaneous forward rate $f_t(T)$, directly under the risk-neutral measure $\mathbb{Q}$. Therefore we choose processes $\alpha_t(T)$ and $\sigma_t(T)$ such that
```math
f_t(T)= \alpha_t(T) dt +\sigma_t(T) dW_t
```
where $W_t$ is a Brownian motion under the risk-neutral measure. We  define directly the zero-coupon bond as $P_t(T)$, the short rate $r_t$ and th bank account $B_t$ with the usual formula:
```math
r_t = f_t(t)
B_t = e^{\int_0^t r_u du}
P_t(T) = e^{\int_t^Tf_t(u) du}
```
 Under the risk-neutral measure the processes $\frac{P_t(T)}{B_t}$ must be martingales for all $T$. The following important theorem shows the conditions on the processes $\alpha_t(T)$ and $\sigma_t(T)$, that guarantee that the processes $\frac{P_t(T)}{B_t}$ are martingales.
\begin{teo}[Heath Jarrow Morton]
	The processes $\frac{P_t(T)}{B_t}$ are martingales if and only if
	\begin{equation} \label{HJM}
	\alpha_t(T)= \sigma_t(T) \int_t^T \sigma_t(u) du
	\end{equation}
\end{teo}
Once chosen an expression for the volatility $\sigma_t(T)$ we can set
\[
f_t(T)= f^*_0(T)+ \int_0^t \alpha_s(T) ds + \int_0^t \sigma_s(T) dW_s
\]
where $f^*_0(T)$ are the instantaneous forward rates values observed in the market, hence the model is automatically calibrated to the bond prices.


HJM models sta
rt by modelling under the risk-neutral probability the instantaneous forward rate $f_t(T)$. Let $P_t(T)$ the price at time $t$ of a zero-coupon-bond (ZCB) that guarantees the holder an amount of $1$ at the maturity $T$. We recall that

```math
P_t(T) = e^{\int_t^Tf_t(u) du}
```

The main result of the HJM framework says that, if

Therefore we suppose for $f_t(T)$,

  

The mathematical formula for the method price in the OptionOnZcb class can be written as:

$C_t(S,T)= P_t(T) \Phi(d_1) - K P_t(S) \Phi(d_2)$

where:

  

$P_t(T)$: Discount factor at time T

$P_t(S)$: Discount factor at time S

$K$: Strike price of the option on bond

$\Phi(d_1)$: Cumulative distribution function of the standard normal distribution evaluated at $d_1$


$d_1 = \frac{ln(P(T)/(P(S) * strike)) + \frac{1}{2} * \int_{0}^{S} \sigma^2(u, S, T) du}{\sqrt{\int_{0}^{S} \sigma^2(u, S, T) du}}$

$\sigma^2(u, S, T) = \sigma_1^2 * (T - S)^2 + \frac{\sigma_2^2}{2\lambda} * (1 - e^{-2\lambda(S-u)}) * (1 - e^{-\lambda(T-S)})^2$

$\Phi(d_2)$: Cumulative distribution function of the standard normal distribution evaluated at $d_2$

  
  

$d_2 = d_1 - \sqrt{\int_{0}^{S} \sigma^2(u, S, T) du}$

  
  

This formula calculates the price of an option on a zero coupon bond based on the HJM model parameters and the discount factors from a given discount curve.