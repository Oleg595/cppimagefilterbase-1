#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
#include<fstream>
#include<vector>
#include "stb_image.h"
using namespace std;
//#include"Filters2/Config.h"

struct Filter {
    enum Fil_Name f_n;
    int u;
    int l;
    int b;
    int r;
};

class Config {
public:
    Config(string conf);
    vector<Filter>* Fil();
private:
    vector<Filter>* filters;
    string Read(string str, int* i);
    enum Fil_Name Name(string str);
};

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

enum Fil_Name {
    BLACK_WHITE,
    RED,
    THRESHOLD,
    BLUR,
    EDGE,
    ERROR
};

#endif // PNG_TOOLKIT_H

