//------------------------------------------------------------------------
// Program: POUDEL_BIPLOV_CSCI4065_ASSIGN01.cpp
// Author: Biplov Poudel
// Student Number: 30156220
// Organization: The University of Louisiana at Monroe
// Class: CSCI 4065, Adv. topics in Computer Science
// Assignment: Assignment 01
// Due Date: 2026, 03, 24
//
// Description: This program reads an input file containing image dimensions and pixel data, converts the pixel data to grey scale and writes the output to a new file in the same format.
// Honor Statement: My signature below attests to the fact that I have
// neither given nor received aid on this project.
//
// Signature: Biplov Poudel
// _____________________________________________________________
//
//------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>

// Helper function to remove whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Structure to hold pixel data
struct PixelData {
    int x, y;
    int red, green, blue, alpha;
    double grey;
};

// Structure to hold processed pixel result
struct PixelResult {
    int index;
    int x, y;
    double grey;
    int alpha;
};

// Thread-safe queue for work distribution
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cond;

public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(value);
        }
        cond.notify_one();
    }

    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.empty()) return false;
        value = queue.front();
        queue.pop();
        return true;
    }

    bool wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [this] { return !queue.empty(); });
        value = queue.front();
        queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};

// Worker thread function to process pixels
void processPixels(ThreadSafeQueue<std::pair<int, PixelData>>& inputQueue,
                   ThreadSafeQueue<PixelResult>& outputQueue,
                   std::atomic<bool>& done) {
    std::pair<int, PixelData> work;
    
    while (inputQueue.wait_and_pop(work)) {
        int index = work.first;
        PixelData pixel = work.second;
        
        // Compute the grey value
        double grey = (0.299 * pixel.red) + (0.587 * pixel.green) + (0.114 * pixel.blue);
        
        // Create result and push to output queue
        PixelResult result;
        result.index = index;
        result.x = pixel.x;
        result.y = pixel.y;
        result.grey = grey;
        result.alpha = pixel.alpha;
        
        outputQueue.push(result);
    }
}

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        std::cerr << "Usage: ./POUDEL_BIPLOV_CSCI4065_ASSIGN01 <input_file> <output_file>\n";
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    // Open input file for reading
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
        std::cerr << "Error: Could not open input file: " << input_file << "\n";
        return 1;
    }

    // Open output file for writing
    std::ofstream out_file(output_file);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not create output file: " << output_file << "\n";
        return 1;
    }

    std::string line;
    
    // Read and process the first line (Image Dimensions)
    if (!std::getline(in_file, line)) {
        std::cerr << "Error: Input file is empty.\n";
        return 1;
    }

    line = trim(line);
    size_t comma_pos = line.find(',');
    if (comma_pos == std::string::npos) {
        std::cerr << "Error: First line must contain width and height separated by a comma.\n";
        return 1;
    }

    int width, height;
    try {
        width = std::stoi(trim(line.substr(0, comma_pos)));
        height = std::stoi(trim(line.substr(comma_pos + 1)));
    } catch (...) {
        std::cerr << "Error: Width and height must be integers.\n";
        return 1;
    }

    // Write dimensions directly to output file
    out_file << width << "," << height << "\n";

    // Read and process the remaining lines (Pixel Data) one by one
    while (std::getline(in_file, line)) {
        line = trim(line);
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string item;
        int parts[6];
        int count = 0;
        bool valid = true;

        // Extract exactly 6 integers separated by commas
        while (std::getline(ss, item, ',')) {
            if (count >= 6) { 
                valid = false; // Too many values
                break; 
            }
            try {
                parts[count] = std::stoi(trim(item));
                count++;
            } catch (...) {
                valid = false; // Non-integer value found
                break;
            }
        }

        // If the line doesn't have exactly 6 integers, skip it
        if (!valid || count != 6) {
            std::cerr << "Skipping invalid line: " << line << "\n";
            continue;
        }

        int x = parts[0];
        int y = parts[1];
        int red = parts[2];
        int green = parts[3];
        int blue = parts[4];
        int alpha = parts[5];

        // Compute the grey value
        double grey = (0.299 * red) + (0.587 * green) + (0.114 * blue);

        // Write the calculated grey pixel directly to the file
        out_file << x << "," << y << ","
                 << std::fixed << std::setprecision(3) << grey << ","
                 << std::fixed << std::setprecision(3) << grey << ","
                 << std::fixed << std::setprecision(3) << grey << ","
                 << alpha << "\n";
    }

    // Close files to free up system resources
    in_file.close();
    out_file.close();

    return 0;
}