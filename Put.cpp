#include "Put.h"
#include <algorithm> 
#include <cmath>     

namespace ensiie
{
    //  CONSTRUCTOR 
    Put::Put(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, unsigned long seed)
        : Pricing(t, T, S0, r, sigma, N, dS, M, "put", seed)
    {
    }

    //  PAYOFF 
    double Put::payoff(const std::vector<double>& path) const
    {
        double ST = path.back();

        // Finds the maximum element in the entire path (Lookback Put)
        double Smax = *std::max_element(path.begin(), path.end());

        // Payoff Lookback Put: Smax - ST
        return Smax - ST;
    }

    //  GREEKS IMPLEMENTATION

    double Put::delta() const
    {
        double eps_delta = 0.01;

        double h = S0_ * eps_delta;
        if (h < 1e-4) h = 1e-4;

        // Forward difference: Create a new Put with S0 + h
        Put up(t_, T_, S0_ + h, r_, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / h;
    }

    double Put::gamma() const
    {
        double eps_gamma = 0.01;

        double h = S0_ * eps_gamma;
        if (h < 1e-4) h = 1e-4;

        // Central difference
        Put up(t_, T_, S0_ + h, r_, sigma_, N_, dS_, M_, seed_);
        Put down(t_, T_, S0_ - h, r_, sigma_, N_, dS_, M_, seed_);

        double price_curr = this->price();

        return (up.price() - 2.0 * price_curr + down.price()) / (h * h);
    }

    double Put::vega() const
    {
        double eps_vega = 0.01;

        Put up(t_, T_, S0_, r_, sigma_ + eps_vega, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_vega;
    }

    double Put::theta() const
    {
        double eps_theta = 1.0 / 365.0; 

        // Check time bounds using member variables t_ and T_
        if (t_ + eps_theta >= T_) eps_theta = (T_ - t_) * 0.5;

        Put up(t_ + eps_theta, T_, S0_, r_, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_theta;
    }

    double Put::rho() const
    {
        double eps_rho = 0.0001;

        Put up(t_, T_, S0_, r_ + eps_rho, sigma_, N_, dS_, M_, seed_);

        return (up.price() - this->price()) / eps_rho;
    }
}