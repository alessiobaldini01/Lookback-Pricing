#include "Interface.h"
#include "Call.h"
#include "Put.h"
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
        if (argc < 12) {
            throw std::runtime_error("Insufficient arguments provided by Excel.");
        }

        // Mapping raw arguments to internal struct
        args_.mode = argv[1];
        args_.type = argv[2];
        args_.t = std::stod(argv[3]);
        args_.T = std::stod(argv[4]);
        args_.S0 = std::stod(argv[5]);
        args_.r = std::stod(argv[6]);
        args_.sigma = std::stod(argv[7]);
        args_.N = std::stoi(argv[8]);
        args_.dS = std::stod(argv[9]);
        args_.M = std::stoi(argv[10]);
        args_.seed = std::stoul(argv[11]);
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

        // Use Data's static method for case-insensitive type validation
        ensiie::OptionType type = ensiie::Data::parse_option_type(args_.type);

        // Polymorphic instantiation based on validated enum
        if (type == ensiie::OptionType::Call) {
            option = std::make_unique<Call>(args_.t, args_.T, args_.S0, args_.r, args_.sigma,
                args_.N, args_.dS, args_.M, args_.seed);
        }
        else {
            option = std::make_unique<Put>(args_.t, args_.T, args_.S0, args_.r, args_.sigma,
                args_.N, args_.dS, args_.M, args_.seed);
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
        ensiie::OptionType type = ensiie::Data::parse_option_type(args_.type);

        for (int i = 0; i <= args_.M; ++i)
        {
            double current_S = S_min + i * args_.dS;
            unsigned long current_seed = args_.seed + i; 

            std::unique_ptr<Pricing> option;

            if (type == ensiie::OptionType::Call) {
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