#pragma once
#include <iostream>

/*
	Dodanie potrzebnych funkcjonalnoœci do klasy:
		- dodaæ zapis binarnych plików P4

*/

class PPM_parser
{
public:
	PPM_parser(std::string file_name);
	~PPM_parser();

	void display_ppm_file() const;
	void display_pgm_file() const;
	void save_ppm_file();
	bool save_image_in_grayscale(std::string file_name) const;
	bool grayscale_image_histogram_equalization();
	void grayscale_image_gamma_correction(const double& gamma = 0.5);
	void grayscale_image_filtering();

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
	// zmieniæ wartoœæ zwracan¹ na bool
	void pgm_image_allocate();
	void grayscale_avaraging_method() const;
	void grayscale_weighted_method() const;
	PGM_file grayscale_image_file_;
	
	bool memory_allocated_ = false;
	std::string ppm_file_name_;

	// histogram equalization methodes

};