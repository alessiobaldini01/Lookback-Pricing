#include "pricing.h"
#include <cmath>    // std::exp, std::sqrt
#include <vector>

namespace ensiie
{
    double Pricing::price() const
    {
        // discounted mean payoff
        const double maturity = get_T() - get_t();
        const double discount = std::exp(-get_r() * maturity);
        return discount * payoff_mean();
    }

    double Pricing::payoff_mean() const
    {
        const auto& paths = get_paths();
        const int N = get_N();

        if (N == 0)
            return 0.0;

        double sum = 0.0;
        for (int i = 0; i < N; ++i)
        {
            sum += payoff(paths[i]);
        }

        return sum / static_cast<double>(N);
    }

    double Pricing::payoff_std() const
    {
        const int N = get_N();

        if (N < 2)
            return 0.0; // std not defined for N < 2, return 0 for safety

        const double mean = payoff_mean();
        double accum = 0.0;

        const auto& paths = get_paths();
        for (int i = 0; i < N; ++i)
        {
            double p = payoff(paths[i]);
            double diff = p - mean;
            accum += diff * diff;
        }

        return std::sqrt(accum / static_cast<double>(N - 1)); // unbiased estimator
    }

    double Pricing::payoff_stderr() const
    {
            const int N = get_N();
        if (N == 0)
            return 0.0;
        return payoff_std() / std::sqrt(static_cast<double>(N));
    }
}
