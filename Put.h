#pragma once

#include "pricing.h"
#include <vector>
#include <string>

namespace ensiie
{
    /**
     * @brief Class representing the Put Option (implemented with Lookback payoff).
     *
     * The payoff is defined as max(S_t) - S_T over the time interval [0, T].
     */
    class Put : public Pricing
    {
    public:
        /**
         * @brief Constructor.
         *
         * @param t Initial time.
         * @param T Maturity time.
         * @param S0 Initial (spot) price.
         * @param r Risk-free interest rate.
         * @param sigma Underlying asset's volatility.
         * @param N Number of Monte Carlo Simulations.
         * @param dS Price grid step (inherited parameter).
         * @param M Number of discrete price nodes (inherited parameter).
         * @param seed Random number generator seed.
         */
        Put(double t, double T, double S0, double r, double sigma,
            int N, double dS, int M, unsigned long seed);

        /** @brief Default destructor. */
        ~Put() = default;

        /**
         * @brief Computes the payoff for a specific simulated path.
         *
         * Payoff = max(S_t) - S_T for t in [0, T].
         *
         * @param path Vector containing the simulated price trajectory.
         * @return The calculated payoff value.
         */
        double payoff(const std::vector<double>& path) const override;

        // --- GREEKS ---

        /**
		 * @brief Computes Delta using Forward Finite Differences (PUT option).
         * @return The sensitivity of the price to the underlying asset price.
         */
        double delta() const override;

        /**
		 * @brief Computes Gamma using Central Finite Differences (PUT option).
         * @return The sensitivity of Delta to the underlying asset price.
         */
        double gamma() const override;

        /**
		 * @brief Computes Vega using Forward Finite Differences (PUT option).
         * @return The sensitivity of the price to volatility.
         */
        double vega() const override;

        /**
		 * @brief Computes Theta using Forward Finite Differences (PUT option).
         * @return The sensitivity of the price to time passage.
         */
        double theta() const override;

        /**
		 * @brief Computes Rho using Forward Finite Differences (PUT option).
         * @return The sensitivity of the price to the risk-free rate.
         */
        double rho() const override;
    };
}