#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string encode(std::string line) {
    std::string result;
    for (size_t i = 0; i < line.size(); ++i) {
        result.push_back(char(line[i] + i % 95));
    }
    return result;
};

std::string decode(std::string line) {
    std::string result;
    for (size_t i = 0; i < line.size(); ++i) {
        result.push_back(char(line[i] - i % 95));
    }
    return result;
};

void encodeFile(std::string inputFileName, std::string outFileName) {
    std::ifstream inputFile;
    std::ofstream outputFile;

    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << "Error opening file: " << inputFileName << std::endl;
        return;
    }

    outputFile.open(outFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outFileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
        outputFile << encode(line) << std::endl;
    }
    inputFile.close();
    outputFile.close();
}

void decodeFile(std::string inputFileName, std::string outFileName) {
    std::ifstream inputFile;
    std::ofstream outputFile;

    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << "Error opening file: " << inputFileName << std::endl;
        return;
    }

    outputFile.open(outFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outFileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
        outputFile << decode(line) << std::endl;
    }
    inputFile.close();
    outputFile.close();
}

std::vector<fs::path> getAllFiles(const fs::path& directory) {
    std::vector<fs::path> files;

    // 递归遍历目录
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }
    return files;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Tool Name: " << argv[0] << std::endl;
        std::cout << "Tool arg1: " << argv[1] << std::endl;
    }

    fs::path rootPath = "/home/lijiaxiang/Desktop/record/bestPractice";
    std::vector<fs::path> filtes = getAllFiles(rootPath);

    for (auto file : filtes) {
        encodeFile(file.string(), file.string() + ".encoded");
        decodeFile(file.string() + ".encoded", file.string() + ".decoded");
    }
}