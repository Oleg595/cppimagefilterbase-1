#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include <vector>
#include<algorithm>
#include "stb_image_write.h"
#include "png_toolkit.h"

png_toolkit::png_toolkit()
{
}

png_toolkit::~png_toolkit()
{
    stbi_image_free(imgData.pixels);
}

bool png_toolkit::load( const std::string &pictureName )
{
    imgData.pixels = stbi_load(pictureName.c_str(), &imgData.w, &imgData.h, &imgData.compPerPixel, 0);
    return imgData.pixels != nullptr;
}

bool png_toolkit::save( const std::string &pictureName )
{
    return stbi_write_png(pictureName.c_str(),
                   imgData.w, imgData.h,
                   imgData.compPerPixel,
                          imgData.pixels, 0) != 0;
}


image_data png_toolkit::getPixelData( void ) const
{
    return imgData;
}

void png_toolkit::Black_White(int start_x, int start_y, int finish_x, int finish_y) {
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			stbi_uc x = (3 * imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] + 6 * imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] + imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2]) / 10;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = x;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = x;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = x;
		}
	}
}

stbi_uc png_toolkit::Matrix3_Edge(int i, int j, int start_x, int start_y, int finish_x, int finish_y) {
	int x = 0;
	for (int i1 = (i - 1 >= start_y) ? i - 1 : i; i1 < i + 2 && i1 < finish_y; i1++) {
		for (int j1 = (j - 1 >= start_x) ? j - 1 : j; j1 < j + 2 && j1 < finish_x; j1++) {
			x += imgData.pixels[imgData.compPerPixel * (i1 * imgData.w + j1) + 1];
		}
	}
	x = 10 * imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] - x;
	if (x < 0) {
		return 0;
	}
	if (x > 255) {
		return 255;
	}
	return x;
}

void png_toolkit::Edge(int start_x, int start_y, int finish_x, int finish_y) {
	Black_White(start_x, start_y, finish_x, finish_y);
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = Matrix3_Edge(i, j, start_x, start_y, finish_x, finish_y);
		}
	}
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)];
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)];
		}
	}
}

stbi_uc png_toolkit::Matrix3_Blur(int i, int j, int s, int start_x, int start_y, int finish_x, int finish_y) {
	int x = 0;
	int n = 0;
	for (int i1 = (i - 1 >= start_y) ? i - 1 : i; i1 < i + 2 && i1 < finish_y; i1++) {
		for (int j1 = (j - 1 >= start_x) ? j - 1 : j; j1 < j + 2 && j1 < finish_x; j1++) {
			x += imgData.pixels[imgData.compPerPixel * (i1 * imgData.w + j1) + s];
			n++;
		}
	}
	return (x / n);
}

void png_toolkit::Blur(int start_x, int start_y, int finish_x, int finish_y) {
	stbi_uc* arr = new stbi_uc[imgData.compPerPixel * imgData.h * imgData.w];
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			arr[imgData.compPerPixel * (i * imgData.w + j)] = Matrix3_Blur(i, j, 0, start_x, start_y, finish_x, finish_y);
			arr[imgData.compPerPixel * (i * imgData.w + j) + 1] = Matrix3_Blur(i, j, 1, start_x, start_y, finish_x, finish_y);
			arr[imgData.compPerPixel * (i * imgData.w + j) + 2] = Matrix3_Blur(i, j, 2, start_x, start_y, finish_x, finish_y);
		}
	}
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = arr[imgData.compPerPixel * (i * imgData.w + j)];
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = arr[imgData.compPerPixel * (i * imgData.w + j) + 1];
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = arr[imgData.compPerPixel * (i * imgData.w + j) + 2];
		}
	}
}

stbi_uc png_toolkit::Matrix5(int i, int j, int start_x, int start_y, int finish_x, int finish_y) {
	std::vector<stbi_uc> arr;
	int n = 0;
	for (int i1 = (i - 2 >= start_y) ? i - 2 : (i - 1 >= start_y) ? i - 1 : i; i1 < i + 3 && i1 < finish_y; i1++) {
		for (int j1 = (j - 2 >= start_x) ? j - 2 : (j - 1 >= start_x) ? j - 1 : j; j1 < j + 3 && j1 < finish_x; j1++) {
			n++;
			arr.push_back(imgData.pixels[imgData.compPerPixel * (i1 * imgData.w + j1) + 1]);
		}
	}
	std::sort(arr.begin(), arr.end());
	if (n % 2 == 0) {
		return arr[n / 2 - 1];
	}
	else {
		return arr[(n - 1) / 2];
	}
}

void png_toolkit::Threshold(int start_x, int start_y, int finish_x, int finish_y) {
	Black_White(start_x, start_y, finish_x, finish_y);
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			if (imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] < Matrix5(i, j, start_x, start_y, finish_x, finish_y)) {
				imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = 0;
			}
		}
	}
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)];
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)];
		}
	}
}

void png_toolkit::Red(int start_x, int start_y, int finish_x, int finish_y) {
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = 255;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = 0;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = 0;
		}
	}
}

void png_toolkit::Paint_Filter(Filter filter) {
	switch (filter.f_n) {
	case BLACK_WHITE:
		Black_White((filter.u == 0) ? 0 : imgData.w / filter.u, (filter.l == 0) ? 0 : imgData.h / filter.l, (filter.b == 0) ? 0 : imgData.w / filter.b, (filter.r == 0) ? 0 : imgData.h / filter.r);
		break;
	case RED:
		Red((filter.u == 0) ? 0 : imgData.w / filter.u, (filter.l == 0) ? 0 : imgData.h / filter.l, (filter.b == 0) ? 0 : imgData.w / filter.b, (filter.r == 0) ? 0 : imgData.h / filter.r);
		break;
	case THRESHOLD:
		Threshold((filter.u == 0) ? 0 : imgData.w / filter.u, (filter.l == 0) ? 0 : imgData.h / filter.l, (filter.b == 0) ? 0 : imgData.w / filter.b, (filter.r == 0) ? 0 : imgData.h / filter.r);
		break;
	case EDGE:
		Edge((filter.u == 0) ? 0 : imgData.w / filter.u, (filter.l == 0) ? 0 : imgData.h / filter.l, (filter.b == 0) ? 0 : imgData.w / filter.b, (filter.r == 0) ? 0 : imgData.h / filter.r);
		break;
	case BLUR:
		Blur((filter.u == 0) ? 0 : imgData.w / filter.u, (filter.l == 0) ? 0 : imgData.h / filter.l, (filter.b == 0) ? 0 : imgData.w / filter.b, (filter.r == 0) ? 0 : imgData.h / filter.r);
		break;
	}
}

void png_toolkit::Paint(Config* conf) {
	vector<Filter>* filters = conf->Fil();
	vector<Filter>::iterator i;
	for (i = filters->begin(); i != filters->end(); i++) {
		Paint_Filter(*i);
	}
}

Config::Config(string conf) {
	ifstream file(conf);
	string str;
	filters = new vector<Filter>;
	while (getline(file, str)) {
		string* s = new string[5];
		int j = 0;
		for (int i = 0; i < 5; i++) {
			s[i] = Read(str, &j);
		}
		Filter filter;
		filter.f_n = Name(s[0]);
		filter.u = atoi(s[1].c_str());
		filter.l = atoi(s[2].c_str());
		filter.b = atoi(s[3].c_str());
		filter.r = atoi(s[4].c_str());
		//filters->insert(filters->end(), filters->capacity(), filter);
		filters->push_back(filter);
	}
}

string Config::Read(string str, int* i) {
	int j = *i;
	string s;
	char ch = str[j];
	while (ch != ' ' && ch != '\0') {
		s = s + ch;
		j++;
		ch = str[j];
	}
	j++;
	*i = j;
	return s;
}

enum Fil_Name Config::Name(string str) {
	if (str == "Blackwhite") {
		return BLACK_WHITE;
	}
	if (str == "Red") {
		return RED;
	}
	if (str == "Threshold") {
		return THRESHOLD;
	}
	if (str == "Edge") {
		return EDGE;
	}
	if (str == "Blur") {
		return BLUR;
	}
	return ERROR;
}

vector<Filter>* Config::Fil() {
	return filters;
}