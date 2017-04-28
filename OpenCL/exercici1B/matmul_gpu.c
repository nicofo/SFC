// Host code
// Matrices are stored in row-major order:
// M(row, col) = *(M.elements + row * M.width + col)

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "CL/cl.h"

#define BLOCK_SIZE 32

// Source will be read from disk
#define MAX_SOURCE_SIZE (0x100000)

// Matrix multiplication - Host code
// Matrix dimensions are assumed to be multiples of BLOCK_SIZE
void MatMulHost(
    float *A, int A_width, int A_height,
    float *B, int B_width, int B_height, 
    float *C, int C_width, int C_height,
    cl_context context,
    cl_kernel matMulKernel,
    cl_command_queue queue)
{
  // Copy A and B to device memory
  cl_mem clA, clB, clC;

  // To profile the execution time
  cl_event event;
  cl_ulong time_start, time_end;
  double total_time;

  size_t size = A_width * A_height * sizeof(float);
  clA = clCreateBuffer(
      context, 
      CL_MEM_READ_ONLY,
      size, 
      NULL, 
      NULL);

  size = B_width * B_height * sizeof(float);
  clB = clCreateBuffer(
      context, 
      CL_MEM_READ_ONLY,
      size, 
      NULL, 
      NULL);

  // Allocate C in device memory
  size = C_width * C_height * sizeof(float);
  clC = clCreateBuffer(
      context,
      CL_MEM_WRITE_ONLY, 
      size, 
      NULL, 
      NULL);

  // Transfer data to device
  size = A_width * A_height * sizeof(float);
  clEnqueueWriteBuffer(
        queue, 
        clA, 
        CL_FALSE, 
        0, 
        size,                         
        A, 
        0, 
        NULL, 
        NULL);

  size = B_width * B_height * sizeof(float);
  clEnqueueWriteBuffer(
        queue, 
        clB, 
        CL_FALSE, 
        0, 
        size,                         
        B, 
        0, 
        NULL, 
        NULL);

  // Invoke kernel
  cl_uint i = 0;
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(int), 
      (void*)&A_width);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(int), 
      (void*)&A_height);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(cl_mem), 
      (void*)&clA);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(int), 
      (void*)&B_width);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(B_height), 
      (void*)&B_height);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(cl_mem), 
      (void*)&clB);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(int), 
      (void*)&C_width);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(int), 
      (void*)&C_height);
  clSetKernelArg(
      matMulKernel, 
      i++, 
      sizeof(cl_mem), 
      (void*)&clC);

  clFinish(queue);

  size_t localWorkSize[]  = { BLOCK_SIZE, BLOCK_SIZE };
  size_t globalWorkSize[] = { C_width, C_height };

  clEnqueueNDRangeKernel(
      queue, 
      matMulKernel, 
      2, 
      NULL,
      globalWorkSize, 
      localWorkSize,
      0, 
      NULL, 
      &event);

  clWaitForEvents(1, &event);

  clGetEventProfilingInfo(event,  
      CL_PROFILING_COMMAND_START, 
      sizeof(time_start), 
      &time_start, 
      NULL);

  clGetEventProfilingInfo(event, 
      CL_PROFILING_COMMAND_END, 
      sizeof(time_end), 
      &time_end, 
      NULL);

  total_time = time_end - time_start;
  printf("Execution kernel time in milliseconds = %0.3f ms\n", (total_time / 1000000.0) );

  // Read C from device memory
  size = C_width * C_height * sizeof(float);
  clEnqueueReadBuffer(
      queue, 
      clC, 
      CL_TRUE, 
      0, 
      size,
      C, 
      0, 
      0, 
      0);

  // Free device memory
  clReleaseMemObject(clA);
  clReleaseMemObject(clB);
  clReleaseMemObject(clC);
}

int main() {
  // This code executes on the OpenCL host
  FILE *fp;

  char *source_str;
  size_t source_size;

  fp = fopen("matmul_kernel.cl", "r");
  if (!fp) {
    fprintf(stderr, "Failed to load kernel.\n");
    exit(1);
  }
  source_str = (char*)malloc(MAX_SOURCE_SIZE);
  source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
  fclose( fp );

  // Host data
  float *A = NULL;  // Input array
  float *B = NULL;  // Input array
  float *C = NULL;  // Output array

  // Size of the matrix
  const int size = 2048;
  const int elements = size * size;

  // Compute the size of the data 
  size_t datasize = sizeof(float) * elements;

  // Allocate space for input/output data
  A = (float *) malloc(datasize);
  B = (float *) malloc(datasize);
  C = (float *) malloc(datasize);

  // Initialize the input data
  for(int i = 0; i < elements; i++) {
    A[i] = 1;
    B[i] = 1;
  }

  // Use this to check the output of each API call
  cl_int status;  

  //-----------------------------------------------------
  // Discover and initialize the platforms
  //-----------------------------------------------------

  cl_uint numPlatforms = 0;
  cl_platform_id *platforms = NULL;

  // Use clGetPlatformIDs() to retrieve the number of 
  // platforms
  status = clGetPlatformIDs(0, NULL, &numPlatforms);

  // Allocate enough space for each platform
  platforms =   
    (cl_platform_id*)malloc(
	numPlatforms*sizeof(cl_platform_id));

  // Fill in platforms with clGetPlatformIDs()
  status = clGetPlatformIDs(numPlatforms, platforms, 
      NULL);

  //-----------------------------------------------------
  // Discover and initialize the devices
  //----------------------------------------------------- 

  cl_uint numDevices = 0;
  cl_device_id *devices = NULL;

  // Use clGetDeviceIDs() to retrieve the number of 
  // devices present
  status = clGetDeviceIDs(
      platforms[0], 
      CL_DEVICE_TYPE_ALL, 
      0, 
      NULL, 
      &numDevices);

  // Allocate enough space for each device
  devices = 
    (cl_device_id*)malloc(
	numDevices*sizeof(cl_device_id));

  // Fill in devices with clGetDeviceIDs()
  status = clGetDeviceIDs(
      platforms[0], 
      CL_DEVICE_TYPE_ALL,        
      numDevices, 
      devices, 
      NULL);

  //-----------------------------------------------------
  // Create a context
  //----------------------------------------------------- 

  cl_context context = NULL;

  // Create a context using clCreateContext() and 
  // associate it with the devices
  context = clCreateContext(
      NULL, 
      numDevices, 
      devices, 
      NULL, 
      NULL, 
      &status);

  //-----------------------------------------------------
  // Create a command queue
  //----------------------------------------------------- 

  cl_command_queue cmdQueue;

  // Create a command queue using clCreateCommandQueue(),
  // and associate it with the device you want to execute 
  // on
  cmdQueue = clCreateCommandQueue(
      context, 
      devices[0], 
      CL_QUEUE_PROFILING_ENABLE, 
      &status);

  //-----------------------------------------------------
  // Create and compile the program
  //----------------------------------------------------- 

  // Create a program using clCreateProgramWithSource()
  cl_program program = clCreateProgramWithSource(
      context, 
      1, 
      (const char**)&source_str,                                 
      (const size_t *)&source_size, 
      &status);

  // Build (compile) the program for the devices with
  // clBuildProgram()
  status = clBuildProgram(
      program, 
      numDevices, 
      devices, 
      NULL, 
      NULL, 
      NULL);

  if (status != CL_SUCCESS) {
    cl_build_status build_status;
    clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);

    char *build_log;
    size_t ret_val_size;
    clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);

    build_log = (char *) malloc(ret_val_size+1);
    clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
    build_log[ret_val_size] = '\0';
    printf("BUILD LOG: \n %s", build_log);
    free(build_log);
    exit(1);
  }

  //-----------------------------------------------------
  // Create the kernel
  //----------------------------------------------------- 

  cl_kernel kernel = NULL;

  // Use clCreateKernel() to create a kernel from the 
  // vector addition function (named "MatMulKernel")
  kernel = clCreateKernel(program, "MatMulKernel", &status);

  //-----------------------------------------------------
  // Call matrix multiplication
  //----------------------------------------------------- 

  MatMulHost(A, size, size,
      B, size, size,
      C, size, size,
      context, kernel, cmdQueue);
  
  //-----------------------------------------------------
  // Result is in C
  //----------------------------------------------------- 

  float dif, max = 0;
  int good = 1;
  for(int i = 0; i < elements; i++)
  {
    dif = fabs(C[i] - size);
    if (dif > max)
      max = dif; 
  }

  printf("Maxima diferencia: %f\n", max);

  free(A);
  free(B);
  free(C);
}

