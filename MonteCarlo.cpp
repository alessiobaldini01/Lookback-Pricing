#include "MonteCarlo.h"

#include <random>
#include <cmath>

namespace ensiie
{
    MonteCarlo::MonteCarlo(double t, double T, double S0, double r, double sigma,
        int N, double dS, int M, const std::string& optionStr)
        : Data(t, T, S0, r, sigma, N, dS, M, optionStr)
    {
        // Discretizzazione temporale in giorni
        dt_ = 1.0 / 365.0;
        Nt_ = static_cast<int>((T_ - t_) * 365.0);

        if (Nt_ <= 0)
            throw std::invalid_argument("Invalid time domain: T must be larger than t by at least 1 day.");

        build_time_grid();

        simulate_paths();
    }

    void MonteCarlo::build_time_grid()
    {
        timeGrid_.resize(Nt_ + 1);

        for (int k = 0; k <= Nt_; ++k)
            timeGrid_[k] = t_ + k * dt_;
    }

    void MonteCarlo::simulate_paths()
    {
        paths_.assign(N_, std::vector<double>(Nt_ + 1));

        // Drift & diffusione del GBM
        const double muTerm = (r_ - 0.5 * sigma_ * sigma_) * dt_;
        const double sigmaTerm = sigma_ * std::sqrt(dt_);

        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::normal_distribution<double> normal(0.0, 1.0);

        for (int i = 0; i < N_; ++i)
        {
            auto& path = paths_[i];
            path[0] = S0_;

            for (int k = 1; k <= Nt_; ++k)
            {
                double Z = normal(gen);
                double increment = muTerm + sigmaTerm * Z;
                path[k] = path[k - 1] * std::exp(increment);
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
