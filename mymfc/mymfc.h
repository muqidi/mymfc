
// mymfc.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
//#include "cv.h"
//#include "highgui.h"
#include "CvvImage.h"
#include <fstream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include<cstring>

using namespace cv;
using namespace std;

typedef list<CvRect> LISTRECT;

#define IMAGE_CHANNELS 3
// CmymfcApp: 
// 有关此类的实现，请参阅 mymfc.cpp
//


class NumTrainData
{
public:
	NumTrainData()
	{
		memset(data, 0, sizeof(data));
		result = -1;
	}
public:
	float data[64];
	int result;
};

#define featureLen  64

class CmymfcApp : public CWinApp
{
public:
	CmymfcApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CmymfcApp theApp;