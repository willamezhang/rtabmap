/*
Copyright (c) 2010-2016, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universite de Sherbrooke nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CORELIB_INCLUDE_RTABMAP_CORE_LASERSCAN_H_
#define CORELIB_INCLUDE_RTABMAP_CORE_LASERSCAN_H_

#include <rtabmap/core/Transform.h>

namespace rtabmap {

class LaserScan
{
public:
	enum Format{kUnknown=0,
		kXY=1,
		kXYI=2,
		kXYNormal=3,
		kXYINormal=4,
		kXYZ=5,
		kXYZI=6,
		kXYZRGB=7,
		kXYZNormal=8,
		kXYZINormal=9,
		kXYZRGBNormal=10};

	static int channels(Format format);
	static bool isScan2d(const Format & format);
	static bool isScanHasNormals(const Format & format);
	static bool isScanHasRGB(const Format & format);
	static bool isScanHasIntensity(const Format & format);
	static LaserScan backwardCompatibility(const cv::Mat & oldScanFormat, int maxPoints = 0, int maxRange = 0, const Transform & localTransform = Transform::getIdentity());

public:
	LaserScan();
	LaserScan(const cv::Mat & data, int maxPoints, float maxRange, Format format, const Transform & localTransform = Transform::getIdentity());

	const cv::Mat & data() const {return data_;}
	int maxPoints() const {return maxPoints_;}
	float maxRange() const {return maxRange_;}
	Format format() const {return format_;}
	Transform localTransform() const {return localTransform_;}

	bool isEmpty() const {return data_.empty();}
	int size() const {return data_.cols;}
	int dataType() const {return data_.type();}
	bool is2d() const {return isScan2d(format_);}
	bool hasNormals() const {return isScanHasNormals(format_);}
	bool hasRGB() const {return isScanHasRGB(format_);}
	bool hasIntensity() const {return isScanHasIntensity(format_);}
	bool isCompressed() const {return !data_.empty() && data_.type()==CV_8UC1;}
	LaserScan clone() const {return LaserScan(data_.clone(), maxPoints_, maxRange_, format_, localTransform_.clone());}

	int getIntensityOffset() const {return hasIntensity()?(is2d()?2:3):-1;}
	int getRGBOffset() const {return hasRGB()?(is2d()?2:3):-1;}
	int getNormalsOffset() const {return hasNormals()?(2 + (is2d()?0:1) + ((hasRGB() || hasIntensity())?1:0)):-1;}

	void clear() {data_ = cv::Mat();}

private:
	cv::Mat data_;
	int maxPoints_;
	float maxRange_;
	Format format_;
	Transform localTransform_;
};

}

#endif /* CORELIB_INCLUDE_RTABMAP_CORE_LASERSCAN_H_ */
