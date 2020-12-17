#pragma once
#include"png_toolkit.h"
#include"stb_image_write.h"
#include"Config.h"
#include<algorithm>

class Filters {
public:

	Filters(image_data data) {
		imgData = data;
	}

    bool save(std::string const& pictureName);

    void Black_White(int start_x, int start_y, int finish_x, int finish_y);
    void Red(int start_x, int start_y, int finish_x, int finish_y);
    void Threshold(int start_x, int start_y, int finish_x, int finish_y);
    void Edge(int start_x, int start_y, int finish_x, int finish_y);
    void Blur(int start_x, int start_y, int finish_x, int finish_y);
    stbi_uc Matrix5(int i, int j, int start_x, int start_y, int finish_x, int finish_y);
    stbi_uc Matrix3_Edge(int i, int j, int start_x, int start_y, int finish_x, int finish_y);
    stbi_uc Matrix3_Blur(int i, int j, int s, int start_x, int start_y, int finish_x, int finish_y);
    void Paint(Config* conf);
    void Paint_Filter(Filter filter);

private:
	image_data imgData;
};