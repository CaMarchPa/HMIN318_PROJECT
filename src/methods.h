#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include "CImg.h"
#include <vector>
#include <ctime>


using namespace cimg_library;

//
extern const int NEIGHBOORS[26][3] = {
    {0, 0, -1}, {0, 0, 1}, {0, -1, 0}, {0, 1, 0}, {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1}, {1, -1, 1},
    {-1, 0, 0}, {1, 0, 0}, {-1, -1, 0}, {1, -1, 0}, {-1, 1, 0}, {1, 1, 0}, {-1, 0, -1}, {-1, 0, 1}, {1, 1, -1},
    {1, 0, -1}, {1, 0, 1}, {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1}, {1, -1, -1}, {1, 1, 1}
};

/* DECLARATION OF METHODS */

/* * * * * * * * * * * BY SLIDING BLOC BY BLOC * * * * * * * * */
CImg<> get_segmented_image_by_avg(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_median(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_harmonic(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_otsu(CImg<>img, int neighborhood);

/* * * * * * * * BY SLIDING THE BLOC, VOXEL BY VOXEL * * * * * * * */
CImg<> getSegmentedImageByAvg(CImg<> img, int neighborhood);
CImg<> getSegmentedImageByMedian(CImg<> img, int neighborhood);
CImg<> getSegmentedImageByHarmonic(CImg<> img, int neighborhood);
CImg<> getSegmentedImageByOtsu(CImg<>img, int neighborhood);

/* * * * * * * * SEGMENTATION FROM THRESHOLD CHOSEN BY THE USER * * * * * * * */
CImg<> get_segmented_image_by_CC(CImg<> img, float intensity, float epsilon);

/* SOME UTILS FUNCTIONS */
std::vector<float> getVoxelsOfBloc(CImg<> img, int width, int height, int depth, int i, int j, int k, int neighborhood);
float get_average(std::vector<float> voxels_of_bloc);
float get_median(std::vector<float> voxels_of_bloc);
float get_harmonic_avg(std::vector<float> voxels_of_bloc);
float get_threshold_by_otsu(std::vector<float> voxels_of_bloc, float min, float max);
std::vector<int> get_histogram(std::vector<float> voxels_of_bloc, float min, float max);
bool contains_bone(std::vector<float> voxels_of_bloc, float bone_intensity);
CImg<> get_extended_image(CImg<> img);
void getMinAndMax(const CImg<> img, float& min, float& max);

#endif // METHODS_H

/* * * * * * * * * * * BY SLIDING BLOC BY BLOC * * * * * * * * */
/**
 * computes a binary image 3D from the given image in argument
 * by sliding a cube of size NxNxN (where N = neighborhood) on the image
 * and taking as threshold for bloc the average of all the voxel
 * intesities belonging to the bloc.
 *
 * @return segmented image
 */
CImg<> get_segmented_image_by_avg(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(img);
    for (int j = 0; j < height - neighborhood ; j+=neighborhood) {
        for (int i = 0; i < width - neighborhood; i+=neighborhood) {
            for (int k = 0; k < depth - neighborhood; k+=neighborhood) {
                //Reduce the complexity
                int bloc_height = j + neighborhood;
                int bloc_width = i + neighborhood;
                int bloc_depth = k + neighborhood;

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j; y < bloc_height; y++) {
                    for (int x = i; x < bloc_width; x++) {
                        for (int z = k; z < bloc_depth; z++) {
                            voxels_of_bloc.push_back( img(x, y, z) );
                        }
                    }
                }

                //Set the voxels of the bloc from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_average(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? max : min;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = min;
                            }
                        }
                    }
                }

            }
        }
    }

    return img_result;
}

/**
 * computes a binary image 3D from the given image in argument
 * by sliding a cube of size NxNxN (where N = neighborhood) on the image
 * and taking as threshold for bloc the median  of all the voxel
 * intensities belonging to the bloc.
 *
 * @return segmented image
 */
CImg<> get_segmented_image_by_median(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(img);

    for (int j = 0; j < height - neighborhood ; j+=neighborhood) {
        for (int i = 0; i < width - neighborhood; i+=neighborhood) {
            for (int k = 0; k < depth - neighborhood; k+=neighborhood) {
                //Reduce the complexity
                int bloc_height = j + neighborhood;
                int bloc_width = i + neighborhood;
                int bloc_depth = k + neighborhood;

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j; y < bloc_height; y++) {
                    for (int x = i; x < bloc_width; x++) {
                        for (int z = k; z < bloc_depth; z++) {
                            voxels_of_bloc.push_back( img(x, y, z) );
                        }
                    }
                }

                //Set the voxels of the bloc from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_median(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? max : min;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = min;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

/**
 * computes a binary image 3D from the given image in argument
 * by sliding a cube of size NxNxN (where N = neighborhood) on the image
 * and taking as threshold for bloc the harmonic average  of all the voxel
 * intensities belonging to the bloc.
 *
 * @return segmented image
 */
CImg<> get_segmented_image_by_harmonic(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(img);

    for (int j = 0; j < height - neighborhood ; j+=neighborhood) {
        for (int i = 0; i < width - neighborhood; i+=neighborhood) {
            for (int k = 0; k < depth - neighborhood; k+=neighborhood) {
                //Reduce the complexity
                int bloc_height = j + neighborhood;
                int bloc_width = i + neighborhood;
                int bloc_depth = k + neighborhood;

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j; y < bloc_height; y++) {
                    for (int x = i; x < bloc_width; x++) {
                        for (int z = k; z < bloc_depth; z++) {
                            voxels_of_bloc.push_back( img(x, y, z) );
                        }
                    }
                }

                //Set the voxels of the bloc from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_harmonic_avg(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? max : min;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = min;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

/**
 * computes a binary image 3D from the given image in argument
 * by sliding a cube of size NxNxN (where N = neighborhood) on the image
 * and taking as threshold the threshold got by appliying otsu's
 * algorithm on the voxel intensities belonging to the bloc.
 *
 * @return segmented image
 */
CImg<> get_segmented_image_by_otsu(CImg<>img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(img);

    for (int j = 0; j < height - neighborhood ; j+=neighborhood) {
        for (int i = 0; i < width - neighborhood; i+=neighborhood) {
            for (int k = 0; k < depth - neighborhood; k+=neighborhood) {
                //Reduce the complexity
                int bloc_height = j + neighborhood;
                int bloc_width = i + neighborhood;
                int bloc_depth = k + neighborhood;

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j; y < bloc_height; y++) {
                    for (int x = i; x < bloc_width; x++) {
                        for (int z = k; z < bloc_depth; z++) {
                            voxels_of_bloc.push_back( img(x, y, z) );
                        }
                    }
                }

                //Set the voxels of the bloc from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_threshold_by_otsu(voxels_of_bloc, min, max);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? max : min;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = min;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

/* * * * * * * BY SLIDING THE BLOC, VOXEL BY VOXEL * * * * * */
/**/
CImg<> getSegmentedImageByAvg(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(width, height, depth, 1, min);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j - neighborhood; y <= j + neighborhood; y++) {
                    if (y >= 0 && y < height)
                        for (int x = i - neighborhood; x <= i + neighborhood; x++) {
                            if (x >= 0 && x < width)
                                for (int z = k - neighborhood; z <= k + neighborhood; z++) {
                                    if (z >= 0 && z < depth)
                                        voxels_of_bloc.push_back(img(x, y, z));
                                }

                        }

                }
                // voxels_of_bloc = getVoxelsOfBloc(img, width, height, depth, i, j, k, neighborhood);

                //Set the voxel(i, j, k) from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_average(voxels_of_bloc);
                    float voxel = ( img(i, j, k) > threshold ) ? max : min;
                    img_result(i, j, k) = voxel;
                }
            }
        }
    }

    return img_result;
}

/**/
CImg<> getSegmentedImageByMedian(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(width, height, depth, 1, min);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j - neighborhood; y <= j + neighborhood; y++) {
                    if (y >= 0 && y < height)
                        for (int x = i - neighborhood; x <= i + neighborhood; x++) {
                            if (x >= 0 && x < width)
                                for (int z = k - neighborhood; z <= k + neighborhood; z++) {
                                    if (z >= 0 && z < depth)
                                        voxels_of_bloc.push_back(img(x, y, z));
                                }

                        }

                }
                // voxels_of_bloc = getVoxelsOfBloc(img, width, height, depth, i, j, k, neighborhood);

                //Set the voxel(i, j, k) from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_median(voxels_of_bloc);
                    float voxel = ( img(i, j, k) > threshold ) ? max : min;
                    img_result(i, j, k) = voxel;
                }
            }
        }
    }

    return img_result;
}

/**/
CImg<> getSegmentedImageByHarmonic(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(width, height, depth, 1, min);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                voxels_of_bloc = getVoxelsOfBloc(img, width, height, depth, i, j, k, neighborhood);

                //Set the voxel(i, j, k) from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_harmonic_avg(voxels_of_bloc);
                    float voxel = ( img(i, j, k) > threshold ) ? max : min;
                    img_result(i, j, k) = voxel;
                }
            }
        }
    }

    return img_result;
}

/**/
CImg<> getSegmentedImageByOtsu(CImg<>img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

    float min, max;
    float bone_intensity = 100;
    getMinAndMax(img, min, max);
    if (min < 0 && max > 255)
        bone_intensity = 300;

    CImg<> img_result(width, height, depth, 1, min);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {

                //Get the voxels of the bloc
                std::vector<float> voxels_of_bloc;
                for (int y = j - neighborhood; y <= j + neighborhood; y++) {
                    if (y >= 0 && y < height)
                        for (int x = i - neighborhood; x <= i + neighborhood; x++) {
                            if (x >= 0 && x < width)
                                for (int z = k - neighborhood; z <= k + neighborhood; z++) {
                                    if (z >= 0 && z < depth)
                                        voxels_of_bloc.push_back(img(x, y, z));
                                }

                        }

                }
                // voxels_of_bloc = getVoxelsOfBloc(img, width, height, depth, i, j, k, neighborhood);

                //Set the voxel(i, j, k) from threshold
                if (contains_bone(voxels_of_bloc, bone_intensity)) {
                    float threshold = get_threshold_by_otsu(voxels_of_bloc, min, max);
                    float voxel = ( img(i, j, k) > threshold ) ? max : min;
                    img_result(i, j, k) = voxel;
                }
            }
        }
    }

    return img_result;
}

/**
 *
 */
CImg<> get_segmented_image_by_CC(CImg<> img, float intensity, float epsilon) {
    CImg<> img_extended = get_extended_image(img);
    CImg<> img_result(img);

    int width_ext = img_extended.width();
    int height_ext = img_extended.height();
    int depth_ext = img_extended.depth();

    for (int j = 1; j < height_ext-1; j++) {
        for (int i = 1; i < width_ext-1; i++) {
            for (int k = 1; k < depth_ext-1; k++) {
                //
                float voxel = img_extended(i, j, k);
                if ( voxel >= intensity ) {
                    img_extended(i, j, k) = -1; //label to not test this voxel's position later
                    std::vector< std::vector<int> > position_list;
                    std::vector<int> tmp(3, 0);
                    tmp[0] = i;
                    tmp[1] = j;
                    tmp[2] = k;
                    position_list.push_back(tmp);

                    bool stop = false;
                    int n = 0;
                    while (stop != true) {
                        for (int counter = 0; counter < 26; counter++) {
                            int x = position_list[n][0] + NEIGHBOORS[counter][0];
                            int y = position_list[n][1] + NEIGHBOORS[counter][1];
                            int z = position_list[n][2] + NEIGHBOORS[counter][2];
                            voxel = img_extended(x, y, z);
                            if ( voxel >= intensity ) {
                                img_extended(x, y, z) = -1.0;
                                tmp[0] = x;
                                tmp[1] = y;
                                tmp[2] = z;
                                position_list.push_back(tmp);
                            }
                        }
                        n++;
                        if (n == position_list.size()) {
                            stop = true;
                        }
                    }

                }
                //
            }
        }
    }

    int height = height_ext - 2;
    int width = width_ext - 2;
    int depth = depth_ext -2;
    // set all the voxels with the label - 1 to 255 and the others to 0
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {
                float voxel = img_extended(i + 1, j + 1, k + 1);
                img_result(i, j, k) = (voxel == -1) ? 255 : 0;
            }
        }
    }

    return img_result;
}

/****/
std::vector<float> getVoxelsOfBloc(CImg<> img, int width, int height, int depth, int i, int j, int k, int neighborhood) {

    std::vector<float> voxels_of_bloc;

    for (int y = j - neighborhood; y <= j + neighborhood; y++) {
        if (y >= 0 && y < height)
            for (int x = i - neighborhood; x <= i + neighborhood; x++) {
                if (x >= 0 && x < width)
                    for (int z = k - neighborhood; z <= k + neighborhood; z++) {
                        if (z >= 0 && z < depth)
                            voxels_of_bloc.push_back(img(x, y, z));
                    }

            }

    }

    return voxels_of_bloc;
}

/**
 * computes and returns the average value of the intensities of the vector of voxel given
 * in argument.
 */
float get_average(std::vector<float> voxels_of_bloc) {
    float avg_value = 0.0;
    int size = voxels_of_bloc.size();
    for (int i = 0; i < size; i++) {
        avg_value += voxels_of_bloc[i];
    }
    
    return avg_value /= size;
}

/**
 * computes and returns the median value of the intensities of the vector of voxel given
 * in argument.
 */
float get_median(std::vector<float> voxels_of_bloc) {
    int size = voxels_of_bloc.size();
    std::sort(voxels_of_bloc.begin(), voxels_of_bloc.end());
    float median_value = voxels_of_bloc[size / 2];
    if (size % 2 == 0) {
        median_value = (voxels_of_bloc[size / 2 ] + voxels_of_bloc[size / 2 - 1]) / 2;
    }
    return median_value;
}

/**
 * computes and returns the value of harmonic average by using the average and median values
 * of the intensities of the vector of voxel given in argument.
 */
float get_harmonic_avg(std::vector<float> voxels_of_bloc) {
    float avg_value = get_average(voxels_of_bloc);
    float median_value = get_median(voxels_of_bloc);
    return (2 * avg_value * median_value) / (avg_value + median_value);
}

/**
 * computes and returns the threshold by using otsu's algorithm
 * on the intensities of the vector of voxel given in argument.
 */
float get_threshold_by_otsu(std::vector<float> voxels_of_bloc, float min, float max) {
    std::vector<int> histogram = get_histogram(voxels_of_bloc, min, max);
    int min_abs = (int) abs (min);
    int max_abs = (int) abs (max);
    int size = min_abs + max_abs;

    float probability[size];
    float mean[size];
    float between[size];
    float max_between = 0.0;
    int threshold = 0;

    for (int i = 0; i < size; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }

    probability[0] = histogram[0];
    for (int i = 1; i < size; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    for (int i = 0; i < size-1; i++) {
        if (probability[i] != 0.0 && probability[i] != 1.0) {
            between[i] = pow(mean[size-1]*probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
        } else
            between[i] = 0.0;
        if (between[i] > max_between) {
            max_between = between[i];
            threshold = i;
        }
    }

    return threshold - min_abs;
}

/**
 * return a vector of the tonal distribution of the given vector of voxels in argument
 */
std::vector<int> get_histogram(std::vector<float> voxels_of_bloc, float min, float max) {
    int min_abs = (int) abs (min);
    int max_abs = (int) abs (max);

    std::vector<int> histogram(min_abs + max_abs);
    int size = voxels_of_bloc.size();

    for (int i = 0; i < size; i++) {
        int int_v = (int) voxels_of_bloc[i];
        histogram[int_v + min_abs] ++;
    }

    return histogram;
}

/**
 * check if the vector of voxels given in argument contains at least one intensity
 * which represent a bone. A bone's intensity must be greater than the bone_intensity
 * value given in argument.
 * @return true if the vector of voxels contains a bone, false else.
 */
bool contains_bone(std::vector<float> voxels_of_bloc, float bone_intensity) {
    int size = voxels_of_bloc.size();

    for (int i = 0; i < size; i++) {
        if ( voxels_of_bloc[i] > bone_intensity) {
            return true;
        }
    }
    return false;
}

/**
 *
 */
CImg<> get_extended_image(CImg<> img) {
    //
    int width = img.width();
    int height = img.height();
    int depth = img.depth();
    //
    int width_ext = width + 2;
    int height_ext = height + 2;
    int depth_ext = depth + 2;

    CImg<> img_result(width_ext, height_ext, depth_ext);

    for (int j = 0; j < height_ext; j++) {
        for (int i = 0; i < width_ext; i++) {
            for (int k = 0; k < depth_ext; k++) {
                if ( ((j == 0) || (j == height_ext - 1)) || ((i == 0) || (i == width_ext - 1)) || ((k == 0) || (k == depth_ext - 1)) ) {
                    img_result(i, j, k) = 0.0;
                } else {
                    img_result(i, j, k) = img(i - 1, j - 1, k - 1);
                }
            }
        }
    }

    return img_result;
}

/**/
void getMinAndMax(const CImg<> img, float& min, float& max) {
    int width = img.width();
    int height = img.height();
    int depth = img.depth();
    min = img(0, 0, 0);
    max = img(0, 0, 0);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {
                float curr_intensity = img (i, j, k);
                min = (min < curr_intensity) ? min : curr_intensity;
                max = (max > curr_intensity) ? max : curr_intensity;
            }
        }
    }
}
