#include "Interface.h"
#include <iostream>
#include <stdexcept>


int main(int argc, char* argv[]) {
    try {
        // Initialize the interface with command line arguments from Excel
        ensiie::Interface app(argc, argv);

        // Execute the full calculation (Pricing + Graphs)
        app.run();
    }

    catch (const std::invalid_argument& e) {
        // Specifically catch input errors like "cALl" 
        std::cerr << "Input Error: " << e.what() << std::endl;
        return 1; 
    }
    catch (const std::exception& e) {
        // Catch any other generic runtime exceptions
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1; 
    }
    catch (...) {
        // Ultimate fallback to prevent Excel from freezing during a crash
        std::cerr << "Unknown critical error occurred." << std::endl;
        return 1; 
    }

    // Success: Excel receives exit code 0 and knows data is valid
    return 0;
}