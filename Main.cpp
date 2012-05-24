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
 * \brief	Compares the two given sets of float vectors. It matches the
 *		first vector of the first set with the best fitting vector of
 *		the second set and then matches the second vector of the first
 *		set with the best fitting vector of the second set excluding
 *		the vector that has already been matched and so on.
 *		This method WON't find an optimal matching between the two
 *		sets of vectors. Better results may be achieved by heuristics.
 *
 * \param	descriptors1	The first set of vectors (="descriptors")
 * \param	descriptors2	The second set of vectors (="descriptors")
 * \param	descriptorSize	The length of every vector in the sets. 
 *
 * \return	The sum of all squared distances in the resulting matching.
 */
double matchDescriptors(vector<float> descriptors1, vector<float> descriptors2, const int descriptorSize)
{
	//This will hold the sum of all distances
	double result = 0;
	
	// less points to the smaller vector
	vector<float> &less = descriptors1.size() <  descriptors2.size() ? descriptors1 : descriptors2;

	// more points to the bigger vector
	vector<float> &more = descriptors1.size() >= descriptors2.size() ? descriptors1 : descriptors2;

	for (int l = 0; l < less.size(); l+= descriptorSize)
	{
		int bestMatchIndex = 0;
		double bestDistance = FLT_MAX;
		//search best match
		for (int m = 0; m < more.size(); m+= descriptorSize)
		{
			//calculate squared euclidean distance 
			double distance = 0;
			for (int v = 0; v < descriptorSize; v++)
			{
				distance += (less[l+v] - more[m+v]) * (less[l+v] - more[m+v]);
			}

			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestMatchIndex = m;
			}
		}
		
		result += bestDistance;
	
		//erase match to avoid collisions 
		more.erase(more.begin() + bestMatchIndex, more.begin() + bestMatchIndex + descriptorSize);	
	}
	return result;
}

/**
 * \brief	Compares two images using SURF features
 *
 * \param	img1	The fist image. Must be a 3 channel RGB image.
 * \param	img2	The second image. Must be a 3 channel RGB image.
 * \param	ht	The hessian Threshold used in the SURF algorithm
 *
 * \return	A value between 0 and 1 indicating the similarity of the
 * 		input images. 0 means no similarity and 1 means that both
 *		images are identical.
 */
double compareImagesSURF(cv::Mat &image1, cv::Mat &image2, double ht)
{
	//convert first image to gray scale
	cv::Mat img1;
	cv::cvtColor(image1, img1, CV_RGB2GRAY);
	
	//convert second image to gray scale
	cv::Mat img2;
	cv::cvtColor(image2, img2, CV_RGB2GRAY);
	
	//initialize SURF object	
	cv::SURF surf(ht);
	
	//calculate SURF features for the first image
	vector<cv::KeyPoint> keyPoints1;
	vector<float> descriptors1;
	surf(img1, cv::Mat(), keyPoints1, descriptors1);	
	
	//calculate SURF features for the second image
	vector<cv::KeyPoint> keyPoints2;
	vector<float> descriptors2;
	surf(img2, cv::Mat(), keyPoints2, descriptors2);	

	//compare features
	return matchDescriptors(descriptors1, descriptors2, surf.descriptorSize());
}

int main (int argc, char** argv)
{

	if (argc != 4)
	{
		//try 500 or 1000 for hessian threshold
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
