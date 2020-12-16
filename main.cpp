#include <iostream>
#include "png_toolkit.h"
#include"Filters2/Config.h"

int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!
    try
    {
        if (argc != 3)
            throw "Not enough arguments"; 

        Config* conf = new Config(argv[1]);
        png_toolkit studTool;
        studTool.load(argv[1]);
        studTool.Paint(conf);
        studTool.save(argv[2]);
    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }

    return 0;
}
