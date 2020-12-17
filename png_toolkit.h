#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
#include<fstream>
#include<vector>
#include "stb_image.h"
#include"Config.h"
using namespace std;

struct image_data
{
    stbi_uc *pixels;
    int w, h;
    int compPerPixel;
};

class png_toolkit
{
public:
    enum class Error {
        WrongSize,
        WrongFormat,
        Ok
    };

    png_toolkit();
    ~png_toolkit();
    bool load( std::string const &pictureName );
    bool save( std::string const &pictureName );
    image_data getPixelData( void ) const;
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

#endif // PNG_TOOLKIT_H
