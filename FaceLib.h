#ifndef FACELIB_H
#define FACELIB_H

#include <string>
#include <vector>
#include <stdexcept>

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

// Forward declaration to hide OpenCV implementation
class ImageData;

// Custom exception classes for better error handling
class FACELIB_API FaceLibException : public std::runtime_error {
public:
    explicit FaceLibException(const std::string& message) : std::runtime_error(message) {}
};

class FACELIB_API ImageLoadException : public FaceLibException {
public:
    explicit ImageLoadException(const std::string& filename)
        : FaceLibException("Failed to load image: " + filename) {}
};

class FACELIB_API ImageSaveException : public FaceLibException {
public:
    explicit ImageSaveException(const std::string& format)
        : FaceLibException("Failed to save image in format: " + format) {}
};

class FACELIB_API ImageProcessingException : public FaceLibException {
public:
    explicit ImageProcessingException(const std::string& operation)
        : FaceLibException("Image processing failed: " + operation) {}
};

class FACELIB_API FileOperationException : public FaceLibException {
public:
    explicit FileOperationException(const std::string& filename)
        : FaceLibException("File operation failed: " + filename) {}
};

// Image processing functions - no OpenCV types exposed
FACELIB_API ImageData* loadImageFromFile(const std::string& filename);
FACELIB_API ImageData* loadImageFromBinary(const std::vector<unsigned char>& imageData, bool grayscale = false);
FACELIB_API void displayImage(const ImageData* image, const std::string& windowName, int waitTime = 0);
FACELIB_API void closeAllWindows();

// Image manipulation functions
FACELIB_API std::vector<unsigned char> saveImageToBinary(const ImageData* image, const std::string& format = ".jpg");

// Image utility functions
FACELIB_API void getImageDimensions(const ImageData* image, int& width, int& height);
FACELIB_API bool isImageEmpty(const ImageData* image);
FACELIB_API ImageData* copyImage(const ImageData* source);
FACELIB_API void deleteImage(ImageData* image);

// File I/O functions
FACELIB_API std::vector<unsigned char> readImageFile(const std::string& filename);
FACELIB_API void writeBinaryToFile(const std::vector<unsigned char>& binaryData, const std::string& filename);

#endif // FACELIB_H