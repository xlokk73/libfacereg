#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "FaceLib.h"

// Reads an image file into a binary buffer
std::vector<uchar> readImageFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Error: Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uchar> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error: Failed to read file: " + filename);
    }

    return buffer;
}

// Decodes a color image from binary data
cv::Mat loadImageFromBinary(const std::vector<uchar>& imageData) {
    cv::Mat image = cv::imdecode(imageData, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Error: Failed to decode color image data.");
    }
    return image;
}

// Decodes a grayscale image from binary data
cv::Mat loadGrayscaleFromBinary(const std::vector<uchar>& imageData) {
    cv::Mat image = cv::imdecode(imageData, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        throw std::runtime_error("Error: Failed to decode grayscale image data.");
    }
    return image;
}

std::vector<uchar> save_image(cv::Mat& my_image, const std::string& format = ".jpg") {
    if (my_image.empty()) {
        throw std::runtime_error("Error: Cannot save empty image.");
    }

    std::vector<uchar> buffer;
    std::vector<int> compression_params;

    // Set compression parameters based on format
    std::string ext = format;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".jpg" || ext == ".jpeg") {
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(95); // High quality (0-100)
    } else if (ext == ".png") {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(1); // Low compression for better quality (0-9)
    } else if (ext == ".webp") {
        compression_params.push_back(cv::IMWRITE_WEBP_QUALITY);
        compression_params.push_back(95); // High quality (1-100)
    }

    // Encode image to binary buffer
    bool success = cv::imencode(format, my_image, buffer, compression_params);

    if (!success) {
        throw std::runtime_error("Error: Failed to encode image to binary format: " + format);
    }

    std::cout << "Image saved to binary format " << format << ". Size: " << buffer.size() << " bytes" << std::endl;
    return buffer;
}

// Optional: Helper function to write binary data to file if needed
void writeBinaryToFile(const std::vector<uchar>& binaryData, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    if (!file) {
        throw std::runtime_error("Error: Failed to write binary data to file: " + filename);
    }

    std::cout << "Binary data written to file: " << filename << std::endl;
}

int main() {
    try {
        // Test string utilities
        std::string testStr = "Hello, World!";
        std::cout << "Original string: " << testStr << '\n';

        int length = StringLength(testStr);
        std::cout << "String length: " << length << '\n';

        ReverseString(testStr);
        std::cout << "Reversed string: " << testStr << '\n';

        Test();

        // Load and display image from binary
        const std::string imagePath = "C:/Users/vassg031/CLionProjects/FaceRecognition/HelloThere.jpg";
        std::cout << "\nReading image file into memory...\n";
        std::vector<uchar> imageData = readImageFile(imagePath);
        std::cout << "Image loaded. Size: " << imageData.size() << " bytes\n";

        // Load and display color image
        std::cout << "Decoding color image...\n";
        cv::Mat colorImage = loadImageFromBinary(imageData);
        std::cout << "Color image dimensions: " << colorImage.cols << "x" << colorImage.rows << '\n';
        displayImage(colorImage, "Color Image (from binary)", 1000);

        // Load and display grayscale image
        std::cout << "\nDecoding grayscale image...\n";
        cv::Mat grayImage = loadGrayscaleFromBinary(imageData);
        std::cout << "Grayscale image dimensions: " << grayImage.cols << "x" << grayImage.rows << '\n';
        displayImage(grayImage, "Grayscale Image (from binary)", 0);

        // Rotate and display the image
        std::cout << "\nRotating image by 90 degrees...\n";
        rotate90(colorImage);
        displayImage(colorImage, "Rotated Color Image", 1000);

        // Save images using binary approach (no file paths)
        std::cout << "\nSaving original color image to binary...\n";
        cv::Mat originalColor = loadImageFromBinary(imageData); // Reload original
        std::vector<uchar> savedOriginal = save_image(originalColor, ".jpg");

        // Save the grayscale image to binary
        std::cout << "Saving grayscale image to binary...\n";
        std::vector<uchar> savedGrayscale = save_image(grayImage, ".png");

        // Save the rotated image to binary
        std::cout << "Saving rotated image to binary...\n";
        std::vector<uchar> savedRotated = save_image(colorImage, ".jpg");

        // Optional: Write binary data to files if needed
        std::cout << "\nOptionally writing binary data to files...\n";
        writeBinaryToFile(savedOriginal, "C:/Users/vassg031/CLionProjects/FaceRecognition/saved_from_binary_original.jpg");
        writeBinaryToFile(savedGrayscale, "C:/Users/vassg031/CLionProjects/FaceRecognition/saved_from_binary_grayscale.png");
        writeBinaryToFile(savedRotated, "C:/Users/vassg031/CLionProjects/FaceRecognition/saved_from_binary_rotated.jpg");

        // Demonstrate round-trip: binary -> image -> binary
        std::cout << "\nDemonstrating round-trip conversion...\n";
        cv::Mat roundTripImage = loadImageFromBinary(savedOriginal);
        std::vector<uchar> roundTripBinary = save_image(roundTripImage, ".png");
        std::cout << "Round-trip completed. Final binary size: " << roundTripBinary.size() << " bytes\n";

        // Close all OpenCV windows
        std::cout << "\nClosing all windows...\n";
        closeAllWindows();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Program completed successfully.\n";
    return EXIT_SUCCESS;
}