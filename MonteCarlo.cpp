#include "MonteCarlo.h"

#include <random>
#include <cmath>

namespace ensiie
{
    MonteCarlo::MonteCarlo(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, const std::string& optionStr, unsigned long seed)
        : Data(t, T, S0, r, sigma, N, dS, M, optionStr, seed)
    {
        // Time discretization: daily steps between t_ and T_.
        dt_ = 1.0 / 365.0;
        Nt_ = static_cast<int>((T_ - t_) * 365.0);

        if (Nt_ <= 0)
            throw std::invalid_argument(
                "Invalid time domain: T must be larger than t by at least 1 day.");

        build_time_grid();
        simulate_paths();
    }

    void MonteCarlo::build_time_grid()
    {
        timeGrid_.resize(Nt_ + 1);
        for (int k = 0; k <= Nt_; ++k)
        {
            timeGrid_[k] = t_ + k * dt_;
        }
    }

    void MonteCarlo::simulate_paths()
    {
        // Initialize matrix: N_ paths, each with Nt_ + 1 time steps (including S0 at k=0)
        paths_.assign(N_, std::vector<double>(Nt_ + 1));

        // GBM parameters for one step:
        // S_{t+dt} = S_t * exp((r - 0.5 sigma^2) dt + sigma sqrt(dt) Z)
        const double muTerm = (r_ - 0.5 * sigma_ * sigma_) * dt_;
        const double sigmaTerm = sigma_ * std::sqrt(dt_);

        
        std::mt19937_64 gen(seed_);
        std::normal_distribution<double> normal(0.0, 1.0);

        // Use antithetic variates in pairs (i, i+1)
        int i = 0;
        for (; i + 1 < N_; i += 2)
        {
            auto& path1 = paths_[i];
            auto& path2 = paths_[i + 1];

            // Initial condition
            path1[0] = S0_;
            path2[0] = S0_;

            for (int k = 1; k <= Nt_; k++)
            {
                // One Gaussian draw and its antithetic counterpart
                double Z = normal(gen);
                double Za = -Z;

                double incr1 = muTerm + sigmaTerm * Z;
                double incr2 = muTerm + sigmaTerm * Za;

                path1[k] = path1[k - 1] * std::exp(incr1);
                path2[k] = path2[k - 1] * std::exp(incr2);
            }
        }

        // If N_ is odd, simulate the last path without an antithetic pair
        if (i < N_)
        {
            int last = N_ - 1;
            auto& path = paths_[last];
            path[0] = S0_;

            for (int k = 1; k <= Nt_; k++)
            {
                double Z = normal(gen);
                double incr = muTerm + sigmaTerm * Z;
                path[k] = path[k - 1] * std::exp(incr);
            }
        }
    }

    const std::vector<std::vector<double>>& MonteCarlo::get_paths() const
    {
        return paths_;
    }

    const std::vector<double>& MonteCarlo::get_time_grid() const
    {
        return timeGrid_;
    }

    int MonteCarlo::get_Nt() const
    {
        return Nt_;
    }

    double MonteCarlo::get_dt() const
    {
        return dt_;
    }
}
