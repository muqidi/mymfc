
// mymfcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mymfc.h"
#include "mymfcDlg.h"
#include "afxdialogex.h"
#include <list>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int IMAGE_WIDTH = 0;
int  IMAGE_HEIGHT = 0;



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CPoint M_point;
	bool isDraw;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_Cancel, &CAboutDlg::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CmymfcDlg 对话框



CmymfcDlg::CmymfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmymfcDlg::IDD, pParent)
	, TheImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmymfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmymfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CmymfcDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EdgeDetect, &CmymfcDlg::OnBnClickedEdgedetect)
//	ON_BN_CLICKED(IDCANCEL, &CmymfcDlg::OnBnClickedCancel)
//	ON_BN_CLICKED(IDC_ReadImg, &CmymfcDlg::OnBnClickedReadimg)
	ON_BN_CLICKED(IDC_TRAIN, &CmymfcDlg::OnBnClickedTrain)
//	ON_BN_CLICKED(IDC_BUTTON1, &CmymfcDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_TEST, &CmymfcDlg::OnBnClickedTest)
ON_BN_CLICKED(IDC_BUTTON2, &CmymfcDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON1, &CmymfcDlg::OnBnClickedButton1)
ON_MESSAGE(WM_SYSTEMSET, OnMyMessage)
END_MESSAGE_MAP()


// CmymfcDlg 消息处理程序

BOOL CmymfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	CDC* pDC = GetDlgItem(IDC_ShowImg)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(IDC_ShowImg)->GetClientRect(&rect);
	IMAGE_WIDTH = rect.right - rect.left;			// 求出图片控件的宽和高
	 IMAGE_HEIGHT = rect.bottom - rect.top;
	ReleaseDC(pDC);
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);

	//CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	//pProgCtrl->SetRange(0, 100);//设置进度条范围

	//pProgCtrl->SetStep(10);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CmymfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmymfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();						// 重绘对话框
		CDialog::UpdateWindow();				// 更新windows窗口，如果无这步调用，图片显示还会出现问题
		ShowImage(TheImage, IDC_ShowImg);		// 重绘图片函数
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmymfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CmymfcDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
// TODO: 处理用户自定义消息

		
		IplImage* ipl = cvLoadImage("D:\\1.bmp", 1);	// 读取图片、缓存到一个局部变量 ipl 中
		if (!ipl)									// 判断是否成功读取图片
			return 0;
		if (TheImage)								// 对上一幅显示的图片数据清零
			cvZero(TheImage);

		ResizeImage(ipl, IDC_ShowImg);	// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
		ShowImage(TheImage, IDC_ShowImg);			// 调用显示图片函数	
		cvReleaseImage(&ipl);
		return 0;
}


//交换高低端,因为数据是大段模式存储的
void swapBuffer(char* buf)
{
	char temp;
	temp = *(buf);
	*buf = *(buf + 3);
	*(buf + 3) = temp;

	temp = *(buf + 1);
	*(buf + 1) = *(buf + 2);
	*(buf + 2) = temp;
}

//查找边缘src原图像，dst根据边缘切割的小图像
void GetROI(Mat& src, Mat& dst)
{
	int left, right, top, bottom;
	left = src.cols;
	right = 0;
	top = src.rows;
	bottom = 0;

	//Get valid area
	for (int i = 0; i<src.rows; i++)
	{
		for (int j = 0; j<src.cols; j++)
		{
			if (src.at<uchar>(i, j) > 0)
			{
				if (j<left) left = j;
				if (j>right) right = j;
				if (i<top) top = i;
				if (i>bottom) bottom = i;
			}
		}
	}

	//Point center;
	//center.x = (left + right) / 2;
	//center.y = (top + bottom) / 2;

	int width = right - left;
	int height = bottom - top;
	int len = (width < height) ? height : width;

	//Create a squre
	dst = Mat::zeros(len, len, CV_8UC1);

	//Copy valid data to squre center
	Rect dstRect((len - width) / 2, (len - height) / 2, width, height);
	Rect srcRect(left, top, width, height);
	Mat dstROI = dst(dstRect);
	Mat srcROI = src(srcRect);
	srcROI.copyTo(dstROI);
}


int OpenFile(CString& fileName, CString& labelFileName){
	CFileDialog dlg(
		TRUE, _T("*.idx3-ubyte"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.idx3-ubyte)|*.idx3-ubyte"), NULL
		);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("Open test Image");	// 打开文件对话框的标题名
	if (dlg.DoModal() != IDOK)					// 判断是否获得图片
		return -1;

	fileName = dlg.GetPathName();			// 获取图片路径

	CFileDialog dlg1(
		TRUE, _T("*.idx1-ubyte"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("label files (*.idx1-ubyte)|*.idx1-ubyte"), NULL
		);										// 选项图片的约定
	dlg1.m_ofn.lpstrTitle = _T("Open test Label");	// 打开文件对话框的标题名
	if (dlg1.DoModal() != IDOK)					// 判断是否获得图片
		return -1;

	labelFileName = dlg1.GetPathName();			// 获取图片路径
	return 0;
}


void CmymfcDlg::AddTestImgToDisplay(Mat dst){

	CDC* pDC = GetDlgItem(IDC_TESTIMG)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(IDC_TESTIMG)->GetClientRect(&rect);
	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	ReleaseDC(pDC);
	Mat img = Mat::zeros(rh, rw, CV_8UC3);
	resize(dst, img, img.size());
	testImg.push_back(img);
}

//读入MNIST数据 flag 0:train  1:test
int CmymfcDlg::ReadTrainData(vector<NumTrainData>& buffer, int maxCount,int istrain)
{
	CString fileName;			// 获取图片路径
	CString labelFileName;			// 获取图片路径
	int flag = OpenFile(fileName, labelFileName);
	if (flag == -1){
		return -1;
	}

	ifstream lab_ifs(labelFileName, ios_base::binary);
	ifstream ifs(fileName, ios_base::binary);

	if (ifs.fail() == true)
		return -1;

	if (lab_ifs.fail() == true)
		return -1;

	//Read train data number and image rows / cols
	char magicNum[4], ccount[4], crows[4], ccols[4];
	ifs.read(magicNum, sizeof(magicNum));
	ifs.read(ccount, sizeof(ccount));
	ifs.read(crows, sizeof(crows));
	ifs.read(ccols, sizeof(ccols));

	int count, rows, cols;
	swapBuffer(ccount);
	swapBuffer(crows);
	swapBuffer(ccols);

	memcpy(&count, ccount, sizeof(count));
	memcpy(&rows, crows, sizeof(rows));
	memcpy(&cols, ccols, sizeof(cols));

	//Just skip label header
	lab_ifs.read(magicNum, sizeof(magicNum));
	lab_ifs.read(ccount, sizeof(ccount));

	//Create source and show image matrix
	Mat src = Mat::zeros(rows, cols, CV_8UC1);
	Mat temp = Mat::zeros(8, 8, CV_8UC1);
	Mat  dst;
	char label = 0;
	//Scalar templateColor(255, 0, 255);

	NumTrainData rtd;

	//int loop = 1000;
	int total = 0;

	while (!ifs.eof())
	{
		if (total >= count)
			break;
		total++;
		CString str;
		str.Format("正在读取文件：%d/%d", total, count);
		GetDlgItem(IDC_PTEXT)->SetWindowText(str);
		//Read label
		lab_ifs.read(&label, 1);
		label = label + '0';
		//Read source data
		ifs.read((char*)src.data, rows * cols);
		GetROI(src, dst);
		if (istrain == 1){
			AddTestImgToDisplay(dst);
		}
		rtd.result = label;
		resize(dst, temp, temp.size());
		for (int i = 0; i<8; i++)
		{
			for (int j = 0; j<8; j++)
			{
				rtd.data[i * 8 + j] = temp.at<uchar>(i, j);
			}
		}
		buffer.push_back(rtd);
		maxCount--;
		if (maxCount == 0)
			break;
	}
	if (istrain == 1){
		i = testImg.begin();
	}
	ifs.close();
	lab_ifs.close();
	return 0;
}

//train集合的数据分成image和label两部分
void CmymfcDlg::VectorToMatIAL(vector<NumTrainData>& trainData, Mat& data, Mat& res,string name){
	
	int testCount = trainData.size();
	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	data = Mat::zeros(testCount, featureLen, CV_32FC1);
	res = Mat::zeros(testCount, 1, CV_32SC1);

	for (int i = 0; i< testCount; i++)
	{

		NumTrainData td = trainData.at(i);
		if (name == "SVM"){
			memcpy(m.data, td.data, featureLen*sizeof(float));
			normalize(m, m);
			memcpy(data.data + i*featureLen*sizeof(float), m.data, featureLen*sizeof(float));
		}
		else memcpy(data.data + i*featureLen*sizeof(float), td.data, featureLen*sizeof(float));
		res.at<unsigned int>(i, 0) = td.result;
	}
}


//随机决策森林——训练
void CmymfcDlg::newRtStudy(vector<NumTrainData>& trainData)
{
	Mat data, res;
	VectorToMatIAL(trainData, data, res,"RT");
	/////////////START RT TRAINNING//////////////////
	CvRTrees forest;
	CvMat* var_importance = 0;

	forest.train(data, CV_ROW_SAMPLE, res, Mat(), Mat(), Mat(), Mat(),
		CvRTParams(10, 10, 0, false, 15, 0, true, 4, 100, 0.01f, CV_TERMCRIT_ITER));
	forest.save("new_rtrees.xml");
}

//支持向量机--训练
void CmymfcDlg::newSvmStudy(vector<NumTrainData>& trainData)
{
	Mat data, res;
	VectorToMatIAL(trainData, data, res,"SVM");

	/////////////START SVM TRAINNING//////////////////
	CvSVM svm;
	CvSVMParams param;
	CvTermCriteria criteria;

	criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);

	svm.train(data, res, Mat(), Mat(), param);
	svm.save("SVM_DATA.xml");
}


//随机决策森林预测
void CmymfcDlg::newRtPredict(vector<NumTrainData>& trainData)
{
	CvRTrees forest;
	forest.load("new_rtrees.xml");
	int right = 0;
	int error = 0;
	int total = 0;
	int testCount = trainData.size();

	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	for (int i = 0; i< testCount; i++)
	{

		NumTrainData td = trainData.at(i);
		memcpy(m.data, td.data, featureLen*sizeof(float));
		
		//normalize(m, m);
		char ret = (char)forest.predict(m);

		testLab.push_back(ret);

		char l = td.result;
		if (ret == l)
		{
			right++;
		}
		else
		{
			error++;
		}
		total++;
		CString str;
		str.Format("正在测试数据：%d/%d", total, testCount);
		GetDlgItem(IDC_PTEXT)->SetWindowText(str);
	}
	l = testLab.begin();
	CString string;
	string.Format("总共：%d 正确：%d 正确率：%f", total, right, 100 * right / (float)total); //将变量组装到字符串中
	MessageBox(string, "标题", MB_OKCANCEL);
}


//支持向量机预测
void CmymfcDlg::newSvmPredict(vector<NumTrainData>& trainData)
{
	
	CvSVM svm;
	svm.load("SVM_DATA.xml");

	int right = 0;
	int error = 0;
	int total = 0;
	int testCount = trainData.size();

	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	for (int i = 0; i< testCount; i++)
	{

		NumTrainData td = trainData.at(i);
		memcpy(m.data, td.data, featureLen*sizeof(float));
		normalize(m, m);
		char ret = (char)svm.predict(m);
		testLab.push_back(ret);
		char l = td.result;
		if (ret == l)
		{
			right++;
		}
		else
		{
			error++;
		}
		total++;
		CString str;
		str.Format("正在测试数据：%d/%d", total, testCount);
		GetDlgItem(IDC_PTEXT)->SetWindowText(str);
	}
	l = testLab.begin();
	CString string;
	string.Format("总共：%d 正确：%d 正确率：%f", total, right, 100*right/(float)total); //将变量组装到字符串中
	MessageBox(string, "标题", MB_OKCANCEL);
}


int isContent(CvRect r1,CvRect r2){
	//区域相等
	if (r1.height == r2.height &&r1.width == r2.width && r1.x == r2.x && r1.y == r2.y){
		return 0;
	}
	//r1区域包含r2
	if (r1.x <= r2.x && r1.y<= r2.y && (r1.x + r1.width) >= (r2.x + r2.width) && (r1.y + r1.height) >= (r2.y + r2.height)){
		return  1;
	}
	//r2包含r1
	if (r1.x >= r2.x && r1.y >= r2.y && (r1.x + r1.width) <= (r2.x + r2.width) && (r1.y + r1.height) <= (r2.y + r2.height)){
		return 2;
	}
	return 3;
}
//将边缘添加到listrect中并且只保留最外层的图像
void addRect(CvRect rect,LISTRECT& listrect){
	int flag = 0;
	LISTRECT::iterator i ;
	bool flag_add = true;
	if (listrect.begin() == listrect.end()){
		listrect.push_back(rect);
	}
	else{
		
		for (i = listrect.begin(); i != listrect.end();){
			flag = isContent(*i, rect);
			if (flag == 3){
				flag_add =  true;
			}
			else if (flag == 2) {
				i=listrect.erase(i);
				flag_add = true;
				continue;
			}
			else if (flag == 1) {
				flag_add = false;
			}
			else if (flag == 0) {
				flag_add = false;
			}
			i++;
		}
		if (flag_add){
			listrect.push_back(rect);
		}
	}
}

//把边缘矩形变成正方形
LISTRECT guiyihua(LISTRECT listrect){
	LISTRECT newlist;
	for each (Rect rect in listrect)
	{
		int h = rect.height > rect.width ? rect.height : rect.width;
		rect.x = rect.x - (h - rect.width) / 2;
		rect.y = rect.y - (h - rect.height) / 2;
		rect.height = h;
		rect.width = h;
		newlist.push_back(rect);
	}
	return newlist;
}


// 查找边缘，输入矩形的边缘
// 参数:   
// pBinary: 输入二值图像，单通道，位深IPL_DEPTH_8U。  
LISTRECT CmymfcDlg::FillInternalContours(IplImage *pBinary)
{
	CvSeq *pContour = NULL;
	CvSeq *pConInner = NULL;
	CvMemStorage *pStorage = NULL;
	LISTRECT listrect;
	// 执行条件   
	if (pBinary)
	{
		// 查找所有轮廓   
		pStorage = cvCreateMemStorage(0);
		//cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//提取所有的边缘放在list中
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//值提取最外层边缘
		// 填充所有轮廓   
		//cvDrawContours(pBinary, pContour, CV_RGB(255, 0, 255), CV_RGB(255, 0, 0), 2, CV_FILLED, 8, cvPoint(0, 0));

		// 外轮廓循环   
		int wai = 0;
		int nei = 0;
		for (; pContour != NULL; pContour = pContour->h_next)
		{
			CvRect rect = cvBoundingRect(pContour, 0);
			//if( !(rect.x == rect.y && rect.x == 1)){
				//addRect(rect, listrect);
				listrect.push_back(rect);
			//}
			//cvRectangle(pBinary, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), CV_RGB(0, 0, 255), 1, 8, 0);
		}
		//return listrect;
		
	}
	return guiyihua(listrect);
}


//加载选择的图像
void CmymfcDlg::OnBnClickedOk()
{

	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||"), NULL
		);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("Open Image");	// 打开文件对话框的标题名
	if (dlg.DoModal() != IDOK)					// 判断是否获得图片
		return;

	CString mPath = dlg.GetPathName();			// 获取图片路径

	IplImage* ipl = cvLoadImage(mPath, 1);	// 读取图片、缓存到一个局部变量 ipl 中
	if (!ipl)									// 判断是否成功读取图片
		return;
	if (TheImage)								// 对上一幅显示的图片数据清零
		cvZero(TheImage);

	ResizeImage(ipl, IDC_ShowImg);	// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
	ShowImage(TheImage, IDC_ShowImg);			// 调用显示图片函数	
	cvReleaseImage(&ipl);

}

//值指定控件中显示图像
void CmymfcDlg::ShowImage(IplImage* img, UINT ID)	// ID 是Picture Control控件的ID号
{
	CDC* pDC = GetDlgItem(ID)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	int iw = img->width;						// 读取图片的宽和高
	int ih = img->height;
	int tx = (int)(rw - iw) / 2;					// 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(img);							// 复制图片
	cimg.DrawToHDC(hDC, &rect);				// 将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}

//点击按钮依次显示分割出的小图像
void CmymfcDlg::OnBnClickedEdgedetect()
{

	IplImage *gray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//edge = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	IplImage *bin = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//int thresh = Otsu(TheImage);
	
	cvCvtColor(TheImage, gray, CV_BGR2GRAY);
	cvThreshold(gray, bin, 179, 255, CV_THRESH_BINARY);


	//获取图片的一些属性
	int height = bin->height;                     // 图像高度
	int width = bin->width;                       // 图像宽度（像素为单位）
	int step = bin->widthStep;                 // 相邻行的同列点之间的字节数
	uchar *data = (uchar *)bin->imageData;

	//反色操作
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			data[i*step + j] = 255 - data[i*step + j];
		}
	}

	
	//cvCvtColor(edge, TheImage, CV_GRAY2BGR);
	ShowImage(bin, IDC_ShowImg);	
	// 调用显示图片函数
	IplImage *temp = cvCloneImage(bin);

	list<CvRect> CVRect = FillInternalContours(temp);//查找边缘

	CDC* pDC = GetDlgItem(IDC_GRAYPIC)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(IDC_GRAYPIC)->GetClientRect(&rect);
	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	ReleaseDC(pDC);
	int j = 0;
	CvSVM svm;
	svm.load("SVM_DATA.xml");
	GetDlgItem(IDC_RESULT)->SetWindowText("");
	for each (CvRect r in CVRect)
	{
		j++;
		cvSetImageROI(bin, r);
		IplImage *g = cvCloneImage(bin);
		IplImage* pDstImage = cvCreateImage(cvSize(rw, rh), g->depth, g->nChannels);
		cvResize(g, pDstImage);
		ShowImage(pDstImage, IDC_GRAYPIC);
		Mat dst = pDstImage;
		Mat temp = Mat::zeros(8, 8, CV_8UC1);
		Mat m = Mat::zeros(1, featureLen, CV_32FC1);
		resize(dst, temp, temp.size());
		for (int i = 0; i<8; i++)
		{
			for (int j = 0; j<8; j++)
			{
				m.at<float>(0, j + i * 8) = temp.at<uchar>(i, j);
			}
		}
		normalize(m, m);
		char ret = (char)svm.predict(m);
		CString s;
		s.Format("%d",ret-'0');
		GetDlgItem(IDC_NOWV)->SetWindowText(s);
		CString result;
		GetDlgItemText(IDC_RESULT, result);
		result = result +" "+ s;
		GetDlgItem(IDC_RESULT)->SetWindowText(result);
		cvResetImageROI(bin);
		cvReleaseImage(&pDstImage);
		cvReleaseImage(&g);
		Sleep(1000);
	}

	cvReleaseImage(&gray);
	cvReleaseImage(&bin);
	cvReleaseImage(&temp);

}

//根据picture控件大小缩放图片
void CmymfcDlg::ResizeImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI(TheImage, cvRect(0, 0, rw, rh));

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, TheImage);

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(TheImage);
}



//数据训练
void CmymfcDlg::OnBnClickedTrain()
{
	vector<NumTrainData> buffer;
	int flag;
	flag = ReadTrainData(buffer,60000,0);
	if (flag == -1){
		MessageBox("文件读取出错，重新读取");
		return;
	}
	if (this->IsDlgButtonChecked(IDC_RADIO1) == BST_CHECKED){//决策森林选中
		MessageBox("训练开始");
		newRtStudy(buffer);
		MessageBox("训练结束");
	}
	else{
		MessageBox("训练开始");
		newSvmStudy(buffer);
		MessageBox("训练结束");
	}
}



//数据测试
void CmymfcDlg::OnBnClickedTest()
{
	int flag;
	vector<NumTrainData> buffer;
	testImg.clear();
	testLab.clear();
	flag = CmymfcDlg::ReadTrainData( buffer,100,1);
	if (flag == -1){
		MessageBox("文件读取出错，重新读取");
		return;
	}
	if (this->IsDlgButtonChecked(IDC_RADIO1) == BST_CHECKED){//决策森林选中
		MessageBox("测试文件读取完毕，请点击开始测试");
		GetDlgItem(IDC_PTEXT)->SetWindowText("正在准备测试数据");
		newRtPredict(buffer);
	}
	else{
		MessageBox("测试文件读取完毕，请点击开始测试");
		GetDlgItem(IDC_PTEXT)->SetWindowText("正在准备测试数据");
		newSvmPredict(buffer);
	}
	buffer.empty();
}

//预览测试图像
void CmymfcDlg::OnBnClickedButton2()
{
	list<Mat>::iterator i_end = testImg.end();
	if (!testImg.empty()){
		if (i != i_end){
			IplImage p = *i;
			ShowImage(&p, IDC_TESTIMG);
			CString str;
			str.Format("系统识别的数字：%d", *l - '0');
			GetDlgItem(IDC_TESTLABE)->SetWindowText(str);
			i++;
			l++;
		}
		else
		{
			MessageBox("测试数据预览完。");
			return;
		}
	}
	else {
		MessageBox("还没有测试结果。");
		return;
	}
}



void CmymfcDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	
	CAboutDlg dlg;
	dlg.DoModal();

}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CClientDC dc(this);
	CRect rect;
	CWnd *pWin = this;//获取该控件的指针，就可以对该控件直接操作了
	pWin->GetClientRect(rect);//把控件的长宽、坐标等信息保存在rect里
	rect.top = rect.top + 50;
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	//创建兼容位图
	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//将屏幕DC的图像复制到内存DC中
	ATL::CImage image;
	image.Attach(hbitmap);                //将位图转化为一般图像
	HRESULT hResult = image.Save("D:\\1.bmp");     //保存图像
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
	image.Detach();
	SelectObject(hdc, hOldMap);

	
	CWnd *pWnd = CWnd::FindWindow(NULL, _T("mymfc"));
	if (pWnd == NULL)
	{
		AfxMessageBox("接收程序没有运行!");
		return;
	}

	pWnd->SendMessage(WM_SYSTEMSET, NULL, 0);
	CDialogEx::OnOK();
}


void CAboutDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	
	CDialogEx::OnOK();
}


void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	CWnd *pWin = this;//获取该控件的指针，就可以对该控件直接操作了
	pWin->GetClientRect(rect);//把控件的长宽、坐标等信息保存在rect里
	rect.bottom = rect.bottom - 50;
	if (rect.PtInRect(point)){
		isDraw = true;
		M_point = point;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	isDraw = false;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	CWnd *pWin = this;//获取该控件的指针，就可以对该控件直接操作了
	pWin->GetClientRect(rect);//把控件的长宽、坐标等信息保存在rect里
	rect.bottom = rect.bottom - 50;
	if (rect.PtInRect(point) && isDraw)
	{
		CDC *pDc = pWin->GetDC();//获取该控件的画布

		pDc->MoveTo(M_point);

		CPen newPen;       // 用于创建新画笔   
		CPen *pOldPen;     // 用于存放旧画笔   

		// 创建实心画笔，粗度为1，颜色为绿色   
		newPen.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		// 选择新画笔，并将旧画笔的指针保存到pOldPen   
		pOldPen = pDc->SelectObject(&newPen);
		pDc->LineTo(point);
		ReleaseDC(pDc);
		M_point = point;
		// 恢复旧画笔   
		pDc->SelectObject(pOldPen);
		// 删除新画笔   
		newPen.DeleteObject();

	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);

::SetCursor(hCur);

return TRUE;

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rect;
	GetClientRect(rect);
	rect.bottom -= 50;
	dc.FillSolidRect(rect, RGB(255, 255, 255));   //设置为绿色背景

	CDialog::OnPaint();

	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
