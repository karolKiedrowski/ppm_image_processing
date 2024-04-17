#include "PPM_parser.h"
#include <fstream>
#include <string>
#include <vector>
//#include <arm_neon.h>

PPM_parser::PPM_parser(std::string file_name)
{
	std::ifstream file;
	file.open(file_name, std::ios::in);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		memory_allocated_ = false;
		image_file_.pixels = nullptr;
		return;
	}
	// saving given file name
	ppm_file_name_ = file_name;
	std::string header;
	file >> header;
	file.close();

	if (header == "P3")
		ppm_text_image_allocate(file_name);
	else if (header == "P6")
		ppm_binary_image_allocate(file_name);
	else
	{
		std::cout << "Wrong header!\nHeader should be P3 or P6!\n";
		return;
	}

	memory_allocated_ = true;
	pgm_image_allocate();
	
}

bool PPM_parser::ppm_text_image_allocate(const std::string& file_name)
{
	std::ifstream file;
	file.open(file_name, std::ios::in);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		memory_allocated_ = false;
		image_file_.pixels = nullptr;
		return false;
	}

	file >> image_file_.header;
	std::string stream;
	file >> stream;
	image_file_.cols = std::stoi(stream);
	file >> stream;
	image_file_.rows = std::stoi(stream);
	file >> stream;
	image_file_.color_inf = std::stoi(stream);

	image_file_.pixels = new Pixel*[image_file_.rows];

	for (int i = 0; i < image_file_.rows; i++)
	{
		image_file_.pixels[i] = new Pixel[image_file_.cols];
	}

	for (int i = 0; i < image_file_.rows; i++)
	{
		for (int j = 0; j < image_file_.cols; j++)
		{
			file >> stream;
			image_file_.pixels[i][j].red = std::stoi(stream);
			file >> stream;
			image_file_.pixels[i][j].green = std::stoi(stream);
			file >> stream;
			image_file_.pixels[i][j].blue = std::stoi(stream);
		}
	}

	if (file.fail())
	{
		std::cout << "File stream operations failed.\n";
		memory_allocated_ = false;
		return false;
	}

	file.close();
	return true;
}

bool PPM_parser::ppm_binary_image_allocate(const std::string& file_name)
{
	
	std::streampos beg_binary;
	// Processing file header as a text file.
	std::ifstream file;
	file.open(file_name, std::ios::in);
	if(!file)
	{
		std::cout << "File opening failed.\n";
		memory_allocated_ = false;
		image_file_.pixels = nullptr;
		return false;
	}

	std::string data;
	file >> data;
	image_file_.header = data;
	file >> data;
	image_file_.cols = std::stoi(data);
	file >> data;
	image_file_.rows = std::stoi(data);
	file >> data;
	image_file_.color_inf = std::stoi(data);

	beg_binary = file.tellg();
	file.close();
	
	image_file_.pixels = new Pixel * [image_file_.rows];

	for (int i = 0; i < image_file_.rows; i++)
	{
		image_file_.pixels[i] = new Pixel[image_file_.cols];
	}

	// Processing the rest of the file as a binary file.
	file.open(file_name, std::ios::in | std::ios::binary);
	if (!file)
	{
		std::cout << "File opening failed.\n";
		memory_allocated_ = false;
		image_file_.pixels = nullptr;
		return false;
	}
	
	file.seekg(beg_binary);

	/*
	struct Pixel_v2 {
		unsigned char r, g, b;
	};

	std::vector<Pixel_v2> pixels;

	pixels.resize(image_file_.rows * image_file_.cols);
	unsigned int size = image_file_.rows * image_file_.cols;
	
	* To dziala
	file.read(reinterpret_cast<char*>(pixels.data()), size * sizeof(Pixel));
	for (unsigned int i = 0; i < image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < image_file_.cols; j++)
		{
			image_file_.pixels[i][j].red = (unsigned int)pixels[(i * image_file_.cols + j)].r;
			image_file_.pixels[i][j].green = (unsigned int)pixels[(i * image_file_.cols + j)].g;
			image_file_.pixels[i][j].blue = (unsigned int)pixels[(i * image_file_.cols + j)].b;
		}
	}
	*/
	
	unsigned char unit{};
	for (int i = 0; i < image_file_.rows; i++)
	{
		for (int j = 0; j < image_file_.cols; j++)
		{
			file.read(reinterpret_cast<char*>(&unit), sizeof(unit));
			image_file_.pixels[i][j].red = (unsigned int)unit;
			file.read(reinterpret_cast<char*>(&unit), sizeof(unit));
			image_file_.pixels[i][j].green = (unsigned int)unit;
			file.read(reinterpret_cast<char*>(&unit), sizeof(unit));
			image_file_.pixels[i][j].blue = (unsigned int)unit;
		}
	}

	file.close();

	return true;
}

PPM_parser::~PPM_parser()
{
	if(image_file_.pixels != nullptr)
	{
		for (int i = 0; i < image_file_.rows; i++)
		{
			delete[] image_file_.pixels[i];
		}
		delete[] image_file_.pixels;
	}

	if (grayscale_image_file_.pixels != nullptr)
	{
		for (int i = 0; i < grayscale_image_file_.rows; i++)
		{
			delete[] grayscale_image_file_.pixels[i];
		}
		delete[] grayscale_image_file_.pixels;
	}
}

void PPM_parser::display_ppm_file() const
{
	if (memory_allocated_ == false)
	{
		std::cout << "Image is not allocated.\n";
		return;
	}
	std::cout 
		<< image_file_.header		<< std::endl
		<< image_file_.cols			<< " "
		<< image_file_.rows			<< std::endl
		<< image_file_.color_inf	<< std::endl;


	for (unsigned int i = 0; i < image_file_.rows; i++)
	{
		for(unsigned int j = 0; j < image_file_.cols; j++)
		{
			std::cout 
				<< image_file_.pixels[i][j].red << " "
				<< image_file_.pixels[i][j].green << " "
				<< image_file_.pixels[i][j].blue << " ";
		}
		std::cout << std::endl;
	}
}

void PPM_parser::display_pgm_file() const
{
	if (memory_allocated_ == false)
	{
		std::cout << "Image is not allocated.\n";
		return;
	}
	std::cout
		<< grayscale_image_file_.header << std::endl
		<< grayscale_image_file_.cols << " "
		<< grayscale_image_file_.rows << std::endl
		<< grayscale_image_file_.color_inf << std::endl;


	for (unsigned int i = 0; i < image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < image_file_.cols; j++)
		{
			std::cout
				<< grayscale_image_file_.pixels[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void PPM_parser::save_ppm_file() const
{
	if (memory_allocated_ == false)
	{
		std::cout << "Image is not allocated.\n";
		return;
	}
	std::string file_name_v2 = "image_v2.ppm";
	std::ofstream file;
	std::streampos position{};
	file.open(file_name_v2, std::ios::out);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		return;
	}
	file << image_file_.header << "\n";
	file << image_file_.cols << " " << image_file_.rows << "\n";
	file << image_file_.color_inf << "\n";
	position = file.tellp();
	file.close();

	
	file.open(file_name_v2, std::ios::out | std::ios::binary | std::ios::app);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		return;
	}
	file.seekp(position);
	char unit{};
	for (unsigned int i = 0; i < image_file_.rows; i+=3)
	{
		for (unsigned int j = 0; j < image_file_.cols; j+=3)
		{
			unit = (unsigned char)image_file_.pixels[i][j].red;
			file.write(reinterpret_cast<char*>(&unit), sizeof(unit));
			unit = (unsigned char)image_file_.pixels[i][j].green;
			file.write(reinterpret_cast<char*>(&unit), sizeof(unit));
			unit = (unsigned char)image_file_.pixels[i][j].blue;
			file.write(reinterpret_cast<char*>(&unit), sizeof(unit));
		}
	}
	

	file.close();
}

bool PPM_parser::save_text_pgm(const std::string& file_name) const
{
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Data is not loaded, no data will be saved.\n";
		return false;
	}

	std::ofstream file;
	file.open(file_name, std::ios::out);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		return false;
	}

	file << grayscale_image_file_.header << "\n";
	file << grayscale_image_file_.cols << " " << grayscale_image_file_.rows << "\n";
	file << grayscale_image_file_.color_inf << "\n";

	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			file << grayscale_image_file_.pixels[i][j] << " ";
		}
		file << "\n";
	}
	if (file.fail())
	{
		std::cout << "File stream operations failed.\n";
		return false;
	}

	file.close();
	return true;
}

bool PPM_parser::save_binary_pgm(const std::string& file_name) const
{
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Data is not loaded, no data will be saved.\n";
		return false;
	}

	std::ofstream file;
	file.open(file_name, std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		return false;
	}

	file << grayscale_image_file_.header << "\n";
	file << grayscale_image_file_.cols << " " << grayscale_image_file_.rows << "\n";
	file << grayscale_image_file_.color_inf << "\n";

	unsigned char unit{};
	for (int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (int j = 0; j < grayscale_image_file_.cols; j++)
		{
			unit = (unsigned char)grayscale_image_file_.pixels[i][j];
			file.write(reinterpret_cast<char*>(&unit), sizeof(unit));
		}
	}

	file.close();
}

bool PPM_parser::save_image_in_grayscale(const std::string& file_name) const
{
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Data is not loaded, no data will be saved.\n";
		return false;
	}

	if (grayscale_image_file_.header == "P2")
		return save_text_pgm(file_name);
	else if (grayscale_image_file_.header == "P5")
		return save_binary_pgm(file_name);
	else
	{
		std::cout << "Wrong header, maybe grayscale image is not allocated in program memory.\n";
		return false;
	}

	/*
	std::ofstream file;
	file.open(file_name, std::ios::out);
	if (!file.is_open())
	{
		std::cout << "File opening failed.\n";
		return false;
	}

	//file << grayscale_image_file_.header << "\n";
	file << "P2" << "\n";
	file << grayscale_image_file_.cols << " " << grayscale_image_file_.rows << "\n";
	file << grayscale_image_file_.color_inf << "\n";

	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			file << grayscale_image_file_.pixels[i][j] << " ";
		}
		file << "\n";
	}
	if (file.fail())
	{
		std::cout << "File stream operations failed.\n";
		return false;
	}

	file.close();
	*/

	return true;
}

void PPM_parser::pgm_image_allocate()
{
	if (image_file_.pixels == nullptr)
	{
		std::cout << "Cannot allocate grayscale image, because image was not allocated.\n";
		return;
	}
	if (image_file_.header == "P3")
		grayscale_image_file_.header = "P2";
	else if (image_file_.header == "P6")
		grayscale_image_file_.header = "P5";
	grayscale_image_file_.rows		= image_file_.rows;
	grayscale_image_file_.cols		= image_file_.cols;
	grayscale_image_file_.color_inf = image_file_.color_inf;
	grayscale_image_file_.pixels	= new unsigned int*[grayscale_image_file_.rows];
	for (int i = 0; i < grayscale_image_file_.rows; i++)
	{
		grayscale_image_file_.pixels[i] = new unsigned int[grayscale_image_file_.cols];
	}
	grayscale_weighted_method();
	// grayscale_avaraging_method();
}

void PPM_parser::grayscale_avaraging_method() const
{
	unsigned int converted_value;
	for (unsigned int i = 0; i < image_file_.rows; i++)
	{
		for (int j = 0; j < image_file_.cols; j++)
		{
			converted_value = (image_file_.pixels[i][j].red + image_file_.pixels[i][j].green + image_file_.pixels[i][j].blue) / 3;
			grayscale_image_file_.pixels[i][j] = converted_value;
		}
	}
}

void PPM_parser::grayscale_weighted_method() const
{
	unsigned int converted_value;
	for (unsigned int i = 0; i < image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < image_file_.cols; j++)
		{
			converted_value = (unsigned int)((0.299 * image_file_.pixels[i][j].red) + (0.587 * image_file_.pixels[i][j].green) + (0.114 * image_file_.pixels[i][j].blue));
			grayscale_image_file_.pixels[i][j] = converted_value;
		}
	}
}

bool PPM_parser::grayscale_image_histogram_equalization()
{
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Grayscale image is not allocated, cannot perform histogram equalization operation.\n";
		return false;
	}

	// mo¿liwe, ¿einformacja o kolorze jest 0-255 w³¹cznie z zerem
	unsigned int size = grayscale_image_file_.color_inf + 1;
	unsigned int* intensity_array = new unsigned int[size]();

	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			intensity_array[grayscale_image_file_.pixels[i][j]]++;
		}
	}

	// szukamy minimalnej liczby wyst¹pieñ, która jest jednak wiêksza od zera
	unsigned int minimum = size + 1;
	for (int i = 0; i < size; i++)
	{
		if (minimum == (size + 1) && intensity_array[i] > 0)
			minimum = i;
	}
	if (minimum == (size + 1))
	{
		std::cout << "All of histogram elements were zeros\n";
		return false;
	}

	unsigned int* cumulative_image_hist = new unsigned int[size]();

	cumulative_image_hist[0] = intensity_array[0];
	for (int i = 1; i < size; i++)
	{
		cumulative_image_hist[i] = cumulative_image_hist[i - 1] + intensity_array[i];
	}

	unsigned int Hmin = cumulative_image_hist[minimum];
	unsigned int* T = new unsigned int[size]();
	double value;

	for (int i = 0; i < size; i++)
	{
		value = (double)((cumulative_image_hist[i] - Hmin) / (double)(image_file_.rows * image_file_.cols - Hmin)) * (size - 1);
		T[i] = round(value);
	}

	unsigned int final_intensity{};

	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			grayscale_image_file_.pixels[i][j] = T[grayscale_image_file_.pixels[i][j]];
		}
	}

	delete[]T;
	delete[]cumulative_image_hist;
 	delete[]intensity_array;
	return true;
}

void PPM_parser::grayscale_image_gamma_correction(const double& gamma)
{
	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			grayscale_image_file_.pixels[i][j] = std::pow(grayscale_image_file_.pixels[i][j], (1 / gamma));
			
			/*
			if (grayscale_image_file_.pixels[i][j] > 256 || grayscale_image_file_.pixels[i][j] < 0)
			{
				std::cout << "New color information is out of range.\n";
				return;
			}
			*/
		}
	}
}

void PPM_parser::grayscale_image_filtering()
{
	/*
		filtr
		1 2 1
		2 4 2
		1 2 1
		i ta maske podzielic na 16
	*/
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Grayscale image uninitialized, cannot perform image filtering.\n";
		return;
	}

	unsigned int** filtered = new unsigned int*[grayscale_image_file_.rows];
	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		filtered[i] = new unsigned int[grayscale_image_file_.cols];
	}

	unsigned int divider = 16;

	// setting edges
	for (unsigned int i = 0; i < grayscale_image_file_.cols; i++)
		filtered[0][i] = grayscale_image_file_.pixels[0][i];

	for (unsigned int i = 0; i < grayscale_image_file_.cols; i++)
		filtered[grayscale_image_file_.rows - 1][i] = grayscale_image_file_.pixels[grayscale_image_file_.rows - 1][i];

	for (unsigned int i = 1; i < (grayscale_image_file_.cols - 1); i++)
		filtered[i][0] = grayscale_image_file_.pixels[i][0];

	for (unsigned int i = 1; i < (grayscale_image_file_.cols - 1); i++)
		filtered[i][grayscale_image_file_.cols - 1] = grayscale_image_file_.pixels[i][grayscale_image_file_.cols - 1];

	// filtering
	for (unsigned int i = 1; i < (grayscale_image_file_.rows - 1); i++)
	{
		for (unsigned int j = 1; j < (grayscale_image_file_.cols); j++)
		{
			filtered[i][j] = (grayscale_image_file_.pixels[i - 1][j - 1] + 2 * grayscale_image_file_.pixels[i - 1][j] + grayscale_image_file_.pixels[i - 1][j + 1]);
			filtered[i][j] += (2 * grayscale_image_file_.pixels[i][j - 1] + 4 * grayscale_image_file_.pixels[i][j] + 2 * grayscale_image_file_.pixels[i][j + 1]);
			filtered[i][j] += (grayscale_image_file_.pixels[i + 1][j - 1] + 2 * grayscale_image_file_.pixels[i + 1][j] + grayscale_image_file_.pixels[i + 1][j + 1]);
			filtered[i][j] /= divider;
		}
	}

	for (int i = 0; i < grayscale_image_file_.rows; i++)
	{
		delete[] grayscale_image_file_.pixels[i];
	}
	delete[] grayscale_image_file_.pixels;

	grayscale_image_file_.pixels = filtered;
}

void PPM_parser::grayscale_image_thresholding(const double& threshold)
{
	if (grayscale_image_file_.pixels == nullptr)
	{
		std::cout << "Grayscale image uninitialized, cannot perform image thresholding.\n";
		return;
	}

	unsigned int size = grayscale_image_file_.color_inf + 1;
	double* gray_level_hist = new double[size]();

	for (unsigned int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (unsigned int j = 0; j < grayscale_image_file_.cols; j++)
		{
			gray_level_hist[grayscale_image_file_.pixels[i][j]]++;
		}
	}

	unsigned int number_of_pixels = grayscale_image_file_.rows * grayscale_image_file_.cols;

	for (int i = 0; i < size; i++)
	{
		gray_level_hist[i] /= number_of_pixels;
		//std::cout << i << " dividing resoult: " << gray_level_hist[i] << "\n";
	}

	double* distributor = new double[size]();
	distributor[0] = gray_level_hist[0];
	for (int i = 1; i < size; i++)
	{
		distributor[i] = distributor[i - 1] + gray_level_hist[i];
		std::cout << i << " distributor: " << distributor[i] << "\n";
	}

	for (int i = 0; i < grayscale_image_file_.rows; i++)
	{
		for (int j = 0; j < grayscale_image_file_.cols; j++)
		{
			if (distributor[grayscale_image_file_.pixels[i][j]] > threshold)
				grayscale_image_file_.pixels[i][j] = 255;
			else
				grayscale_image_file_.pixels[i][j] = 0;
		}
	}

	delete[]distributor;
	delete[]gray_level_hist;
}