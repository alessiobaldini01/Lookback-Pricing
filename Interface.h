#pragma once

#include <string>
#include <vector>
#include "data.h"


namespace ensiie {

    /**
     * @brief Interface to comunicate with Excel
     * * This class handles the parsing of command-line arguments.
     */

    class Interface {
    public:
        /**
         * @brief Constructor that initializes the interface with command-line arguments.
         * @param argc Number of command-line arguments.
         * @param argv Array of command-line argument strings.
         */
        Interface(int argc, char* argv[]);

        /**
         * @brief Main execution entry point.
         */
        void run();

    private:
        /**
         * @struct InputArgs
         * @brief Internal structure to store parsed market and simulation parameters.
         */
        struct InputArgs {
            std::string type;  
            double t;          
            double T;          
            double S0;         
            double r;          
            double sigma;      
            double dS;         
            int N;             
            int M;             
            unsigned long seed;
        } args_;

        /**
         * @brief Converts raw command-line strings into numeric data.
         * @param argc Number of arguments.
         * @param argv Array of strings.
         */
        void parse_arguments(int argc, char* argv[]);

        /**
         * @brief Calculates Price and all Greeks (Delta, Gamma, Theta, Rho, Vega) 
         */
        void run_pricing_mode();
        /**
        * @brief Calculates and prints M rows of: Spot;Price;Delta.         */
        void run_graph_mode();
    };
}
 