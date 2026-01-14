#pragma once
#include "data.h"

namespace ensiie
{
    /**
     * @brief Monte Carlo simulator for GBM paths with antithetic variates.
     *
     * Inherits market parameters from Data.
     * Stores N_ paths, each of length Nt_ + 1 (including the initial time).
     */
    class MonteCarlo : public Data
    {
    public:

        /**
         * @brief Constructor.
         *
         * Builds the time grid and immediately simulates the paths.
         */
        MonteCarlo(double t, double T, double S0, double r, double sigma,
            int N, double dS, int M, const std::string& optionStr, unsigned long seed);

        /**
         * @brief (Re)simulate all GBM paths using antithetic variates.
         *
         * Paths are stored in a matrix of size N_ x (Nt_ + 1).
         * Row i is the i-th path, column k is time step k.
         */
        void simulate_paths();

        /** @brief Returns the matrix of simulated paths. */
        const std::vector<std::vector<double>>& get_paths() const;

        /** @brief Returns the time grid (Nt_ + 1 points from t_ to T_). */
        const std::vector<double>& get_time_grid() const;

        /** @brief Returns the number of time steps (excluding initial). */
        int get_Nt() const;

        /** @brief Returns the time step size (dt). */
        double get_dt() const;

    private:
        int Nt_;                           ///< Number of time steps (e.g. days)
        double dt_;                        ///< Time step size (e.g. 1/365)
        std::vector<double> timeGrid_;     ///< Time grid of size Nt_ + 1
        std::vector<std::vector<double>> paths_; ///< N_ x (Nt_ + 1) matrix

        /** @brief Build the time grid from t_ to T_ using dt_. */
        void build_time_grid();
    };
}
