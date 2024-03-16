// PPM_parser_project.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "PPM_parser.h"

int main(int argc, char* argv[])
{
	PPM_parser image("pole.ppm");
	//image.grayscale_image_histogram_equalization();
	//image.grayscale_image_gamma_correction(0.9);
	//image.display_pgm_file();
	//image.display_ppm_file();
	image.grayscale_image_filtering();
	image.save_image_in_grayscale("pole.pgm");
	//image.save_ppm_file();

	/*
	if (argc == 2 && strcmp(argv[1], "?") == 0)
	{
		std::cout
			<< "After aplication's name there should be given "
			<< "processed image file name with ppm extension name and "
			<< "name of file in which converted image will be saved.\n";
		return 0;
	}
	if (argc != 3)
	{
		std::cout << "Invalid number of parameters.\n";
		return 1;
	}
	PPM_parser image(argv[1]);
	//image.display_ppm_file();
	//image.grayscale_image_histogram_equalization();
	//image.grayscale_image_gamma_correction();
	image.grayscale_image_filtering();
	image.save_image_in_grayscale(argv[2]);
	*/
	
	return 0;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
