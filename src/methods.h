#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include "CImg.h"
#include <vector>

using namespace cimg_library;


void hello();
CImg<> get_segmented_image_by_avg(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_median(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_harmonic(CImg<> img, int neighborhood);
CImg<> get_segmented_image_by_otsu(CImg<>img, int neighborhood);

float get_median(std::vector<float> voxels_of_bloc);
float get_average(std::vector<float> voxels_of_bloc);
float get_harmonical_avg(std::vector<float> voxels_of_bloc);
float get_threshold_by_otsu(std::vector<float> voxels_of_bloc);
std::vector<int> get_histogram(std::vector<float> voxels_of_bloc);
bool contains_bone(std::vector<float> voxels_of_bloc, int bone_intensity);

#endif // METHODS_H

CImg<> get_segmented_image_by_avg(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

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
                if (contains_bone(voxels_of_bloc, 100)) {
                    float threshold = get_average(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? 255.0 : 0.0;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = 0.0;
                            }
                        }
                    }
                }

            }
        }
    }

    return img_result;
}

CImg<> get_segmented_image_by_median(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

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
                if (contains_bone(voxels_of_bloc, 100)) {
                    float threshold = get_median(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? 255.0 : 0.0;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = 0.0;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

CImg<> get_segmented_image_by_harmonic(CImg<> img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

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
                if (contains_bone(voxels_of_bloc, 100)) {
                    float threshold = get_harmonical_avg(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? 255.0 : 0.0;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = 0.0;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

CImg<> get_segmented_image_by_otsu(CImg<>img, int neighborhood) {
    int height = img.height();
    int width = img.width();
    int depth = img.depth();

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
                if (contains_bone(voxels_of_bloc, 100)) {
                    float threshold = get_threshold_by_otsu(voxels_of_bloc);
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                float voxel = ( img(x, y, z) > threshold ) ? 255.0 : 0.0;
                                img_result(x, y, z) = voxel;
                            }
                        }
                    }
                } else {
                    for (int y = j; y < bloc_height; y++) {
                        for (int x = i; x < bloc_width; x++) {
                            for (int z = k; z < bloc_depth; z++) {
                                img_result(x, y, z) = 0.0;
                            }
                        }
                    }
                }
            }
        }
    }

    return img_result;
}

float get_median(std::vector<float> voxels_of_bloc) {
    int size = voxels_of_bloc.size();
    float median_value = voxels_of_bloc[size / 2];
    if (size % 2 == 0) {
        median_value = (voxels_of_bloc[size / 2 ] + voxels_of_bloc[size / 2 - 1]) / 2;
    }
    return median_value;
}

float get_average(std::vector<float> voxels_of_bloc) {
    float avg_value = 0.0;
    int size = voxels_of_bloc.size();
    for (int i = 0; i < size; i++) {
        avg_value += voxels_of_bloc[i];
    }
    return avg_value /= size;
}

float get_harmonical_avg(std::vector<float> voxels_of_bloc) {
    float avg_value = get_average(voxels_of_bloc);
    float median_value = get_median(voxels_of_bloc);
    return (2 * avg_value * median_value) / (avg_value + median_value);
}

float get_threshold_by_otsu(std::vector<float> voxels_of_bloc) {
    std::vector<int> histogram = get_histogram(voxels_of_bloc);
    float probability[256];
    float mean[256];
    float between[256];
    float max_between = 0.0;
    int threshold = 0;

    for (int i = 0; i < 256; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }

    probability[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    for (int i = 0; i < 255; i++) {
        if (probability[i] != 0.0 && probability[i] != 1.0) {
            between[i] = pow(mean[255]*probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
        } else
            between[i] = 0.0;
        if (between[i] > max_between) {
            max_between = between[i];
            threshold = i;
        }
    }

    return threshold;
}

std::vector<int> get_histogram(std::vector<float> voxels_of_bloc) {
    std::vector<int> histogram(256);
    int size = voxels_of_bloc.size();

    for (int i = 0; i < size; i++) {
        int int_v = (int) voxels_of_bloc[i];
        histogram[int_v] ++;
    }
    return histogram;
}

bool contains_bone(std::vector<float> voxels_of_bloc, int bone_intensity) {
    int size = voxels_of_bloc.size();

    for (int i = 0; i < size; i++) {
        if ( voxels_of_bloc[i] > bone_intensity) {
            return true;
        }
    }
    return false;
}
