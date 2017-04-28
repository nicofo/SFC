#include <iostream>
#include "CImg.h"

#include "common.h"

using namespace std;
using namespace cimg_library;

CImg<float> pattern_matching(CImg<unsigned char> &img, CImg<unsigned char> &pat)
{
    struct timeval start, end;
    float mtime, seconds, useconds;

    CImg<float> output(img.width()-PADDING,img.height()-PADDING,1,1,0);

    gettimeofday(&start, NULL);

    for(int i = 0; i < img.height()-PADDING; i++){
        for(int j = 0; j < img.width()-PADDING; j++){
            float sum = 0.0;
            for(int k = 0; k < pat.height(); k++){
                for(int l = 0; l < pat.width(); l++){

                    float value = (float) img(j+l,i+k) - (float) pat(l,k);
                    sum += value * value;
                }
            }
            output(j,i) = sum / (float) (MIDAP*MIDAP);
        }
    }

    gettimeofday(&end, NULL);	
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds / 1000.0);
    cout << "Elapsed time of pattmatch function at CPU = " << mtime << "ms" << endl;

    return output;
}


