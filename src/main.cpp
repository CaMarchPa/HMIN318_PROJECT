#include "methods.h"

/**
 *
 */
int main(int argc, char **argv) {
	int threshold_method = 0;
	int neighborhood = 3;

	char resume = 'n';
	if (argc != 2) {
		std::cout << "USAGE : (UNIX) ./project image_name.hdr" << std::endl;
		std::cout << "        (WINDOWS) project.xe image_name.hdr)" << '\n';
		return 1;
	}

	float voxel_size[3];
	CImg<> img_read;
	CImg<> img_tmp;

	img_read.load_analyze(argv[1], voxel_size);
	bool redraw = false;
	int slice = 0;
	int width = img_read.width();
	int height = img_read.height();
	int depth = img_read.depth();

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
		std::cout << std::endl;

		std::cout << "\t             +-----------------------------------+" << std::endl;
		std::cout << "\t      -------| Choisissez la taille du voisinage |-------" << std::endl;
		// std::cout << "\t             +-----------------------------------+" << std::endl;
		std::cout << "\t      +------                                     -------+" << std::endl;
		std::cout << "\t      |           Exemple : 3 ==> un bloc 3x3x3          |" << std::endl;
		std::cout << "\t      +--------------------------------------------------+" << std::endl;
		std::cout << "\t                   NEIGHBORHOOD SIZE : " << '\t';
		std::cin >> neighborhood;
		if ((neighborhood > height / 2) || (neighborhood > width / 2) || (neighborhood > depth / 2) || (neighborhood < 0)) {
			neighborhood = (height < width) ? height : width;
			neighborhood = (neighborhood < depth) ? (neighborhood / 2) : depth / 2;
		}
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
			case 5:
					std::cout << "\t\t   SEUIL FIXÉ PAR L'UTILISATEUR " << std::endl;
					std::cout << "Segmentation by searching the connected components of intensities close to the intensity chosen by the user." << std::endl;
					std::cout << "\t\t\t\t NOT YET IMPLEMENTED (coming soooooooon)" << std::endl;
					break;

			default:
					std::cout << "\t\t Le calcul du seuil à partir de la moyenne est pris par défaut " << std::endl;
					img_tmp = get_segmented_image_by_avg(img_read, neighborhood);
					break;
		}

		CImgDisplay img_read_disp(img_tmp, "Segmented image");
		std::cout << "\t\t   Revenir au MENU en tapant sur la touche ESPACE" << std::endl;
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

		std::cout << "\n\t  Voulez vous ressayer une autre méthode de calcul du seuil [o/n] ? : \t ";
		std::cin >> resume;
		if(resume != 'o')
		{
			std::cout << "\t\t    ----------- PROGRAMME TERMINÉ -------------- " << std::endl;
			return 0;
		}
		img_read_disp.display(img_read);
	} while (resume == 'o');

	return 0;
}
