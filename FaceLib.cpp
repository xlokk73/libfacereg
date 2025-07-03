#include "FaceLib.h"
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

// Internal class to wrap cv::Mat - hidden from header
class ImageData {
public:
    cv::Mat mat;

    ImageData() = default;
    explicit ImageData(const cv::Mat& image) : mat(image) {}
    ImageData(const ImageData& other) : mat(other.mat.clone()) {}
    ImageData& operator=(const ImageData& other) {
        if (this != &other) {
            mat = other.mat.clone();
        }
        return *this;
    }
};

// Global Haar cascade classifier
static cv::CascadeClassifier faceCascade;
static bool cascadeLoaded = false;

// File I/O functions
FACELIB_API std::vector<unsigned char> readImageFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw FileOperationException("Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw FileOperationException("Failed to read file: " + filename);
    }

    return buffer;
}

FACELIB_API void writeBinaryToFile(const std::vector<unsigned char>& binaryData, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw FileOperationException("Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    if (!file) {
        throw FileOperationException("Failed to write binary data to file: " + filename);
    }

    std::cout << "Binary data written to file: " << filename << std::endl;
}

// Image processing functions
FACELIB_API ImageData* loadImageFromFile(const std::string& filename) {
    try {
        cv::Mat image = cv::imread(filename);
        if (image.empty()) {
            throw ImageLoadException(filename);
        }
        return new ImageData(image);
    } catch (const cv::Exception& e) {
        throw ImageLoadException(filename + " - OpenCV error: " + e.what());
    }
}

FACELIB_API ImageData* loadImageFromBinary(const std::vector<unsigned char>& imageData, bool grayscale) {
    try {
        int flags = grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR;
        cv::Mat image = cv::imdecode(imageData, flags);
        if (image.empty()) {
            std::string mode = grayscale ? "grayscale" : "color";
            throw ImageProcessingException("Failed to decode " + mode + " image from binary data");
        }
        return new ImageData(image);
    } catch (const cv::Exception& e) {
        throw ImageProcessingException("OpenCV decode error: " + std::string(e.what()));
    }
}

FACELIB_API void displayImage(const ImageData* image, const std::string& windowName, int waitTime) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot display empty or null image");
    }

    try {
        cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
        cv::imshow(windowName, image->mat);
        cv::waitKey(waitTime);
    } catch (const cv::Exception& e) {
        throw ImageProcessingException("Display error: " + std::string(e.what()));
    }
}

FACELIB_API void closeAllWindows() {
    try {
        cv::destroyAllWindows();
    } catch (const cv::Exception& e) {
        throw ImageProcessingException("Failed to close windows: " + std::string(e.what()));
    }
}

FACELIB_API std::vector<unsigned char> saveImageToBinary(const ImageData* image, const std::string& format) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot save empty or null image");
    }

    std::vector<unsigned char> buffer;
    std::vector<int> compression_params;

    // Set compression parameters based on format
    std::string ext = format;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    try {
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
        bool success = cv::imencode(format, image->mat, buffer, compression_params);

        if (!success) {
            throw ImageSaveException(format);
        }

        std::cout << "Image saved to binary format " << format << ". Size: " << buffer.size() << " bytes" << std::endl;
        return buffer;
    } catch (const cv::Exception& e) {
        throw ImageSaveException(format + " - OpenCV error: " + e.what());
    }
}

// Image utility functions
FACELIB_API void getImageDimensions(const ImageData* image, int& width, int& height) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot get dimensions of empty or null image");
    }
    width = image->mat.cols;
    height = image->mat.rows;
}

FACELIB_API bool isImageEmpty(const ImageData* image) {
    return !image || image->mat.empty();
}

FACELIB_API ImageData* copyImage(const ImageData* source) {
    if (!source || source->mat.empty()) {
        throw ImageProcessingException("Cannot copy empty or null image");
    }
    return new ImageData(*source);
}

FACELIB_API void deleteImage(ImageData* image) {
    delete image;
}

// Face detection functions
FACELIB_API bool loadHaarCascade(const std::string& cascadePath) {
    try {
        cascadeLoaded = faceCascade.load(cascadePath);
        if (!cascadeLoaded) {
            throw FaceDetectionException("Failed to load Haar cascade from: " + cascadePath);
        }
        std::cout << "Haar cascade loaded successfully from: " << cascadePath << std::endl;
        return true;
    } catch (const cv::Exception& e) {
        throw FaceDetectionException("OpenCV error loading cascade: " + std::string(e.what()));
    }
}

FACELIB_API std::vector<FaceRect> detectFaces(const ImageData* image, double scaleFactor, int minNeighbors, int minSize) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot detect faces in empty or null image");
    }

    if (!cascadeLoaded) {
        throw FaceDetectionException("Haar cascade not loaded. Call loadHaarCascade() first.");
    }

    std::vector<FaceRect> result;

    try {
        // Convert to grayscale if needed
        cv::Mat grayImage;
        if (image->mat.channels() == 3) {
            cv::cvtColor(image->mat, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image->mat;
        }

        // Detect faces
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(
            grayImage,
            faces,
            scaleFactor,
            minNeighbors,
            0,
            cv::Size(minSize, minSize)
        );

        // Convert cv::Rect to FaceRect
        for (const auto& face : faces) {
            result.emplace_back(face.x, face.y, face.width, face.height);
        }

        std::cout << "Detected " << result.size() << " face(s)" << std::endl;
        return result;

    } catch (const cv::Exception& e) {
        throw FaceDetectionException("OpenCV error during face detection: " + std::string(e.what()));
    }
}

FACELIB_API ImageData* cropToFace(const ImageData* image, const FaceRect& face, double padding) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot crop empty or null image");
    }

    try {
        // Calculate padding
        int padX = static_cast<int>(face.width * padding);
        int padY = static_cast<int>(face.height * padding);

        // Calculate crop rectangle with padding
        int cropX = std::max(0, face.x - padX);
        int cropY = std::max(0, face.y - padY);
        int cropWidth = std::min(image->mat.cols - cropX, face.width + 2 * padX);
        int cropHeight = std::min(image->mat.rows - cropY, face.height + 2 * padY);

        // Create crop rectangle
        cv::Rect cropRect(cropX, cropY, cropWidth, cropHeight);

        // Crop the image
        cv::Mat croppedMat = image->mat(cropRect);

        std::cout << "Cropped image to face region: " << cropWidth << "x" << cropHeight
                  << " (padding: " << (padding * 100) << "%)" << std::endl;

        return new ImageData(croppedMat);

    } catch (const cv::Exception& e) {
        throw ImageProcessingException("OpenCV error during cropping: " + std::string(e.what()));
    }
}

FACELIB_API ImageData* cropToLargestFace(const ImageData* image, double padding) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot crop empty or null image");
    }

    // Detect faces
    std::vector<FaceRect> faces = detectFaces(image);

    if (faces.empty()) {
        throw FaceDetectionException("No faces detected in image");
    }

    // Find the largest face
    FaceRect largestFace = faces[0];
    int largestArea = largestFace.width * largestFace.height;

    for (size_t i = 1; i < faces.size(); ++i) {
        int area = faces[i].width * faces[i].height;
        if (area > largestArea) {
            largestArea = area;
            largestFace = faces[i];
        }
    }

    std::cout << "Using largest face of " << faces.size() << " detected faces" << std::endl;

    return cropToFace(image, largestFace, padding);
}

FACELIB_API ImageData* drawFaceRectangles(const ImageData* image, const std::vector<FaceRect>& faces) {
    if (!image || image->mat.empty()) {
        throw ImageProcessingException("Cannot draw on empty or null image");
    }

    try {
        // Create a copy of the image
        cv::Mat resultMat = image->mat.clone();

        // Draw rectangles around faces
        for (const auto& face : faces) {
            cv::Rect rect(face.x, face.y, face.width, face.height);
            cv::rectangle(resultMat, rect, cv::Scalar(0, 255, 0), 2); // Green rectangle

            // Add face number text
            std::string text = "Face";
            cv::putText(resultMat, text,
                       cv::Point(face.x, face.y - 10),
                       cv::FONT_HERSHEY_SIMPLEX,
                       0.5,
                       cv::Scalar(0, 255, 0),
                       1);
        }

        std::cout << "Drew rectangles around " << faces.size() << " face(s)" << std::endl;

        return new ImageData(resultMat);

    } catch (const cv::Exception& e) {
        throw ImageProcessingException("OpenCV error while drawing rectangles: " + std::string(e.what()));
    }
}