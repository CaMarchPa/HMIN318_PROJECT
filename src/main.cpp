#include "methods.h"

/**
 *
 */
int main(int argc, char const *argv[]) {
	int threshold_method = 0;
	char resume = 'n';

	// float voxel_size[3];
	// CImg<> img_read;
	// img_read.load_analyze(argv[1], voxel_size);

	hello();
	// if (argc != 2) {
	// 	std::cout << "USAGE : (UNIX) ./project image_name.extension" << std::endl;
	// 	std::cout << "        (WINDOWS) project.xe image_name.extension)" << '\n';
	// 	return 1;
	// }
	do {
		std::cout << "\t             +---------------------------------------+" << std::endl;
		std::cout << "\t      -------| Choisissez le type de calcul du seuil |-------" << std::endl;
		std::cout << "\t      -------|  en tapant le chiffre 1, 2, 3 4 ou 5  |-------" << std::endl;
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

	    switch(threshold_method)
	    {
	    	case 1:
					std::cout << "\t\t\t SEUIL À PARTIR DE LA MOYENNE DU BLOC " << std::endl;
					//
					break;

	        case 2:
					std::cout << "\t\t\t SEUIL À PARTIR DE LA VALEUR MÉDIANE " << std::endl;
	                //
	                break;

	        case 3:
					std::cout << "\t\t\t SEUIL À PARTIR D'UNE MOYENNE HARMONIQUE " << std::endl;
	                //
	                break;

			case 4:
					std::cout << "\t\t\t SEUIL À PARTIR DE LA MÉTHODE D'OTSU " << '\n';
					//
	        case 5:
					std::cout << "\t\t\t SEUIL FIXÉ PAR L'UTILISATEUR " << std::endl;
	                //
	                break;

	        default:
					std::cout << "\t\t Le calcul du seil à partir de la moyenne est pris par défaut " << '\n';
					threshold_method = 1;
	                 break;
	    }

		std::cout << "\n\t  Voulez vous ressayer une autre méthode de calcul du seuil [o/n] ? : \t ";
		std::cin >> resume;
		if(resume != 'o')
		{
			std::cout << "\t\t    ----------- PROGRAMME TERMINÉ -------------- " << std::endl;
			return 0;
		}
	} while (resume == 'o');
	return 0;
}
