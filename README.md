# RGBA to Greyscale Textual Pixel Converter

A highly efficient, sequential C++ command-line utility that processes textual representations of images, converting RGBA pixel data into Greyscale using the standard relative luminance formula.

## 🚀 Overview
This project was developed to cleanly and efficiently manipulate image data at the pixel level. It reads a source text file containing image dimensions and RGBA values, applies a weighted grayscale conversion, and streams the formatted output to a new destination file.

## ✨ Technical Highlights
* **Extreme Memory Efficiency:** Engineered with an **O(1) space complexity**. Instead of loading the entire image into memory (such as vectors or arrays) which can crash on large 4K files, the program streams data line-by-line. This allows it to convert massive, gigabyte-sized files instantly with near-zero RAM footprint.
* **Modern C++ Standards:** Written in pure C++ and compiles flawlessly under the `C++17` standard.
* **Robust Error Handling:** Safely catches missing files, empty inputs, non-integer data, and malformed CSV lines, skipping bad data without interrupting the execution flow for valid pixels.

## 🧮 The Math
The program calculates the grayscale value using the standard weighted average for human visual perception of luminance. It applies the following formula to the Red, Green, and Blue channels:

`Grey = (0.299 * R) + (0.587 * G) + (0.114 * B)`

The resulting `Grey` value is duplicated across the R, G, and B channels in the output file, formatted to exactly three decimal places. The Alpha channel is preserved untouched.

## 📄 File Formats

### Input Format (`RGBA.txt`)
A comma-separated text file. The first line contains the image dimensions (width, height), and every subsequent line contains exactly 6 integers representing a single pixel (X, Y, R, G, B, A).
```text
2,2
0,0,255,0,0,255
0,1,0,255,0,255
1,0,0,0,255,255
1,1,255,255,255,255
```
### Output Format (GREY.txt)
A corresponding text file where the distinct RGB values are replaced by the calculated grey value.
``` text
2,2
0,0,76.245,76.245,76.245,255
0,1,149.685,149.685,149.685,255
1,0,29.070,29.070,29.070,255
1,1,255.000,255.000,255.000,255
```
### 🛠️ Compilation & Usage
This program is designed to be compiled using GCC in a Linux environment.

1. Compile the source code:
   ```
   g++ -std=c++17 -o POUDEL_BIPLOV_CSCI4065_ASSIGN01 POUDEL_BIPLOV_CSCI4065_ASSIGN01.cpp
   ```
2. Execute the program:
Provide the input file name as the first argument, and the desired output file name as the second argument.
```
./POUDEL_BIPLOV_CSCI4065_ASSIGN01 RGBA.txt GREY.txt
```
### 👨‍💻 Author
Biplov Poudel Computer Science at the University of Louisiana at Monroe (ULM)
