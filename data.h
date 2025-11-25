#pragma once
#include <vector>
#include <stdexcept>
#include <string>

namespace ensiie
{
	/** @brief Option type. */
	enum class OptionType { Call, Put };

	/**
	 * @brief Holds parameters and data structures for computations.
	 *
	 * Base class that encapsulates common parameters used in pricing and Monte Carlo simulation.
	 */
	class Data
	{
	protected:

		/** @brief Initial time. */
		const double t_;

		/** @brief Maturity time. Must be >= t_. */
		const double T_;

		/** @brief Initial (spot) price of the underlying asset. */
		const double S0_;

		/** @brief Risk-free interest rate. */
		const double r_;

		/** @brief Constant volatility of the underlying asset. */
		const double sigma_;

		/** @brief Number of Monte Carlo Simulations. */
		const int N_;

		/** @brief Price step in the state grid. */
		const double dS_;

		/** @brief Number of discrete price nodes in the grid. */
		const int M_;

		/** @brief Option type (Call or Put). */
		const OptionType optionType_;

		/** @brief Minimum price value in the grid. */
		double Smin_;

		/** @brief Maximum price value in the grid. */
		double Smax_;

		/** @brief Vector of discrete price nodes. */
		std::vector<double> S_;

		/** @brief Build the discretized price grid. */
		void discretize();

		/** @brief Check that all the input are feasible. */
		void validation();

		/** @brief Parse option type from string ("call","put"). */
		static OptionType parse_option_type(const std::string& optStr);

	public:
		/**
		 * @brief Constructor.
		 *
		 * Parameters follow the order of the protected members to avoid mismatches.
		 *
		 * @param t Initial time.
		 * @param T Maturity time.
		 * @param S0 Initial (spot) price.
		 * @param r Risk-free interest rate.
		 * @param sigma Underlying asset's volatility.
		 * @param N Number of Monte Carlo Simulations.
		 * @param dS Price grid step.
		 * @param M Number of discrete price nodes.
		 * @param optionStr Option type as string (\"call\" or \"put\", any casing).
		 */
		Data(double t, double T, double S0, double r, double sigma, int N, double dS, int M, const std::string& optionStr);

		/** @brief Returns the initial time. */
		double get_t() const;

		/** @brief Returns the maturity time. */
		double get_T() const;

		/** @brief Returns the initial (spot) price. */
		double get_S0() const;

		/** @brief Returns the risk-free rate. */
		double get_r() const;

		/** @brief Returns the volatility. */
		double get_sigma() const;

		/** @brief Returns the number of simulations. */
		int get_N() const;

		/** @brief Returns the price grid step. */
		double get_dS() const;

		/** @brief Returns the number of discrete price nodes in the grid. */
		int get_M() const;

		/** @brief Returns the minimum price in the grid. */
		double get_Smin() const;

		/** @brief Returns the maximum price in the grid. */
		double get_Smax() const;

		/** @brief Returns the vector of discretized price nodes. */
		const std::vector<double>& get_S() const;

		/** @brief Returns the option type. */
		OptionType get_option_type() const;
	};
}