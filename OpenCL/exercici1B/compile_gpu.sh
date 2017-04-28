g++ -O3 main.cpp pattmatch_gpu.cpp -o main_gpu -Wall -W -ansi -pedantic -Dcimg_use_vt100 -I/usr/X11R6/include  -lm -L/usr/X11R6/lib -lpthread -lX11 -I/usr/local/cuda/include -L/usr/local/cuda/lib -lOpenCL

