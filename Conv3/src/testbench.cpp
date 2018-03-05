//in this c++ file, we define the 1st convolution layer of Alexnet. Actually it's not just convolution, it's convolution together with
//Relu, we just compare the output of the convolution, if it's greater than 0, then we store it as output, if it's negative, we store it as 0.
//Also in caffe the Relu layer is inlined with the other layer.
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcnt1.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
//#include <chrono>
//#include "sds_lib.h"
#include "conv3.h"
//#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;
typedef float DataType;

//#define EPSILON 0.001

using namespace std;

void conv3(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
		DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH],
		DataType filter[FILTER_BATCH * INP_IMAGE_CHANNEL * FILTER_SIZE * FILTER_SIZE],
		DataType bias[FILTER_BATCH]);


//main function used to test the functionality of the kernel.
int main()
{
  //int input_vector_size_bytes = sizeof(float) * INP_IMAGE_CHANNEL * INP_IMAGE_SIZE * INP_IMAGE_SIZE;
  //float *inp_image = (float *) malloc(input_vector_size_bytes);
	//initialize the "inp_image" array and print them in order to check it

	//int group =2;
    ifstream inp_file("/home/junnan/Vivado_HLS/clock_problem/inp_conv3.txt");
    DataType *inp_image;
//    inp_image = (DataType *)sds_alloc( INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL * sizeof(DataType));
    inp_image = (DataType *)malloc( INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL * sizeof(DataType));
	if(inp_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i=0; i<INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL; i++)
		{
			inp_file >> inp_image[i];
			//cout<< "inp_image[" << i << "] = " << inp_image[i] << endl;
		}
		inp_file.close();
	}
    cout << "inp_image[0] = " << inp_image[0] << endl;
/*
    DataType filter[256][FILTER_CHANNEL][FILTER_SIZE][FILTER_SIZE] = {
                                #include "conv2Weights.txt"
       	  	  	  	  	  	  	  	  	  	  };
       	  	  	  	  	  	  	  	  	  	  */


	ifstream filter_file("/home/junnan/Vivado_HLS/clock_problem/conv3Weights.txt");
	    DataType *filter;
	    filter = (DataType *)malloc( FILTER_BATCH*INP_IMAGE_CHANNEL*FILTER_SIZE*FILTER_SIZE * sizeof(DataType));
		if(filter_file.is_open())
		{
			cout << "can open the text file" << endl;


			for (int i=0; i<FILTER_BATCH*INP_IMAGE_CHANNEL*FILTER_SIZE*FILTER_SIZE; i++)
			{
				filter_file >> filter[i];
				//cout<< "inp_image[" << i << "] = " << inp_image[i];
			}
			filter_file.close();
		}
	    cout << "filter[0] = " << filter[0] << endl;
	    cout << "filter[1] = " << filter[1] << endl;
	    cout << "filter[2] = " << filter[2] << endl;

		ifstream bias_file("/home/junnan/Vivado_HLS/clock_problem/conv3Bias.txt");
		    DataType *bias;
		    bias = (DataType *)malloc( FILTER_BATCH * sizeof(DataType));
			if(bias_file.is_open())
			{
				cout << "can open the text file" << endl;


				for (int i=0; i<FILTER_BATCH; i++)
				{
					bias_file >> bias[i];
					//cout<< "inp_image[" << i << "] = " << inp_image[i];
				}
				bias_file.close();
			}
		    cout << "bias[0] = " << bias[0] << endl;
		    cout << "bias[1] = " << bias[1] << endl;
		    cout << "bias[2] = " << bias[2] << endl;

/*
  float  inp_image[]  = {
                         #include "inp_image.txt"
                      };
*/
	//DataType *out_image = (DataType *)sds_alloc(OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH * sizeof(DataType));
	DataType *out_image = (DataType *)malloc(OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH * sizeof(DataType));
//  float *out_image = (float *)malloc(OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH * sizeof(float));
//  float out_image[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH];
  cout << "Start calling the conv1 HW function" << endl;

//  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  //call the "conv1" function using the "inp_image" argument, it returns the output in the "out_image" array
  conv3(inp_image, out_image, filter, bias);

//  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  cout << "After calling the conv1 HW function" << endl;
//  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
//  cout << "duration = " << duration << endl;
  //free all the dynamically allocated memory
//  sds_free(inp_image);
  //sds_free(inp_image);
  //sds_free(filter);
  free(inp_image);
  //free(filter);

  //dump the output image into a txt file "out_image.txt"
  ofstream data("/home/junnan/Vivado_HLS/clock_prolem/out_image.txt");
  for (int k = 0; k < OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*FILTER_BATCH; k++)
    {
      data << out_image[k] << "\n";
      //if(out_image[k] != 0)
      //cout << "out_image[" << k << "] = " << out_image[k] << endl;
    }


  const DataType out_img[] = {
                         #include "out_conv3.txt"
                       };
      DataType big_diff = 0;
      DataType diff[OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*FILTER_BATCH];
  for (int i=0; i<OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*FILTER_BATCH; i++){
	   diff[i] = out_img[i]-out_image[i];
	  if (diff[i] < 0)
		  diff[i] = -diff[i];

	  if (diff[i] > big_diff){
		 big_diff = diff[i];
	  }
  }
  cout << "big_diff = " << big_diff << endl;
  cout << "out_img[11] = " << out_img[11] << endl;
  cout << "out_image[11] = " << out_image[11] << endl;

/*
  for (int i = 0; i < OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*FILTER_BATCH; i++){
    if ((out_img[i] - out_image[i] >= 0.001) || (out_img[i] - out_image[i] <= -0.001)){
    //if (fabs(out_img[i] - out_image[i]) > EPSILON){
    cout << "out_img[" << i << "] = " <<  out_img[i] << endl;
    cout << "out_image[" << i << "] = " << out_image[i] << endl;
    cout << "Functionality failed" << endl;
         }
}
*/

//  cout << "Functionality pass" << endl;
  //sds_free(out_image);
  free(out_image);
  return 0;
}
