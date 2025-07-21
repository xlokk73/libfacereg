
#include <iostream>
#include <string>
#include <vector>
#include "FaceLib.h"

// Reusable function to process and save face from an image
bool processAndSaveFace(const std::string& imagePath, const std::string& label, const std::string& outputPath) {
try {
std::cout << "\n=== Processing " << label << " ===\n";

std::vector<unsigned char> imageData = readImageFile(imagePath);
if (imageData.empty()) {
std::cerr << "Error: " << label << " image is empty or could not be read.\n";
return false;
}

ImageData* originalImage = loadImageFromBinary(imageData);
int width, height;
getImageDimensions(originalImage, width, height);
std::cout << label << " dimensions: " << width << "x" << height << '\n';

displayImage(originalImage, label + " - Original", 3000);

std::vector<FaceRect> faces = detectFaces(originalImage, 1.1, 3, 50);
std::cout << "Detected " << faces.size() << " face(s) in " << label << '\n';

if (!faces.empty()) {
ImageData* imageWithFaces = drawFaceRectangles(originalImage, faces);
displayImage(imageWithFaces, label + " - Face Detection", 3000);

ImageData* croppedFace = cropToLargestFace(originalImage, 0.0);
ImageData* grayscaleFace = convertToGrayscale(croppedFace);
getImageDimensions(grayscaleFace, width, height);
std::cout << "Cropped Grayscale " << label << " dimensions: " << width << "x" << height << '\n';

displayImage(grayscaleFace, label + " - Grayscale Face", 0);

std::vector<unsigned char> croppedFaceData = saveImageToBinary(grayscaleFace, ".jpg");
writeBinaryToFile(croppedFaceData, outputPath);

deleteImage(imageWithFaces);
deleteImage(croppedFace);
deleteImage(grayscaleFace);
} else {
std::cout << "No faces detected in " << label << ".\n";
}

deleteImage(originalImage);
return true;
} catch (const std::exception& e) {
std::cerr << "Error processing " << label << ": " << e.what() << '\n';
return false;
}
}

int main() {
try {
const std::string cascadePath = "../Cascade/haarcascade_frontalface_alt.xml";
std::cout << "Loading Haar cascade...\n";
if (!loadHaarCascade(cascadePath)) {
std::cerr << "Failed to load Haar cascade. Please check the path.\n";
return EXIT_FAILURE;
}

// List of images to process
std::vector<std::pair<std::string, std::string>> imagesToProcess = {
{"../face.jpg", "cropped_face1.jpg"},
{"../face2.jpg", "cropped_face2.jpg"},
};

// Process each image
for (size_t i = 0; i < imagesToProcess.size(); ++i) {
std::string label = "Face " + std::to_string(i + 1);
std::string inputPath = imagesToProcess[i].first;
std::string outputPath = "C:/Users/vassg031/CLionProjects/FaceRecognition/" + imagesToProcess[i].second;

bool success = processAndSaveFace(inputPath, label, outputPath);
std::cout << label << ": " << (success ? "SUCCESS" : "FAILED") << "\n";
}

std::cout << "\nClosing all windows...\n";
closeAllWindows();
}
catch (const std::exception& e) {
std::cerr << "Unexpected Error: " << e.what() << '\n';
return EXIT_FAILURE;
}

std::cout << "Program completed successfully.\n";
return EXIT_SUCCESS;
}