#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <algorithm>

void loadTextures() {
	std::string path = "../assets/terrain/hm.palette.txt";
	std::string line;
	std::ifstream pallete(path);
	while( getline(pallete, line) ) {
		size_t end = 0;
		size_t start = 0;
		std::string token;
		std::string delimiter = ", ";
		std::string fname;
		float color[3];
		for(unsigned int i = 0; (end = line.find(delimiter, start)) != std::string::npos; ++i) {
			token = line.substr(start, end - start);
			if (token[0] != '\"') {
				color[i] = std::stoi(token)/255.0f;
			}
			start = end + delimiter.length();
		}
		fname = line.substr(start + 1, line.length() - start - 1 - 3);
		fname.erase(std::remove_if(fname.begin(), fname.end(), isspace), fname.end());
		std::cout << color[0] << " " << color[1] << " " << color[2] << std::endl;
		std::cout << fname << std::endl;
	}
}

int main() {
	//loadTextures();
	std::string path = "assets/terrain/alphamap.txt";
	std::string line;
	std::ifstream texture_files(path);

	for(unsigned int i = 0; getline(texture_files, line); ++i) {
		if (line.length() > 0) {
			std::cout << line;
		}
	}
	return 0;
}