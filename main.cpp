#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "FaceLib.h"

int main() {
    try {
        // Load single Haar cascade for face detection
        const std::string cascadePath = "C:/Users/vassg031/CLionProjects/FaceRecognition/Cascade/haarcascade_frontalface_alt.xml";

        std::cout << "Loading Haar cascade...\n";
        if (!loadHaarCascade(cascadePath)) {
            std::cerr << "Failed to load Haar cascade. Please check the path.\n";
            return EXIT_FAILURE;
        }

        // Load and display image from binary
        const std::string imagePath = "C:/Users/vassg031/CLionProjects/FaceRecognition/face.jpg";
        std::cout << "\nReading image file into memory...\n";
        std::vector<unsigned char> imageData = readImageFile(imagePath);
        std::cout << "Image loaded. Size: " << imageData.size() << " bytes\n";

        // Load original image
        std::cout << "Decoding color image...\n";
        ImageData* originalImage = loadImageFromBinary(imageData);
        int width, height;
        getImageDimensions(originalImage, width, height);
        std::cout << "Original image dimensions: " << width << "x" << height << '\n';

        // Display original image
        displayImage(originalImage, "1. Original Image", 3000);

        // Detect faces
        std::cout << "\nDetecting faces...\n";
        std::vector<FaceRect> faces = detectFaces(originalImage, 1.1, 3, 50);
        std::cout << "Detected " << faces.size() << " face(s)\n";

        if (faces.empty()) {
            std::cout << "No faces detected in the image.\n";
        } else {
            std::cout << "Face detections:\n";
            for (size_t i = 0; i < faces.size(); ++i) {
                std::cout << "  Face " << (i+1) << ": x=" << faces[i].x
                         << ", y=" << faces[i].y
                         << ", width=" << faces[i].width
                         << ", height=" << faces[i].height << '\n';
            }

            // Draw rectangles around detected faces
            std::cout << "\nDrawing face rectangles...\n";
            ImageData* imageWithFaces = drawFaceRectangles(originalImage, faces);
            displayImage(imageWithFaces, "2. Face Detection", 3000);

            // Crop to the largest face with no padding
            std::cout << "\nCropping to largest face (no padding)...\n";
            ImageData* croppedFace = cropToLargestFace(originalImage, 0.0); // No padding
            getImageDimensions(croppedFace, width, height);
            std::cout << "Cropped face dimensions: " << width << "x" << height << '\n';

            // Display cropped face
            displayImage(croppedFace, "3. Cropped Face", 0); // Wait for key press

            // Save the cropped face to binary
            std::cout << "\nSaving cropped face to binary...\n";
            std::vector<unsigned char> croppedFaceData = saveImageToBinary(croppedFace, ".jpg");

            // Write the cropped face to file
            std::cout << "Writing cropped face to file...\n";
            writeBinaryToFile(croppedFaceData, "C:/Users/vassg031/CLionProjects/FaceRecognition/cropped_face.jpg");

            // Clean up face detection images
            deleteImage(imageWithFaces);
            deleteImage(croppedFace);
        }

        // Clean up memory
        std::cout << "\nCleaning up resources...\n";
        deleteImage(originalImage);

        // Close all OpenCV windows
        std::cout << "Closing all windows...\n";
        closeAllWindows();
    }
    catch (const FaceDetectionException& e) {
        std::cerr << "Face Detection Error: " << e.what() << '\n';
        return EXIT_FAILURE;
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
}