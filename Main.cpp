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


int main (int argc, char** argv)
{

	if (argc != 3)
	{
		cout<<"Usage: "<<argv[0]<<" <filename> <minimal pattern size>"<<endl;
		return EXIT_FAILURE;	
	}
	else
	{
		cv::Mat src = cv::imread(argv[1]);

		return EXIT_SUCCESS;
	}

}
