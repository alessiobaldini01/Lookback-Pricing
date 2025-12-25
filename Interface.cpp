#include "Interface.h"
#include "call.h"
#include "put.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace ensiie
{
    // Constructor calling the argument parser
    Interface::Interface(int argc, char* argv[])
    {
        parse_arguments(argc, argv);
    }

    // Convert command line strings into numeric 
    void Interface::parse_arguments(int argc, char* argv[])
    {
        // Check for 12  arguments 
        if (argc < 11) {
            throw std::runtime_error("Insufficient arguments provided by Excel.");
        }

        // Mapping raw arguments to internal struct
        args_.type = argv[1];
        args_.t = std::stod(argv[2]);
        args_.T = std::stod(argv[3]);
        args_.S0 = std::stod(argv[4]);
        args_.r = std::stod(argv[5]);
        args_.sigma = std::stod(argv[6]);
        args_.N = std::stoi(argv[7]);
        args_.dS = std::stod(argv[8]);
        args_.M = std::stoi(argv[9]);
        args_.seed = std::stoul(argv[10]);
    }

    
    void Interface::run()
    {
        // Optimization: disable C-stream synchronization for faster output
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);

        // Set fixed decimal precision for financial results
        std::cout << std::fixed << std::setprecision(6);

        // Execute both modes in a single sequential stream
        run_pricing_mode();
        run_graph_mode();
    }

    // Calculate and print Price and Greeks 
    void Interface::run_pricing_mode()
    {
        std::unique_ptr<Pricing> option;

        std::string type = args_.type;
        std::transform(type.begin(), type.end(), type.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (type == "call") {
            option = std::make_unique<Call>(args_.t, args_.T, args_.S0, args_.r, args_.sigma,
                args_.N, args_.dS, args_.M, args_.seed);
        }
        else if (type == "put") {
            option = std::make_unique<Put>(args_.t, args_.T, args_.S0, args_.r, args_.sigma,
                args_.N, args_.dS, args_.M, args_.seed);
        }
        else {
            throw std::runtime_error("Invalid option type. Use 'call' or 'put'.");
        }

        // Print results formatted for Excel: Price;Delta;Gamma;Theta;Rho;Vega
        std::cout << option->price() << ";"
            << option->delta() << ";"
            << option->gamma() << ";"
            << option->theta() << ";"
            << option->rho() << ";"
            << option->vega() << "\n";
    }

    // Loop through spot prices to generate graph data points
    void Interface::run_graph_mode()
    {
        // Centering the price range around S0
        double S_min = std::max(0.0, args_.S0 - (args_.M * args_.dS / 2.0));

        // Validate type once before starting 
        std::string type = args_.type;
        std::transform(type.begin(), type.end(), type.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (type != "call" && type != "put")
            throw std::runtime_error("Invalid option type. Use 'call' or 'put'.");


        for (int i = 0; i <= args_.M; ++i)
        {
            double current_S = S_min + i * args_.dS;
            unsigned long current_seed = args_.seed + i; 

            std::unique_ptr<Pricing> option;

            if (type == "call") {
                option = std::make_unique<Call>(args_.t, args_.T, current_S, args_.r, args_.sigma,
                    args_.N, args_.dS, args_.M, current_seed);
            }
            else {
                option = std::make_unique<Put>(args_.t, args_.T, current_S, args_.r, args_.sigma,
                    args_.N, args_.dS, args_.M, current_seed);
            }

            // Print graph row: Spot;Price;Delta
            std::cout << current_S << ";"
                << option->price() << ";"
                << option->delta() << "\n";
        }

        // Ensure all data is sent through the pipe
        std::cout << std::flush;
    }
}