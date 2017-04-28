#include <iostream>
#include "CImg.h"

#include "common.h"

using namespace std;
using namespace cimg_library;

CImg<float> pattern_matching(CImg<unsigned char> &img, CImg<unsigned char> &pat);

CImg<unsigned char> setBorder(CImg<unsigned char> &gray)
{
    CImg<unsigned char> gray_border(gray.width()+PADDING,gray.height()+PADDING,1,1,0);

    unsigned char *grayPtr = gray._data;
    unsigned char *bordPtr = gray_border._data;

    for(int i = 0; i < gray.height(); i++)
        for(int j = 0; j < gray.width(); j++)
        {
            int offset_gray = i * gray.width() + j;
            int offset_bord = i * gray_border.width() + j;

            bordPtr[offset_bord] = grayPtr[offset_gray];
        }

    return gray_border;
}

int main(int argc, char** argv )
{
    if( argc != 4)
    {
        cout <<" Usage: " << argv[0] << " <image_in> <pattern_in> <image_out>" << endl;
        return -1;
    }

    CImg<unsigned char> image(argv[1]);   
    CImg<unsigned char> pattern(argv[2]);

    if ((PADDING + 1) != MIDAP) {
        cout << "Error: PADDING + 1 != MIDAP" << endl;
        exit(1);
    }

    if ((pattern.width() != MIDAP) || (pattern.height() != MIDAP)) {
        cout << "Pattern has not size 16 x 16" << endl;
        exit(1);
    }

    CImg<unsigned char> gray;

    int color = image.spectrum(); 
    switch(color)
    {
        case 1:
            gray = image;
            break;
        case 3: 
            gray = image.get_RGBtoYCbCr().get_channel(0);
            break;
        default:
            cout << "Error: image is not graylevel nor color image" << endl;
    }

    CImg<unsigned char> gray_border = setBorder(gray);
    CImg<float> out = pattern_matching(gray_border, pattern);
    CImg<unsigned char> norm_out= out.get_normalize(0, 255);

    norm_out.save(argv[3]);

    return 0;
}
