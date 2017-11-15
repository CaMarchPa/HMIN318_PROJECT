#include "methods.h"

// float min_f_(float a, float b) { return (a < b) ? a : b;}
// float max_f_(float a, float b) { return (a > b) ? a : b;}
/**
 *
 */
int main(int argc, char **argv) {
	int threshold_method = 0;
	int neighborhood = 3;

	char resume = '\n';
	if (argc != 2) {
		std::cout << "USAGE : (UNIX) ./project image_name.hdr" << std::endl;
		std::cout << "        (WINDOWS) project.xe image_name.hdr)" << '\n';
		return 1;
	}

	float voxel_size[3];
	CImg<> img_read;
	CImg<> img_tmp;

	// img_read.load_analyze(argv[1], voxel_size);
	// bool redraw = false;
	// int slice = 0;
	// int width = img_read.width();
	// int height = img_read.height();
	// int depth = img_read.depth();
	// float min = 55550.0, max = -55550.0;
	// for (int j = 0; j < height; j++) {
	// 	for (int i = 0; i < width; i++) {
	// 		for (int k = 0; k < depth; k++) {
	// 			min = min_f_(min, img_read(i, j, k));
	// 			max = max_f_(max, img_read(i, j, k));
	// 		}
	// 	}
	// }
	// min = -1024, max = 3071
	std::cout << "min = " << min << ", max = " << max << '\n';
	CImgDisplay img_disp(img_read, "Original image");
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
		while (std::cin.fail()) {
			std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> threshold_method;
		}

		std::cout << std::endl;

		if (threshold_method != 5) {
			std::cout << "\t             +-----------------------------------+" << std::endl;
			std::cout << "\t      -------| Choisissez la taille du voisinage |-------" << std::endl;
			std::cout << "\t      +------                                     -------+" << std::endl;
			std::cout << "\t      |           Exemple : 3 ==> un bloc 3x3x3          |" << std::endl;
			std::cout << "\t      +--------------------------------------------------+" << std::endl;
			std::cout << "\t                   NEIGHBORHOOD SIZE : " << '\t';
			std::cin >> neighborhood;
			while (std::cin.fail()) {
				std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> neighborhood;
			}
		}

		if ((neighborhood > height / 2) || (neighborhood > width / 2) || (neighborhood > depth / 2) || (neighborhood < 0)) {
			neighborhood = (height < width) ? height : width;
			neighborhood = (neighborhood < depth) ? (neighborhood / 2) : depth / 2;
		}
		int x = - 1, y = - 1, z = 0;
		float intensity;
		switch(threshold_method)
		{
			case 1:
					std::cout << "\t\t   SEUIL À PARTIR DE LA MOYENNE DU BLOC " << std::endl;
					img_tmp = get_segmented_image_by_avg(img_read, neighborhood);
					break;

			case 2:
					std::cout << "\t\t   SEUIL À PARTIR DE LA VALEUR MÉDIANE " << std::endl;
					img_tmp = get_segmented_image_by_median(img_read, neighborhood);
					break;

			case 3:
					std::cout << "\t\t   SEUIL À PARTIR D'UNE MOYENNE HARMONIQUE " << std::endl;
					img_tmp = get_segmented_image_by_harmonic(img_read, neighborhood);
					break;

			case 4:
					std::cout << "\t\t   SEUIL À PARTIR DE LA MÉTHODE D'OTSU " << std::endl;
					img_tmp = get_segmented_image_by_otsu(img_read, neighborhood);
					break;

			case 5:
					std::cout << "\t\t   SEUIL FIXÉ PAR L'UTILISATEUR " << std::endl;
					std::cout << "\n\t  * * * * Utilisez la molette de la souris et cliquer sur l'image pour fixer un seuil * * * * " << std::endl;
					while (x == -1) {
						if (img_disp.wheel()) {
							const int scroll = img_disp.wheel();
							slice += scroll;
				            if (slice < 0) {
				                slice = 0;
				            } else {
				                if (slice >= (int)depth) {
				                    slice = (int)depth - 1;
				                }
				            }
				            img_disp.set_wheel();
				            redraw = true;
						}

						if (redraw) {
							CImg<> current_img = img_read.get_slice(slice);
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
					img_tmp = get_segmented_image_by_CC(img_read, intensity, 20);
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

		std::cin.clear();
		std::cout << "\n\t  Appuyez sur ENTRER pour revenir au menu ou n'importe quel touche+ENTRER pour quitter : \t ";
		std::cin.ignore();
		std::cin.get(resume);
		if (resume == '\n')
			std::cout << "\033[2J\033[1;1H"; //clear the console
	} while (resume == '\n');

	std::cout << "\t\t    ----------- PROGRAMME TERMINÉ -------------- " << std::endl;
	
	return 0;
}
