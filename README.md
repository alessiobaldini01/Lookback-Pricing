# Project Documentation

## PROJECT STRUCTURE
```
/src             # C++ source files
/doc             # index.html (Doxygen HTML documentation)
Lookback.xlsm    # Excel VBA interface
```

## COMPILATION

From the project root directory, compile the C++ sources with:
```bash
g++ -std=c++17 -O2 -Wall -Wextra src/*.cpp -o pricer.exe
```

## EXECUTION

1. Ensure the compiled executable is named `pricer.exe`
2. Ensure `pricer.exe` is placed in the same directory as `Lookback.xlsm`
3. Open `Lookback.xlsm` and enable macros

## NOTES

- The executable name and its location are required for correct interaction with Excel VBA
- The Doxygen documentation is accessible from `index.html` and is not required for execution
