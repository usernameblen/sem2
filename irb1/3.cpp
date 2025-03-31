#include<iostream>
#include<cmath>

class complex {
private:
	double valid;
	double imaginary;

public:
	complex(double vld = 0.0, double mgn = 0.0) : valid(vld), imaginary(mgn) {}

	//+
	complex operator+(const complex& other) {
		return complex(valid + other.valid, imaginary + other.imaginary);
	}
	//-
	complex operator-(const complex& other) {
		return complex(valid - other.valid, imaginary - other.imaginary);
	}
	//*   (1 - i)(3 + 6i) = 3 + 6i - i3 - (-6)
	complex operator*(complex const& other) { 
		double _valid = (valid * other.valid) - (imaginary * other.imaginary);
		double _imaginary = (valid * other.imaginary) + (imaginary * other.valid);
		return complex(_valid, _imaginary);
	}
	//del
	complex operator/(complex const& other) {
		//знаменатель
		complex other2(other.valid, other.imaginary * -1);
		complex res = other2 * other;
		//числитель
		complex copy(this->valid, this->imaginary);
		complex up = other2 * copy;
		if (((res.valid == 0.0) && (res.imaginary == 0.0)) || ((up.valid == 0.0) && (up.imaginary = 0.0))) {
			throw std::overflow_error("Division error");//--------
		}

		double res_valid = (this->valid * other.valid + this->imaginary * other.imaginary) /
			(other.valid * other.valid + other.imaginary * other.imaginary);

		double res_imaginary = (other.valid * this->imaginary - this->valid * other.imaginary) /
			(other.valid * other.valid + other.imaginary * other.imaginary);

		return complex(res_valid, res_imaginary);
	}

	complex& operator +=(complex const& other) {
		this->valid = valid + other.valid;
		this->imaginary = imaginary + other.imaginary;
		return *this;
	}

	complex& operator -=(complex const& other) {
		this->valid = valid - other.valid;
		this->imaginary = imaginary - other.imaginary;
		return *this;
	}

	complex& operator *=(complex const& other) {
		complex this_copy(this->valid, this->imaginary);

		complex res = this_copy * other;
		this->valid = res.valid;
		this->imaginary = res.imaginary;
		return *this;
	}

	complex& operator /=(complex const& other) {
		complex this_copy(this->valid, this->imaginary);

		complex res = this_copy / other;
		this->valid = res.valid;
		this->imaginary = res.imaginary;
		return *this;
	}


	double abs() const {
		return std::sqrt(valid * valid + imaginary * imaginary);
	}

	double arg() {
		if ((valid == 0.0) && (imaginary == 0.0)) {
			throw std::runtime_error("Argument is undefined"); 
		}

		return atan2(imaginary, valid);
	}

	friend std::ostream& operator <<(std::ostream& os, complex const& num) {
		return os << num.valid << (num.imaginary < 0 ? "" : "+") << num.imaginary << 'i';
	}

	friend std::istream& operator >>(std::istream &is, complex& num) {
		return is >> num.valid >> num.imaginary;
	}
};


int main() {
	complex a(1.0, 2.0);
	complex b(3.0, 4.0);
	complex c;
	try {
		std::cout << "Result of (1 + 2i)/ ((1 + 2i)(3 + 4i)):" << std::endl;
		a /= a * b; //0.12 - 0.16i
		std::cout << a << std::endl;

		std::cout << "Result of (3 + 4i)/ 0 + 0i:" << std::endl;
		std::cout <<  b / c << std::endl;
	}
	catch (const std::overflow_error& e) {
		std::cout << "Ooops, you have error: " << e.what() << std::endl;
	}

	std::cout << "b.abs() = " << b.abs() << std::endl; //5

	try {
		std::cout << "b.arg() (b = 3 + 4i)= " << b.arg() << std::endl;

		std::cout << "c.arg() (c = 0 + 0i) = ";
		std::cout << c.arg() << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cout << "Ooops, you have error: " << e.what() << std::endl;
	}
	
	complex something;
	std::cout << "Enter the real and imaginary parts of complex number: " << std::endl;
	std::cin >> something;
	std::cout << "The number is: " << something << std::endl;


	return 0;
}
