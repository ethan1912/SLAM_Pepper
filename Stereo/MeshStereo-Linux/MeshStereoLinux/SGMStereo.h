#pragma once

#include "png++/png.hpp"

class SGMStereo {
public:
	SGMStereo();

	void setDisparityTotal(const int disparityTotal);
	void setDisparityFactor(const double disparityFactor);
	void setDataCostParameters(const int sobelCapValue,
							   const int censusWindowRadius,
							   const double censusWeightFactor,
							   const int aggregationWindowRadius);
	void setSmoothnessCostParameters(const int smoothnessPenaltySmall, const int smoothnessPenaltyLarge);
	void setConsistencyThreshold(const int consistencyThreshold);

	void compute(const png::image<png::rgb_pixel>& leftImage,
				 const png::image<png::rgb_pixel>& rightImage,
				 float* disparityImage, float* disparityImageRight);

public:
	void initialize(const png::image<png::rgb_pixel>& leftImage, const png::image<png::rgb_pixel>& rightImage);
	void setImageSize(const png::image<png::rgb_pixel>& leftImage, const png::image<png::rgb_pixel>& rightImage);
	void allocateDataBuffer();
	void freeDataBuffer();
	void computeCostImage(const png::image<png::rgb_pixel>& leftImage, const png::image<png::rgb_pixel>& rightImage);
	void convertToGrayscale(const png::image<png::rgb_pixel>& leftImage,
							const png::image<png::rgb_pixel>& rightImage,
							unsigned char* leftGrayscaleImage,
							unsigned char* rightGrayscaleImage) const;
	void computeLeftCostImage(const unsigned char* leftGrayscaleImage, const unsigned char* rightGrayscaleImage);
	void computeCappedSobelImage(const unsigned char* image, const bool horizontalFlip, unsigned char* sobelImage) const;
	void computeCensusImage(const unsigned char* image, int* censusImage) const;
	void calcTopRowCost(unsigned char*& leftSobelRow, int*& leftCensusRow,
						unsigned char*& rightSobelRow, int*& rightCensusRow,
						unsigned short* costImageRow);
	void calcRowCosts(unsigned char*& leftSobelRow, int*& leftCensusRow,
					  unsigned char*& rightSobelRow, int*& rightCensusRow,
					  unsigned short* costImageRow);
	void calcPixelwiseSAD(const unsigned char* leftSobelRow, const unsigned char* rightSobelRow);
	void calcHalfPixelRight(const unsigned char* rightSobelRow);
	void addPixelwiseHamming(const int* leftCensusRow, const int* rightCensusRow);
	void computeRightCostImage();
	void performSGM(unsigned short* costImage, unsigned short* disparityImage);
	void speckleFilter(const int maxSpeckleSize, const int maxDifference, unsigned short* image) const;
	void enforceLeftRightConsistency(unsigned short* leftDisparityImage, unsigned short* rightDisparityImage) const;


	// Parameter
	int disparityTotal_;
	double disparityFactor_;
	int sobelCapValue_;
	int censusWindowRadius_;
	double censusWeightFactor_;
	int aggregationWindowRadius_;
	int smoothnessPenaltySmall_;
	int smoothnessPenaltyLarge_;
	int consistencyThreshold_;

	// Data
	int width_;
	int height_;
	int widthStep_;
	unsigned short* leftCostImage_;
	unsigned short* rightCostImage_;
	unsigned char* pixelwiseCostRow_;
	unsigned short* rowAggregatedCost_;
	unsigned char* halfPixelRightMin_;
	unsigned char* halfPixelRightMax_;
	int pathRowBufferTotal_;
	int disparitySize_;
	int pathTotal_;
	int pathDisparitySize_;
	int costSumBufferRowSize_;
	int costSumBufferSize_;
	int pathMinCostBufferSize_;
	int pathCostBufferSize_;
	int totalBufferSize_;
	short* sgmBuffer_;
};
