#include <iostream>
#include <cmath>

class expression
{
public:
	virtual double evaluate() = 0;

	void println()
	{
		print();
		std::cout << std::endl;
	}

	virtual void print() = 0;
};

class binary_operation: public expression
{
public:
	binary_operation(expression* left, expression* right)
		: left_(left)
		, right_(right)
	{}

	void print() override
	{
		std::cout << "(";

		left_->print();

		std::cout << " ";
		print_sign();
		std::cout << " ";

		right_->print();

		std::cout << ")";
	}

	virtual void print_sign() = 0;

protected:
	expression* left_;
	expression* right_;
};

class number: public expression
{
public:
	number(double value)
		: value_(value)
	{}

	double evaluate() override
	{
		return value_;
	}

	void print() override
	{
		std::cout << value_;
	}

private:
	double value_;
};

class addition: public binary_operation
{
public:
	addition(expression* left, expression* right)
		: binary_operation(left, right)
	{}

	double evaluate() override
	{
		return left_->evaluate() + right_->evaluate();
	}

	void print_sign() override
	{
		std::cout << "+";
	}
};

class substraction: public binary_operation
{
public:
	substraction(expression* left, expression* right)
		: binary_operation(left, right)
	{}

	double evaluate() override
	{
		return left_->evaluate() - right_->evaluate();
	}

	void print_sign() override
	{
		std::cout << "-";
	}
};

class unary_operation: public expression
{
public:
	unary_operation(expression* expr)
		: expression_(expr)
	{}

	void print() override
	{
		print_operation();

		std::cout << "(";
		expression_->print();
		std::cout << ")";
	}

	virtual void print_operation() = 0;

protected:
	expression* expression_;
};

class sqrt_t: public unary_operation
{
public:
	sqrt_t(expression* expr)
		: unary_operation(expr)
	{}

	double evaluate() override
	{
		return sqrt(expression_->evaluate());
	}

	void print_operation() override
	{
		std::cout << "sqrt";
	}
};

class abs_t: public unary_operation
{
public:
	abs_t(expression* expr)
		: unary_operation(expr)
	{}

	double evaluate() override
	{
		return abs(expression_->evaluate());
	}

	void print_operation() override
	{
		std::cout << "abs";
	}
};

int main()
{
	expression* result = new addition(new number(5), new substraction(new number(3), new number(6)));
	result->println();

	result = new sqrt_t(new number(5));
	std::cout << result->evaluate() << std::endl;

	return 0;
}