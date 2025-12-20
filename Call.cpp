#include "call.h"
#include <algorithm> 
#include <cmath>     

namespace ensiie
{
    //  CONSTRUCTOR 
    Call::Call(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, unsigned long seed)
        : Pricing(t, T, S0, r, sigma, N, dS, M, "call", seed)
    {
    }

    //  PAYOFF 
    double Call::payoff(const std::vector<double>& path) const
    {
        double ST = path.back();
        
        // Finds the minimum element in the entire path (including S0)
        double Smin = *std::min_element(path.begin(), path.end());

        // Payoff Lookback Floating Strike: ST - Smin
        return ST - Smin;
    }

    //  GREEKS IMPLEMENTATION 

    // DELTA
    double Call::delta() const
    {
        // Perturbazione relativa dell'1% su S0
        double eps_delta = 0.01;

        double h = S0_ * eps_delta;
        if (h < 1e-4) h = 1e-4;

        // Forward difference: Create a new Call with S0 + h
        Call up(t_, T_, S0_ + h, r_, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / h;
    }
    // GAMMA
    double Call::gamma() const
    {
        double eps_gamma = 0.01;

        double h = S0_ * eps_gamma;
        if (h < 1e-4) h = 1e-4;

        // Central difference: Requires (S0 + h) and (S0 - h)
        // Using the same seed ensures variance reduction
        Call up(t_, T_, S0_ + h, r_, sigma_, N_, dS_, M_, seed_);
        Call down(t_, T_, S0_ - h, r_, sigma_, N_, dS_, M_, seed_);

        double price_curr = this->price();

        return (up.price() - 2.0 * price_curr + down.price()) / (h * h);
    }

    // VEGA
    double Call::vega() const
    {
        double eps_vega = 0.01;

        Call up(t_, T_, S0_, r_, sigma_ + eps_vega, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_vega;
    }

    // THETA
    double Call::theta() const
    {
        double eps_theta = 1.0 / 365.0;

        if (t_ + eps_theta >= T_) eps_theta = (T_ - t_) * 0.5;

        Call up(t_ + eps_theta, T_, S0_, r_, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_theta;
    }
    
	// RHO
    double Call::rho() const
    {
        double eps_rho = 0.0001;

        Call up(t_, T_, S0_, r_ + eps_rho, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_rho;
    }
}