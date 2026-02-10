#include "put.h"
#include <algorithm> 
#include <cmath>     

namespace ensiie
{
    // CONSTRUCTOR 
    Put::Put(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, unsigned long seed)
        : Pricing(t, T, S0, r, sigma, N, dS, M, "put", seed)
    {
    }

    // PAYOFF 
    double Put::payoff(const std::vector<double>& path) const
    {
        double ST = path.back();

        // Finds the maximum element in the entire path (Lookback Put)
        double Smax = *std::max_element(path.begin(), path.end());

        // Payoff Lookback Put: Smax - ST
        return Smax - ST;
    }

    // GREEKS IMPLEMENTATION

	// DELTA

    double Put::delta() const
    {
        const auto& paths = get_paths();
        const int N = get_N();
        const double maturity = get_T() - get_t();
        const double discount = std::exp(-get_r() * maturity);

        double sum = 0.0;
        for (int i = 0; i < N; ++i) {
            double ST = paths[i].back();
            double Smax = *std::max_element(paths[i].begin(), paths[i].end());
            // Pathwise derivative: (Smax - ST) / S0
            sum += (Smax - ST) / S0_;
        }

        return discount * sum / static_cast<double>(N);
    }

	// GAMMA
    double Put::gamma() const
    {
        return 0.0;
    }

	// VEGA
    double Put::vega() const
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

            // Find the index of the maximum element (Lookback Put depends on Smax)
            auto max_it = std::max_element(path.begin(), path.end());
            auto max_idx = std::distance(path.begin(), max_it);

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

            // d(payoff)/dsigma = dSmax/dsigma - dST/dsigma
            // (Since Put Payoff = Smax - ST)
            double dpayoff_dsigma = dS_dsigma[max_idx] - dS_dsigma[Nt];

            sum += dpayoff_dsigma;
        }

        return discount * sum / static_cast<double>(N);
    }


	// THETA
    double Put::theta() const
    {
        double eps_theta = 1.0 / 252.0;

        // Check time bounds using member variables t_ and T_
        if (t_ + eps_theta >= T_)
            eps_theta = (T_ - t_) * 0.5;

        Put now(t_, T_, S0_, r_, sigma_, N_, dS_, M_, seed_);
        Put forward(t_ + eps_theta, T_, S0_, r_, sigma_, N_, dS_, M_, seed_);

        // Forward finite difference
        return (forward.price() - now.price()) / eps_theta;
    }

	// RHO
    double Put::rho() const
    {
        double eps_rho = 0.0001;

        Put now(t_, T_, S0_, r_, sigma_, N_, dS_, M_, seed_);
        Put up(t_, T_, S0_, r_ + eps_rho, sigma_, N_, dS_, M_, seed_);

        // Forward finite difference
        return (up.price() - now.price()) / eps_rho;
    }

}