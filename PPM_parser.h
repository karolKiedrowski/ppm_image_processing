#pragma once
#include <iostream>

/*
	Dodanie potrzebnych funkcjonalno�ci do klasy:
		- doda� zapis binarnych plik�w P4
		- ulepszy� zapis danych do pliku ppm
*/

class PPM_parser
{
public:
	PPM_parser(std::string file_name);
	~PPM_parser();

	void display_ppm_file() const;
	void display_pgm_file() const;
	void save_ppm_file() const;
	bool save_image_in_grayscale(const std::string& file_name) const;
	bool grayscale_image_histogram_equalization();
	void grayscale_image_gamma_correction(const double& gamma = 0.5);
	void grayscale_image_filtering();
	void grayscale_image_thresholding(const double& threshold = 0.5);

private:
	struct Pixel
	{
		unsigned int red{};
		unsigned int green{};
		unsigned int blue{};
	};
	struct PPM_file
	{
		std::string header{};
		unsigned int rows{};
		unsigned int cols{};
		unsigned int color_inf{};

		Pixel** pixels = nullptr;
	};
	bool ppm_text_image_allocate(const std::string& file_name);
	bool ppm_binary_image_allocate(const std::string& file_name);
	PPM_file image_file_;

	struct PGM_file
	{
		std::string header{};
		unsigned int rows{};
		unsigned int cols{};
		unsigned int color_inf{};

		unsigned int** pixels = nullptr;
	};
	// methodes for grayscale image allocation
	// zmieni� warto�� zwracan� na bool
	void pgm_image_allocate();
	bool save_text_pgm(const std::string& file_name) const;
	bool save_binary_pgm(const std::string& file_name) const;
	void grayscale_avaraging_method() const;
	void grayscale_weighted_method() const;
	PGM_file grayscale_image_file_;
	
	bool memory_allocated_ = false;
	std::string ppm_file_name_;

	// histogram equalization methodes

};