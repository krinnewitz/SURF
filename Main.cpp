#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

/**
 * \file	Main.cpp
 * \brief 	This is an implementation of image comparison using 
 * 		SURF features. The main goal is to verify how good
 *		this method is when trying to find textures for given
 *		planes in 3d reconstructions.
 *
 * \author	Kim Oliver Rinnewitz, krinnewitz@uos.de
 */

using namespace std;

/**
 * \brief	Compares two images using SURF features
 *
 * \param	img1	The fist image
 * \param	img2	The second image
 *
 * \return	A value between 0 and 1 indicating the similarity of
 *		the input images. 0 means no similarity and 1 means
 *		that both images are identical.
 */
double compareImages(cv::Mat &img1, cv::Mat &img2)
{
	return 0;
}

int main (int argc, char** argv)
{

	if (argc != 3)
	{
		cout<<"Usage: "<<argv[0]<<" <filename1> <filename2>"<<endl;
		return EXIT_FAILURE;	
	}
	else
	{
		cv::Mat img1 = cv::imread(argv[1]);
		cv::Mat img2 = cv::imread(argv[2]);

		cout<<compareImages(img1, img2)<<endl;

		return EXIT_SUCCESS;
	}

}
