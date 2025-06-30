	#include "FaceLib.h"
	#include <opencv2/opencv.hpp>
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