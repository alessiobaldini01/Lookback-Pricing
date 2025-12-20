#pragma once
#include "pricing.h"
#include <vector>
#include <string>

namespace ensiie
{
    /**
     * @brief Class representing the Option (implemented with Lookback payoff).
     *
     * The payoff is defined as S_T - min(S_t) over the time interval [0, T].
     */
    class Call : public Pricing
    {
    public:
        /**
         * @brief Constructor.
         *
         *
         * @param t Initial time.
         * @param T Maturity time.
         * @param S0 Spot price.
         * @param r Risk-free interest rate.
         * @param sigma Underlying asset's volatility.
         * @param N Number of Monte Carlo Simulations.
         * @param dS Price grid step (inherited parameter).
         * @param M Number of discrete price nodes (inherited parameter).
         * @param seed Random number generator seed.
         */
        
        Call(double t, double T, double S0, double r, double sigma,
            int N, double dS, int M, unsigned long seed);

        /** @brief Default destructor. */
        ~Call() = default;

        /**
         * @brief Computes the payoff for a specific simulated path.
         *
         * Logic: Payoff = S_T - min(S_t) for t in [0, T].
         *
         * @param path Vector containing the simulated price trajectory.
         * @return The calculated payoff value.
         */
        double payoff(const std::vector<double>& path) const override;

        // --- GREEKS ---

        /**
		 * @brief Computes Delta using Forward Finite Differences (CALL option).
         * @return The sensitivity of the price to the underlying asset price.
         */
        double delta() const override;

        /**
		 * @brief Computes Gamma using Central Finite Differences (CALL option).
         * @return The sensitivity of Delta to the underlying asset price.
         */
        double gamma() const override;

        /**
		 * @brief Computes Vega using Forward Finite Differences (CALL option).
         * @return The sensitivity of the price to volatility.
         */
        double vega() const override;

        /**
		 * @brief Computes Theta using Forward Finite Differences (CALL option).
         * @return The sensitivity of the price to time passage.
         */
        double theta() const override;

        /**
		 * @brief Computes Rho using Forward Finite Differences (CALL option).
         * @return The sensitivity of the price to the risk-free rate.
         */
        double rho() const override;
    };
}