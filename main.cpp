#include <iostream>
#include <string>
#include <vector>
#include "FaceLib.h"

int main() {
    try {
        // Load and display image from binary
        const std::string imagePath = "C:/Users/vassg031/CLionProjects/FaceRecognition/HelloThere.jpg";
        std::cout << "\nReading image file into memory...\n";
        std::vector<unsigned char> imageData = readImageFile(imagePath);
        std::cout << "Image loaded. Size: " << imageData.size() << " bytes\n";

        // Load and display color image
        std::cout << "Decoding color image...\n";
        ImageData* colorImage = loadImageFromBinary(imageData, false);
        int width, height;
        getImageDimensions(colorImage, width, height);
        std::cout << "Color image dimensions: " << width << "x" << height << '\n';
        displayImage(colorImage, "Color Image (from binary)", 1000);

        // Load and display grayscale image
        std::cout << "\nDecoding grayscale image...\n";
        ImageData* grayImage = loadImageFromBinary(imageData, true);
        getImageDimensions(grayImage, width, height);
        std::cout << "Grayscale image dimensions: " << width << "x" << height << '\n';
        displayImage(grayImage, "Grayscale Image (from binary)", 0);

        // Save images using binary approach (no file paths)
        std::cout << "\nSaving original color image to binary...\n";
        ImageData* originalColor = loadImageFromBinary(imageData, false); // Reload original
        std::vector<unsigned char> savedOriginal = saveImageToBinary(originalColor, ".jpg");

        // Save the grayscale image to binary
        std::cout << "Saving grayscale image to binary...\n";
        std::vector<unsigned char> savedGrayscale = saveImageToBinary(grayImage, ".png");

        // Optional: Write binary data to files if needed
        std::cout << "\nOptionally writing binary data to files...\n";
        writeBinaryToFile(savedOriginal, "C:/Users/vassg031/CLionProjects/FaceRecognition/saved_from_binary_original.jpg");

        // Demonstrate round-trip: binary -> image -> binary
        std::cout << "\nDemonstrating round-trip conversion...\n";
        ImageData* roundTripImage = loadImageFromBinary(savedOriginal, false);
        std::vector<unsigned char> roundTripBinary = saveImageToBinary(roundTripImage, ".png");
        std::cout << "Round-trip completed. Final binary size: " << roundTripBinary.size() << " bytes\n";

        // Clean up memory
        std::cout << "\nCleaning up resources...\n";
        deleteImage(colorImage);
        deleteImage(grayImage);
        deleteImage(originalColor);
        deleteImage(roundTripImage);

        // Close all OpenCV windows
        std::cout << "Closing all windows...\n";
        closeAllWindows();
    }
    catch (const ImageLoadException& e) {
        std::cerr << "Image Load Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const ImageSaveException& e) {
        std::cerr << "Image Save Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const ImageProcessingException& e) {
        std::cerr << "Image Processing Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const FileOperationException& e) {
        std::cerr << "File Operation Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const FaceLibException& e) {
        std::cerr << "FaceLib Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Program completed successfully.\n";
    return EXIT_SUCCESS;
};
