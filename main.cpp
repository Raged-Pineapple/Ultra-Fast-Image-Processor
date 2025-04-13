#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

void processImagePart(Mat& img, int startRow, int endRow) {
    for (int y = startRow; y < endRow; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            Vec3b& pixel = img.at<Vec3b>(y, x);
            uchar grayValue = 0.299 * pixel[2] + 
                              0.587 * pixel[1] + 
                              0.114 * pixel[0];
            pixel = Vec3b(grayValue, grayValue, grayValue);
        }
    }
}

void convertToGrayscaleWithoutThreads(Mat& img) {
    processImagePart(img, 0, img.rows);
}

void convertToGrayscaleWithThreads(Mat& img, int numThreads) {
    vector<thread> threads;
    int rowsPerThread = img.rows / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? img.rows : (i + 1) * rowsPerThread;
        threads.emplace_back(processImagePart, ref(img), startRow, endRow);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    string input = "/mnt/c/Users/Dell/Downloads/os-test.jpg";
    string output1 = "grayscale_output_no_threads.jpg";
    string output2 = "grayscale_output_with_threads.jpg";
    Mat img = imread(input);

    if (img.empty()) {
        cerr << "Error loading image" << endl;
        return -1;
    }

    // Measure time for no threads
    Mat imgWithoutThreads = img.clone();
    auto startNoThreads = high_resolution_clock::now();
    convertToGrayscaleWithoutThreads(imgWithoutThreads);
    auto endNoThreads = high_resolution_clock::now();
    double timeNoThreads = duration_cast<milliseconds>(endNoThreads - startNoThreads).count();
    imwrite(output1, imgWithoutThreads);

    // Measure time for threads
    Mat imgWithThreads = img.clone();
    auto startWithThreads = high_resolution_clock::now();
    convertToGrayscaleWithThreads(imgWithThreads, 4);
    auto endWithThreads = high_resolution_clock::now();
    double timeWithThreads = duration_cast<milliseconds>(endWithThreads - startWithThreads).count();
    imwrite(output2, imgWithThreads);

    // Display results
    cout << "Time without threads: " << timeNoThreads << " ms" << endl;
    cout << "Time with threads: " << timeWithThreads << " ms" << endl;

    // Save results for plotting
    ofstream resultFile("performance_data.txt");
    resultFile << "Without Threads " << timeNoThreads << endl;
    resultFile << "With Threads " << timeWithThreads << endl;
    resultFile.close();

    cout << "Performance data saved to performance_data.txt. Use Python or any graphing tool to visualize." << endl;

    return 0;
}
