#pragma once

#include "MonteCarlo.h"

namespace ensiie
{
    /**
     * @brief Abstract base class for Monte Carlo pricing.
     *
     * This class inherits:
     *   - from MonteCarlo: all GBM simulated paths and market parameters.
     *
     * It provides:
     *   - a general price() method that:
     *        * evaluates the payoff on each simulated path
     *        * computes the mean
     *
     * Child classes must implement:
     *   - payoff(path)
     *     where 'path' is a vector<double> containing the entire simulated trajectory.
     */
    class Pricing : public MonteCarlo
    {
    public:
        Pricing(double t, double T, double S0, double r, double sigma,
            int N, double dS, int M, const std::string& optionType, unsigned long seed)
            : MonteCarlo(t, T, S0, r, sigma, N, dS, M, optionType,seed)
        {
        }

        virtual ~Pricing() = default;

        /// Pure virtual payoff: must be implemented in concrete pricing classes.
        virtual double payoff(const std::vector<double>& path) const = 0;

        /// Computes the Monte Carlo price.
        double price() const;

        double payoff_mean() const;              // mean payoff before discount
        double payoff_std() const;               // std of payoff
        double payoff_stderr() const;            // std / sqrt(N)

        virtual double delta() const = 0;
        virtual double gamma() const = 0;
        virtual double vega() const = 0;
        virtual double theta() const = 0;
        virtual double rho() const = 0;

    };
}
