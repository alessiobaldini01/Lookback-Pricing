#pragma once
#include "data.h"

namespace ensiie
{
    class MonteCarlo : public Data
    {
    public:
        MonteCarlo(double t, double T, double S0, double r, double sigma,
            int N, double dS, int M, const std::string& optionStr);

        void simulate_paths();

        const std::vector<std::vector<double>>& get_paths() const;
        const std::vector<double>& get_time_grid() const;

        int get_Nt() const;
        double get_dt() const;

    private:
        int Nt_;                           // number of days
        double dt_;                        // 1/365
        std::vector<double> timeGrid_;     // Nt+1 times
        std::vector<std::vector<double>> paths_; // N_ x (Nt_+1)

        void build_time_grid();
    };
}
