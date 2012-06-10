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
 * \brief	Matches the two given sets of feature descriptors and 
 *		calculates a value representing the similarity of the 
 *		associated images. This is done by considering only 
 *		"good" matches i.e. matches with a small distance.
 *
 * \param 	descriptors1	A matrix holding all descriptors of the
 *				first image
 * \param	descriptors2	A matrix holding all descriptors of the
 *				second image
 *
 * \return	The sum of all squared distances in the resulting matching.
 */
float matchDescriptors(cv::Mat descriptors1, cv::Mat descriptors2)
{
	float result = 0;
	
	//calculate matching
	cv::FlannBasedMatcher matcher;
	vector< cv::DMatch > matches;
	matcher.match( descriptors1, descriptors2, matches);

	//search best match
	double minDist = 100;
	for (int i = 0; i < matches.size(); i++)
	{ 
		if(matches[i].distance < minDist) minDist = matches[i].distance;
	}

	//Calculate result. Only good matches are considered.
	int numGoodMatches = 0;
	for( int i = 0; i < matches.size(); i++ )
	{ 
		if(matches[i].distance <= 2 * minDist)
		{
			result += matches[i].distance * matches[i].distance;
			numGoodMatches++;
		}
	}
	result /= numGoodMatches;
	return result;
}

/**
 * \brief	Compares two images using SURF features
 *
 * \param	img1	The fist image. Must be a 3 channel RGB image.
 * \param	img2	The second image. Must be a 3 channel RGB image.
 * \param	ht	The hessian Threshold used in the SURF algorithm
 *
 * \return	A value indicating the distance between the both images.
 *		0 means that both images are identical. The higher the
 *		distance, the lower the similarity of the images.
 */
double compareImagesSURF(cv::Mat &image1, cv::Mat &image2, double ht)
{
	//convert first image to gray scale
	cv::Mat img1;
	cv::cvtColor(image1, img1, CV_RGB2GRAY);
	
	//convert second image to gray scale
	cv::Mat img2;
	cv::cvtColor(image2, img2, CV_RGB2GRAY);
	
	//initialize SURF objects
	cv::SurfFeatureDetector detector(ht);
	cv::SurfDescriptorExtractor extractor;

	vector<cv::KeyPoint> keyPoints1, keyPoints2;
	cv::Mat descriptors1, descriptors2;

	//calculate SURF features for the first image
	detector.detect( img1, keyPoints1 );
	extractor.compute( img1, keyPoints1, descriptors1 );

	//calculate SURF features for the second image
	detector.detect( img2, keyPoints2 );
	extractor.compute( img2, keyPoints2, descriptors2 );

	//compare features
	float result = FLT_MAX;
	if (keyPoints1.size() > 0 && keyPoints2.size() > 0)
	{
		result = matchDescriptors(descriptors1, descriptors2);
	}

	return result;
}

int main (int argc, char** argv)
{

	if (argc != 4)
	{
		//try 100, 500 or 1000 for hessian threshold
		cout<<"Usage: "<<argv[0]<<" <filename1> <filename2> <hessianThreshold>"<<endl;
		return EXIT_FAILURE;	
	}
	else
	{
		cv::Mat img1 = cv::imread(argv[1]);
		cv::Mat img2 = cv::imread(argv[2]);

		cout<<compareImagesSURF(img1, img2, atof(argv[3]))<<endl;

		return EXIT_SUCCESS;
	}

}
