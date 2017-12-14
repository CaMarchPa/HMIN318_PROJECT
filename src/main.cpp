#include "methods.h"

/**
 *
 */
int main(int argc, char **argv) {
	int threshold_method = 0; 	// allows to choose the computation method of the threshold
	int sliding_mode = 1; 		// the mode of sliding the bloc, either bloc by bloc or voxel by voxel
	int neighborhood = 3; 		// size of neighborhood
	char resume = '\n'; 		// the value that allow user to come back to the menu or quit the program

	// The two variables allow to compute the execution time of program or function
	double start_time = 0;
	double end_time = 0;
	
	if (argc != 2) {
		std::cout << "USAGE : (UNIX) ./project image_name.hdr" << std::endl;
		std::cout << "        (WINDOWS) project.xe image_name.hdr)" << '\n';
		return 1;
	}

	float voxel_size[3];
	CImg<> img_read; 		// the reading image from the console
	CImg<> img_tmp;			// the variable which get the segemented image

	img_read.load_analyze(argv[1], voxel_size);
	
	bool redraw = false;
	int slice = 0;
	int width = img_read.width();	// width of the image
	int height = img_read.height();	// height of the image
	int depth = img_read.depth();	// depth of the image
	
	CImgDisplay img_disp(img_read, "Original image");	// displays the original image

	bool okay = false;		// variable using to validate or not some request given by the user
	do {
		std::cout << "\t             +---------------------------------------+" << std::endl;
		std::cout << "\t      -------| Choisissez le type de calcul du seuil |-------" << std::endl;
		std::cout << "\t      -------| en tapant le chiffre 1, 2, 3, 4 ou 5  |-------" << std::endl;
		std::cout << "\t             +---------------------------------------+" << std::endl;
		std::cout << "\t     +---------------------------------------------------------+" << std::endl;
		std::cout << "\t     |          1/ La moyenne de chaque bloc nxnxn             |" << std::endl;
		std::cout << "\t     |          2/ La médiane de chaque bloc nxnxn             |" << std::endl;
		std::cout << "\t     | 3/ La moyenne harmonique entre la médiane et la moyenne |" << std::endl;
		std::cout << "\t     |                 4/ La méthode d'Otsu                    |" << std::endl;
		std::cout << "\t     |        5/ Le seuil est fixé par l'utilisateur           |" << std::endl;
		std::cout << "\t     +---------------------------------------------------------+" << std::endl;
		std::cout << "\t                   COMPUTE THRESHOLD METHOD : " << '\t';
		std::cin >> threshold_method;
		while (!okay) {
			if (std::cin.fail()) {
				std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> threshold_method;
			} else if (threshold_method > 5 || threshold_method < 1) {
				std::cout << "\t 	/!\\ Vous devez choisir un nombre entre 1 et 5 " << '\t';
				std::cin >> threshold_method;
			} else
				okay = true;
		}

		std::cout << std::endl;

		if (threshold_method != 5) {
			std::cout << "\t+--------------------------------------------------------------------------------+" << std::endl;
			std::cout << "\t| Choisissez comment glisser le bloc : 1 - bloc par bloc ou 2 - voxel par voxel  |" << std::endl;
			std::cout << "\t+--------------------------------------------------------------------------------+" << std::endl;
			std::cout << "\t\t\t\t 		===> " << '\t';
			std::cin >> sliding_mode;
			okay = false;
			while (!okay) {
				if (std::cin.fail()) {
					std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
					std::cin.clear();
					std::cin.ignore(256, '\n');
					std::cin >> sliding_mode;
				} else if (sliding_mode > 2 || sliding_mode < 1) {
					std::cout << "\t		/!\\ Vous devez saisir le nombre 1 ou 2 " << '\t';
					std::cin >> sliding_mode;
				} else
					okay = true;
			}

			std::cout << "\t             +-----------------------------------+" << std::endl;
			std::cout << "\t      -------| Choisissez la taille du voisinage |-------" << std::endl;
			std::cout << "\t      +------                                     -------+" << std::endl;
			std::cout << "\t      |           Exemple : 3 ==> un bloc 3x3x3          |" << std::endl;
			std::cout << "\t      +--------------------------------------------------+" << std::endl;
			std::cout << "\t                   NEIGHBORHOOD SIZE : " << '\t';
			std::cin >> neighborhood;
			okay = false;
			while (!okay) {
				if (std::cin.fail()) {
					std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
					std::cin.clear();
					std::cin.ignore(256, '\n');
					std::cin >> neighborhood;
				} else  if( sliding_mode == 2 && neighborhood % 2 == 0) {
					std::cout << "\t Pour ce mode de glissement de bloc, saisissez un nombre impair : " << '\t';
					std::cin >> neighborhood;
				} else
					okay = true;
			}
		}

		if ((neighborhood > height / 2) || (neighborhood > width / 2) || (neighborhood > depth / 2) || (neighborhood < 0)) {
			neighborhood = (height < width) ? height : width;
			neighborhood = (neighborhood < depth) ? (neighborhood / 2) : depth / 2;
			if (sliding_mode == 2)
				neighborhood -= 1;
		}

		int x = - 1, y = - 1, z = 0;
		float intensity;
		switch(threshold_method)
		{
			case 1:
					std::cout << "\t\t   SEUIL À PARTIR DE LA MOYENNE DU BLOC " << std::endl;
					start_time = clock();
					if (sliding_mode == 1)
						img_tmp = get_segmented_image_by_avg(img_read, neighborhood);
					else
						img_tmp = getSegmentedImageByAvg(img_read, neighborhood);
					end_time = clock();
					std::cout << "\t\t   	Temps d'exécution : " <<  (end_time - start_time) / double (CLOCKS_PER_SEC) << " secondes "<<std::endl;
					break;

			case 2:
					std::cout << "\t\t   SEUIL À PARTIR DE LA VALEUR MÉDIANE " << std::endl;
					start_time = clock();
					if (sliding_mode == 1)
						img_tmp = get_segmented_image_by_median(img_read, neighborhood);
					else
						img_tmp = getSegmentedImageByMedian(img_read, neighborhood);
					end_time = clock();
					std::cout << "\t\t   	Temps d'exécution : " <<  (end_time - start_time) / double (CLOCKS_PER_SEC) << " secondes "<<std::endl;
					break;

			case 3:
					std::cout << "\t\t   SEUIL À PARTIR D'UNE MOYENNE HARMONIQUE " << std::endl;
					start_time = clock();
					if (sliding_mode == 1)
						img_tmp = get_segmented_image_by_harmonic(img_read, neighborhood);
					else
						img_tmp = getSegmentedImageByHarmonic(img_read, neighborhood);
					end_time = clock();
					std::cout << "\t\t   	Temps d'exécution : " <<  (end_time - start_time) / double (CLOCKS_PER_SEC) << " secondes "<<std::endl;
					break;

			case 4:
					std::cout << "\t\t   SEUIL À PARTIR DE LA MÉTHODE D'OTSU " << std::endl;
					start_time = clock();
					if (sliding_mode == 1)
						img_tmp = get_segmented_image_by_otsu(img_read, neighborhood);
					else
						img_tmp = getSegmentedImageByOtsu(img_read, neighborhood);
					end_time = clock();
					std::cout << "\t\t   	Temps d'exécution : " <<  (end_time - start_time) / double (CLOCKS_PER_SEC) << " secondes "<<std::endl;
					break;

			case 5:
					std::cout << "\t\t   SEUIL FIXÉ PAR L'UTILISATEUR " << std::endl;
					std::cout << "\n\t  * * * * Utilisez la molette de la souris et cliquer sur l'image pour fixer un seuil * * * * " << std::endl;
					while (x == -1) {
						int curr_x = 0;
						int curr_y = 0;
						int curr_z = slice;

						if (img_disp.mouse_x() >= 0 && img_disp.mouse_y() >= 0) {
							curr_x = img_disp.mouse_x();
							curr_y = img_disp.mouse_y();
							redraw = true;
						}

						if (img_disp.wheel()) {
							const int scroll = img_disp.wheel();
							slice += scroll;
				            if (slice < 0) {
				                slice = 0;
				            } else {
				                if (slice >= (int) depth) {
				                    slice = (int) depth - 1;
				                }
				            }
							curr_z = slice;
				            img_disp.set_wheel();
				            redraw = true;
						}

						if (redraw) {
							CImg<> current_img = img_read.get_slice(slice);
							float min, max;
							getMinAndMax(img_read, min, max);
							float white[]={(int)max,(int)max,(int)max},black[]={(int)min,(int)min,(int)min};
							char text[100];
							sprintf(text, " img(%d, %d, %d) : %2.f", curr_x, curr_y, curr_z, img_read(curr_x, curr_y, curr_z));
							current_img.draw_text (2, 2, text, white, black, 0.7f, 8);
							img_disp.display(current_img);
						}

						if (img_disp.button()&1) {
							x = img_disp.mouse_x();
							y = img_disp.mouse_y();
							z = slice;
						}
					}
					intensity = img_read(x, y, z);
					std::cout << "intensity value : img (" << x << ", " << y << ", " << z <<") = " << intensity << '\n';
					if (intensity < 50)
						intensity = 100;
					slice = 0;
					redraw = false;
					start_time = clock();
					img_tmp = get_segmented_image_by_CC(img_read, intensity, 20);
					end_time = clock();
					std::cout << "\t\t   	Temps d'exécution : " <<  (end_time - start_time) / double (CLOCKS_PER_SEC) << " secondes "<<std::endl;
					break;

			default:
					std::cout << "\t\t Le calcul du seuil à partir de la moyenne est pris par défaut " << std::endl;
					img_tmp = get_segmented_image_by_avg(img_read, neighborhood);
					break;
		}

		CImgDisplay img_read_disp(img_tmp, "Segmented image");
		std::cout << "\n\t     * * * * Utilisez la molette de la souris pour faire défiler les coupes * * * * " << std::endl;
		while (!img_read_disp.is_keyESC() && !img_read_disp.is_closed()) {
			if(img_read_disp.wheel()) {
	            const int scroll = img_read_disp.wheel();
	            slice += scroll;
	            if (slice < 0) {
	                slice = 0;
	            } else {
	                if (slice >= (int)depth) {
	                    slice = (int)depth - 1;
	                }
	            }
	            img_read_disp.set_wheel();
	            redraw = true;
	        }
			if (redraw) {
	            CImg<> current_img = img_tmp.get_slice(slice);
	            //mpr_img.resize(512, 512);
	            img_read_disp.display(current_img);
	            redraw = false;
			}
			img_read_disp.wait();
		}
		img_read_disp.close();

		char save = 'n';
		std::cout << "\n\tVoulez-vous sauvegarder l'image résultante? o = oui ou n'importe quelle touche pour passer: \t ";
		std::cin >> save;
		if (save == 'o' || save == 'O') {
			// std::string name = "../img/results/segmented_image";
			char name[40] = "../img/results/segmented_image.hdr";
			std::cout << "\t\t" << name << '\n';
			img_tmp.save_analyze(name);
		}

		std::cin.clear();
		std::cout << "\n\t  Appuyez sur ENTRER pour revenir au menu ou n'importe quel touche+ENTRER pour quitter : \t ";
		std::cin.ignore();
		std::cin.get(resume);
		if (resume == '\n')
			std::cout << "\033[2J\033[1;1H"; //clear the console
	} while (resume == '\n');
	std::cout << "\t\t    ----------- PROGRAMME TERMINÉ ------------- " << std::endl;

	return 0;
}
