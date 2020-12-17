#include"Filters.h"

void Filters::Black_White(int start_x, int start_y, int finish_x, int finish_y) {
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			stbi_uc x = (3 * imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] + 6 * imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] + imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2]) / 10;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = x;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = x;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = x;
		}
	}
}

stbi_uc Filters::Matrix3_Edge(int i, int j, int start_x, int start_y, int finish_x, int finish_y) {
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

void Filters::Edge(int start_x, int start_y, int finish_x, int finish_y) {
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

stbi_uc Filters::Matrix3_Blur(int i, int j, int s, int start_x, int start_y, int finish_x, int finish_y) {
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

void Filters::Blur(int start_x, int start_y, int finish_x, int finish_y) {
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

stbi_uc Filters::Matrix5(int i, int j, int start_x, int start_y, int finish_x, int finish_y) {
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

void Filters::Threshold(int start_x, int start_y, int finish_x, int finish_y) {
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

void Filters::Red(int start_x, int start_y, int finish_x, int finish_y) {
	for (int i = start_y; i < finish_y; i++) {
		for (int j = start_x; j < finish_x; j++) {
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] = 255;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 1] = 0;
			imgData.pixels[imgData.compPerPixel * (i * imgData.w + j) + 2] = 0;
		}
	}
}

void Filters::Paint_Filter(Filter filter) {
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

void Filters::Paint(Config* conf) {
	vector<Filter>* filters = conf->Fil();
	vector<Filter>::iterator i;
	for (i = filters->begin(); i != filters->end(); i++) {
		Paint_Filter(*i);
	}
}

bool Filters::save(const std::string& pictureName)
{
	return stbi_write_png(pictureName.c_str(),
		imgData.w, imgData.h,
		imgData.compPerPixel,
		imgData.pixels, 0) != 0;
}