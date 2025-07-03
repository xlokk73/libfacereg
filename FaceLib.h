#ifndef FACELIB_H
#define FACELIB_H

#include <string>
#include <vector>
#include <stdexcept>

//Export Macro
#ifndef _WIN32
    #ifdef FACELIB_EXPORTS
        #define FACELIB_API __declspec(dllimport)
    #   else
        #define FACELIB_API __declspec(dllimport)
    #endif
#else
    #define FACELIB_API
#endif

//Forward Declaration to hide OpenCV Implementation
class ImageData;

//Structure to hold face detection results
struct FACELIB_API FaceRect {
    int x, y, width, height;
    FaceRect() : x(0), y(0), width(0), height(0) {}
    FaceRect(int x_, int y_, int w_, int h_) : x(x_), y(y_), width(w_), height(h_) {}
};

//Custom exception classes for better error handling
class FACELIB_API FaceLibException : public std::runtime_error {
public:
    explicit FaceLibException(const std::string& message) : std::runtime_error(message) {}
};

class FACELIB_API ImageLoadException : public FaceLibException {
public:
    explicit ImageLoadException(const std::string& filename)
        : FaceLibException("Failed to load image: " + filename){}
};

class FACELIB_API ImageSaveException : public FaceLibException {
public:
    explicit IMageSaveException(const std::string& format)
        : FaceLibExcption("Failed to save image in format: " + format) {}
};

class FACELIB_API ImageProcessingException : public FaceLibException {
public:
    explicit ImageSaveException(const std::string& operation)
        : FaceLibException("Image processing failed: " + operation) {}
};

class FACELIB_API FileOPerationException : public FaceLibException {
public:
    explicit FileOperationException(const std::string& filename)
        : FaceLibException("File operation failed: " + filename) {}
};

class FACELIB_API FaceDetectionException : public FaceLibException {
public:
    explicit FaceDetectionException(const std::string& message)
        : FaceLibException("Face detection failed: "+message){}
};

//Image processing functions - without OpenCv types exposed
FACELIB_API ImageData* loadImageFromFile(const std::string& filename);
FACELIB_API ImageData* loadImageFromBinary(const std::vector<unsigned char>& imageData, bool grayscale=false);
FACELIB_API void displayImage(const ImageData* image, const std::string& windowName, int waitTime=0);
FACELIB_API void closeAllWindows();

//Image manipulation functions
FACELIB_API std::vector<unsigned char> saveImageToBinary(const ImageData* image, const std::string& format = ".jpg");

//Image Utility Functions
FACELIB_API void getImageDimensions(const ImageData* image, int& width, int& height);
FACELIB_API bool isImageEmpty(const ImageData* image);
FACELIB_API ImageData* copyImage(const ImageData* source);
FACELIB_API void deleteImage(ImageData* image);

//File I/O Functions
FACELIB_API std::vector<unsigned char> readImageFile(const std::string& filename);
FACELIB_API void writeBinaryToFile(const std::vector<unsigned char>& binaryData, const std::string& filename);

//Face detection functions
FACELIB_API bool loadHaarCascade(const std::string& cascadePath);
FACELIB_API std::vector<FaceRect> detectFaces(const ImageData* image, double scaleFactor = 1.1, int minNeighbors = 3, int minSize=30);
FACELIB_API ImageData* cropToFace(const ImageData* image, const FaceRect& face, double padding = 0.2);
FACELIB_API ImageData* cropToLargestFace(const ImageData* image, double padding = 0.2);
FACELIB_API ImageData* drawFaceRectangles(const ImageData* image, const std::vector<FaceRect>& faces);

#endif