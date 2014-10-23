#include <iostream>

namespace math
{
	class imp_double
	{
	public:
		imp_double();
		imp_double(const double);
		imp_double(const double, const double);

		~imp_double();

		imp_double(const imp_double&);
		imp_double& operator=(const imp_double& other);

		bool operator<(const imp_double&) const;
		bool operator>(const imp_double&) const;
		bool operator<=(const imp_double&) const;
		bool operator>=(const imp_double&) const;
		bool operator==(const imp_double&) const;
		bool operator!=(const imp_double&) const;

		imp_double operator-() const;

		imp_double& operator+=(const imp_double&);
		imp_double& operator-=(const imp_double&);
		imp_double& operator*=(const imp_double&);
		imp_double& operator/=(const imp_double&);

		imp_double operator+(const imp_double&) const;
		imp_double operator-(const imp_double&) const;
		imp_double operator*(const imp_double&) const;
		imp_double operator/(const imp_double&) const;

		double value() const;
		double precision() const;

	private:
		const double eps = 1e-15;

		double value_;
		double precision_;
	};

	std::istream& operator>>(std::istream& in, imp_double& d);
	std::ostream& operator<<(std::ostream& out, const imp_double& d);
} // imp_double definition