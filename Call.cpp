#include "call.h"
#include <algorithm> 
#include <cmath>     

namespace ensiie
{
    // CONSTRUCTOR 
    Call::Call(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, unsigned long seed)
        : Pricing(t, T, S0, r, sigma, N, dS, M, "call", seed)
    {
    }

    // PAYOFF 
    double Call::payoff(const std::vector<double>& path) const
    {
        double ST = path.back();
        
        // Finds the minimum element in the entire path (including S0)
        double Smin = *std::min_element(path.begin(), path.end());

        // Payoff Lookback Floating Strike: ST - Smin
        return ST - Smin;
    }

    // GREEKS IMPLEMENTATION 
    // DELTA 

    double Call::delta() const
    {
        const auto& paths = get_paths();
        const int N = get_N();
        const double maturity = get_T() - get_t();
        const double discount = std::exp(-get_r() * maturity);

        double sum = 0.0;
        for (int i = 0; i < N; ++i) {
            double ST = paths[i].back();
            double Smin = *std::min_element(paths[i].begin(), paths[i].end());
            // Pathwise derivative: (ST - Smin) / S0
            sum += (ST - Smin) / S0_;
        }

        return discount * sum / static_cast<double>(N);
    }


    // GAMMA
    double Call::gamma() const
    {
        return 0.0;
    }


    // VEGA
    double Call::vega() const
    {
        const auto& paths = get_paths();
        const int N = get_N();
        const int Nt = get_Nt();
        const double dt = get_dt();
        const double maturity = get_T() - get_t();
        const double discount = std::exp(-get_r() * maturity);
        const double sqrt_dt = std::sqrt(dt);

        double sum = 0.0;

        // Optimization: Allocate memory once outside the loop
        std::vector<double> dS_dsigma(Nt + 1);

        for (int i = 0; i < N; ++i) {
            const auto& path = paths[i];

            // Reset the derivatives vector for the current path
            std::fill(dS_dsigma.begin(), dS_dsigma.end(), 0.0);

            // S0 does not depend on sigma, so dS_dsigma[0] remains 0.0

            // Find the index of the minimum element (Lookback Call depends on Smin)
            auto min_it = std::min_element(path.begin(), path.end());
            auto min_idx = std::distance(path.begin(), min_it);

            // Recursively compute dS_k/dsigma
            for (int k = 1; k <= Nt; ++k) {
                double S_prev = path[k - 1];
                double S_curr = path[k];

                // Reconstruct Z_k from the path (inverse of GBM step)
                double drift = (r_ - 0.5 * sigma_ * sigma_) * dt;
                double Z_k = (std::log(S_curr / S_prev) - drift) / (sigma_ * sqrt_dt);

                // Pathwise formula: dS_k/dsigma
                // It applies the chain rule accumulating sensitivity from previous steps
                double exp_term = std::exp((r_ - 0.5 * sigma_ * sigma_) * dt + sigma_ * sqrt_dt * Z_k);

                dS_dsigma[k] = dS_dsigma[k - 1] * exp_term  
                    + S_curr * (-sigma_ * dt + sqrt_dt * Z_k);

            }

            // d(payoff)/dsigma = dST/dsigma - dSmin/dsigma
            // (Since Call Payoff = ST - Smin)
            double dpayoff_dsigma = dS_dsigma[Nt] - dS_dsigma[min_idx];

            sum += dpayoff_dsigma;
        }

        return discount * sum / static_cast<double>(N);
    }



    // THETA
    double Call::theta() const
    {
        double eps_theta = 1.0 / 252.0;

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