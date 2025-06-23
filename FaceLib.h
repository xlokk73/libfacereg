#ifndef FACELIB_H
#define FACELIB_H

#include <string>
#include <opencv2/opencv.hpp>

// Define export macro properly
#ifdef _WIN32
    #ifdef FACELIB_EXPORTS
        #define FACELIB_API __declspec(dllexport)
    #else
        #define FACELIB_API __declspec(dllimport)
    #endif
#else
    #define FACELIB_API
#endif

// String utility functions
FACELIB_API int StringLength(std::string& str);
FACELIB_API void ReverseString(std::string& str);
FACELIB_API void Test();

// Image processing functions
FACELIB_API cv::Mat loadImage(const std::string& filename);
FACELIB_API cv::Mat loadImageGrayscale(const std::string& filename);
FACELIB_API void displayImage(const cv::Mat& image, const std::string& windowName, int waitTime = 0);
FACELIB_API void closeAllWindows();

// Image manipulation functions
FACELIB_API void rotate90(cv::Mat& my_image);

#endif // FACELIB_H