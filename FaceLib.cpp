#include "FaceLib.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

// String utility functions
FACELIB_API int StringLength(std::string& str) {
    return static_cast<int>(str.length());
}

FACELIB_API void ReverseString(std::string& str) {
    std::reverse(str.begin(), str.end());
}

FACELIB_API void Test() {
    std::string version = cv::getVersionString();
    std::cout << "OpenCV Version: " << version << std::endl;
}

// Image processing functions
FACELIB_API cv::Mat loadImage(const std::string& filename) {
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Error: Could not load image: " << filename << std::endl;
    }
    return image;
}

FACELIB_API cv::Mat loadImageGrayscale(const std::string& filename) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error: Could not load grayscale image: " << filename << std::endl;
    }
    return image;
}

FACELIB_API void displayImage(const cv::Mat& image, const std::string& windowName, int waitTime) {
    if (image.empty()) {
        std::cerr << "Error: Cannot display empty image" << std::endl;
        return;
    }

    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName, image);

    // Only wait if waitTime > 0
    if (waitTime > 0) {
        cv::waitKey(waitTime);
    }
    // Don't wait if waitTime is 0 - let caller handle it
}

FACELIB_API void closeAllWindows() {
    cv::destroyAllWindows();
}

// Implementation of rotate90
FACELIB_API void rotate90(cv::Mat& my_image) {
    if (my_image.empty()) {
        std::cerr << "Error: Cannot rotate an empty image." << std::endl;
        return;
    }
    cv::rotate(my_image, my_image, cv::ROTATE_90_CLOCKWISE);
}