#include<iostream>

class logical_values_array {
private:
	unsigned int _value;

public:
	unsigned int accessor() const {
		return _value;
	}

	logical_values_array(unsigned int value = 0) : _value(value){}

	static bool equals(logical_values_array& one, logical_values_array& two) {
		return one._value == two._value;
	}

	int get_bit(size_t p) const {
		if (p >= sizeof(unsigned int) * 8) {
			throw std::invalid_argument("Change the bit number!"); //-----------u know
		}

		return (_value >> p) & 1;
	}

	int operator[] (int p) const {
		return get_bit(p);
	}

	void get_bits(char* str) const {
		if (str == nullptr) {
			throw std::invalid_argument("The pointer is null( unluck"); //----u know
		}

		unsigned int temp = _value;
		size_t size = 8 * sizeof(unsigned int);
		for (int i = 0; i < size; i++) {
			str[size - i - 1] = (temp & 1) + '0';
			temp = temp >> 1;
		}
		str[size] = '\0';
	}


	//standard logical operations
	logical_values_array inversion() const { //not
		return logical_values_array{ ~_value };
	}

	logical_values_array conjunction(logical_values_array const& smt) const { //and
		return logical_values_array{ _value & smt._value };
	}

	logical_values_array disjunction(logical_values_array const& smt) const { //or
		return logical_values_array(_value | smt._value);
	}

	logical_values_array implication(logical_values_array const& smt) const {
		return logical_values_array{ ~_value | smt._value };
	}

	logical_values_array coimplication(logical_values_array const& smt) const {
		return logical_values_array{ _value | ~smt._value };
	}

	logical_values_array xor_(logical_values_array const& smt) const {
		return logical_values_array{ _value ^ smt._value };
	}

	logical_values_array nxor_(logical_values_array const& smt) const {
		return logical_values_array{ ~(_value ^ smt._value) };
	}

	logical_values_array pier_arrow(logical_values_array const &smt) const{
		return logical_values_array{ ~(_value | smt._value) };
	}

	logical_values_array schaeffer(logical_values_array const& smt) const {
		return logical_values_array{ ~(_value & smt._value) };
	}
};


int main() {

	logical_values_array one; //it will be zero
	std::cout << one.accessor() << std::endl;


	logical_values_array a(5);
	logical_values_array b(3);
	logical_values_array c(6);
	logical_values_array d(5);

	logical_values_array result = (a.implication(b)).conjunction(c);
	std::cout << result.accessor() << std::endl; //2

	std::cout << "Result of equals(a, d): " << logical_values_array::equals(a, d) << std::endl; //1

	try {
		std::cout << "Result of b[1]: " << b[1] << std::endl; //1

		char* str = new char[sizeof(unsigned int) * 8 + 1];
		c.get_bits(str);
		std::cout << "Result of c.get_bits(): " << str << std::endl; //00...00110
		delete[] str;
	}
	catch (const std::exception& e) {
		std::cout << "Ooops, you have error: " << e.what() << std::endl;
	}


	return 0;
}
