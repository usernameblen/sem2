#include<iostream>
#include<fstream>
#define SIZE 256


class encoder{
	unsigned char* key_obg;
	size_t key_size_obg;

	void swap(unsigned char& a, unsigned char& b);
	void KSA(unsigned char* s);
	void PRGA(unsigned char* s, unsigned char* data, size_t data_size);

public:
	encoder(unsigned char const* key, size_t size);
	~encoder();
	void mutator(unsigned char const* new_key_mut, size_t key_mut);
	void encode(char const* input_file, char const* output_file, bool mode);
};


encoder::encoder(unsigned char const* key, size_t size) {
	if (size == 0) {
		throw std::invalid_argument("Size of key cannot be zero("); //------думай
	}
	if (key == nullptr) {
		throw std::invalid_argument("The key is not okey.. its nullptr"); //----- короче try catch тралялял
	}
	//копирование в поля класса
	key_size_obg = size;
	key_obg = new unsigned char[size];//----тут память выделили
	memcpy(key_obg, key, key_size_obg);
}

//изменение полей класса(ключа и размера)
void encoder::mutator(unsigned char const* key_mut, size_t size_mut) {
	if (size_mut == 0) {
		throw std::invalid_argument("Size of key cannot be zero(\nThe key has not changed"); //------думай
	}
	if (key_mut == nullptr) {
		throw std::invalid_argument("The key is not okey.. its nullptr\nThe key has not changed"); //----- короче try catch тралялял
	}

	delete[] key_obg;
	key_size_obg = size_mut;
	key_obg = new unsigned char[size_mut];
	memcpy(key_obg, key_mut, key_size_obg);
}

void encoder::encode(char const* input_file, char const* output_file, bool mode) {
	std::ifstream inFile(input_file, std::ios::binary);
	std::ofstream outFile(output_file, std::ios::binary);

	if (!(inFile.is_open() && outFile.is_open())) {
		throw std::runtime_error("File is not open");  //-----делай
	}

	inFile.seekg(0, std::ios::end);
	size_t file_size = inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	unsigned char* data = new unsigned char[file_size];
	inFile.read(reinterpret_cast<char*>(data), file_size);


	//инициализация s-блока
	unsigned char s[256];
	KSA(s);

	//   шифр/дешифр
	PRGA(s, data, file_size);

	//рез в выходной файл
	outFile.write(reinterpret_cast<char*>(data), file_size);

	delete[] data; //освобождаем память
}

encoder::~encoder() {
	delete[] key_obg;//-----тут удалили
}


void encoder::swap(unsigned char& a, unsigned char& b) {
	unsigned char temp = a;
	a = b;
	b = temp;
}

void encoder::KSA(unsigned char* s) {
	if (s == nullptr) {
		throw std::invalid_argument("The S-block little bit wrong(little bit nullptr)"); //-----делай
	}
	for (int i = 0; i < 256; ++i) {
		s[i] = i; //заполняем с нуля до 255
	}

	int j = 0;
	for (int i = 0; i < 256; ++i) {
		j = (j + s[i] + key_obg[i % key_size_obg]) % 256;
		swap(s[i], s[j]);
	}
}

void encoder::PRGA(unsigned char* s, unsigned char* data, size_t data_size) { //Pseudo-Random Generation Algorithm
	int i = 0, j = 0;
	for (size_t n = 0; n < data_size; ++n) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		swap(s[i], s[j]);
		data[n] ^= s[(s[i] + s[j]) % 256];
	}
}


int main() {
	unsigned char key[] = "karlson";

	try {
		encoder codi(key, sizeof(key) / sizeof(key[0]));
		codi.encode("docs.txt", "docs_res.txt", 1);
		codi.mutator(key, sizeof(key) / sizeof(key[0]));
		codi.encode("docs_res.txt", "docs_resres.txt", 0);

	}
	catch (const std::exception& e) {
		std::cout << "You have error: " << e.what() << std::endl;
	}
	return 0;
}
