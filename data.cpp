#include "data.h"
#include <algorithm>
#include <cctype>

namespace ensiie
{

	void Data::discretize()
	{

		Smin_ = S0_ - (M_ * dS_ / 2.0);

		if (Smin_ < 0.0)
		{
			Smin_ = 0.0;
		}

		Smax_ = Smin_ + M_ * dS_;

		S_.resize(M_ + 1);

		for (int i = 0; i <= M_; i++)
		{
			S_[i] = Smin_ + i * dS_;
		}
	}

	void Data::validation()
	{
		if (t_ < 0.0)
			throw std::invalid_argument("t must be non-negative.");

		if (T_ < t_)
			throw std::invalid_argument("T must be non-smaller than t.");

		if (S0_ < 0.0)
			throw std::invalid_argument("S0 must be non-negative.");

		if (sigma_ < 0.0)
			throw std::invalid_argument("sigma must be non-negative.");

		if (N_ <= 0)
			throw std::invalid_argument("N must be a positive integer.");

		if (dS_ <= 0.0)
			throw std::invalid_argument("dS must be positive.");

		if (M_ <= 0)
			throw std::invalid_argument("M must be a positive integer.");
	}

	OptionType Data::parse_option_type(const std::string& optStr)
	{
		std::string s = optStr;
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return std::tolower(c); });

		if (s == "call")
			return OptionType::Call;

		if (s == "put")
			return OptionType::Put;

		throw std::invalid_argument("Option type must be 'call' or 'put' (any casing).");
	}

	Data::Data(double t, double T, double S0, double r, double sigma, int N, double dS, int M, const std::string& optionStr, unsigned long seed)
		: t_(t), T_(T), S0_(S0), r_(r), sigma_(sigma), N_(N), dS_(dS), M_(M), optionType_(parse_option_type(optionStr)), seed_(seed)
	{
		validation();
		discretize();
	}


	double Data::get_t() const
	{
		return t_;
	}

	double Data::get_T() const
	{
		return T_;
	}

	double Data::get_S0() const
	{
		return S0_;
	}

	double Data::get_r() const
	{
		return r_;
	}

	double Data::get_sigma() const
	{
		return sigma_;
	}

	int Data::get_N() const
	{
		return N_;
	}

	double Data::get_dS() const
	{
		return dS_;
	}

	int Data::get_M() const
	{
		return M_;
	}

	double Data::get_Smin() const
	{
		return Smin_;
	}

	double Data::get_Smax() const
	{
		return Smax_;
	}

	const std::vector<double>& Data::get_S() const
	{
		return S_;
	}

	OptionType Data::get_option_type() const
	{
		return optionType_;
	}

	unsigned long Data::get_seed() const
	{
		return seed_;
	}
}