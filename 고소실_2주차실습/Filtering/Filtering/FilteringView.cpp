
// FilteringView.cpp : implementation of the CFilteringView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Filtering.h"
#endif

#include "FilteringDoc.h"
#include "FilteringView.h"
#include <cmath>
#include "Sorting_q.h"

#define square(a) (a)*(a)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFilteringView

IMPLEMENT_DYNCREATE(CFilteringView, CView)

BEGIN_MESSAGE_MAP(CFilteringView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PROBLEM1_A1, &CFilteringView::OnProblem1A1)
	ON_COMMAND(ID_PROBLEM1_A2, &CFilteringView::OnProblem1A2)
	ON_COMMAND(ID_PROBLEM1_A3, &CFilteringView::OnProblem1A3)
	ON_COMMAND(ID_PROBLEM1_B1, &CFilteringView::OnProblem1B1)
	ON_COMMAND(ID_PROBLEM1_B2, &CFilteringView::OnProblem1B2)
	ON_COMMAND(ID_PROBLEM1_B3, &CFilteringView::OnProblem1B3)
	ON_COMMAND(ID_PROBLEM2_A1, &CFilteringView::OnProblem2A1)
	ON_COMMAND(ID_PROBLEM2_A2, &CFilteringView::OnProblem2A2)
	ON_COMMAND(ID_PROBLEM2_A3, &CFilteringView::OnProblem2A3)
	ON_COMMAND(ID_PROBLEM3, &CFilteringView::OnProblem3)
END_MESSAGE_MAP()

// CFilteringView construction/destruction

CFilteringView::CFilteringView()
{
	// TODO: add construction code here
	m_width = 0;
	m_height = 0;

}

CFilteringView::~CFilteringView()
{
	m_mfcImage1->Destroy();
}

BOOL CFilteringView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFilteringView drawing

void CFilteringView::OnDraw(CDC* pDC)
{
	CFilteringDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//if (!m_mfcImage1->IsNull() && m_width > 0)
	if (m_width > 0) {
		m_mfcImage1->Draw(pDC->GetSafeHdc(), 0, 30);  // Display image at x, y coordinates

		m_cs.Format("Filtering time: %5.2f  (milliseconds)", m_result); // print filtering time
		pDC->TextOut(0,10,m_cs);
	}
}

void CFilteringView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFilteringView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFilteringView diagnostics

#ifdef _DEBUG
void CFilteringView::AssertValid() const
{
	CView::AssertValid();
}

void CFilteringView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFilteringDoc* CFilteringView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFilteringDoc)));
	return (CFilteringDoc*)m_pDocument;
}
#endif //_DEBUG


// CFilteringView message handlers


void CFilteringView::OnProblem1A1()
{
	AverageFiltering(3); // 3x3 kernel
}


void CFilteringView::OnProblem1A2()
{
	AverageFiltering(5); // 5x5 kernel
}


void CFilteringView::OnProblem1A3()
{
	AverageFiltering(9); // 9x9 kernel
}


void CFilteringView::OnProblem1B1()
{
	MovingAverageFiltering(3); // 3x3 kernel
}


void CFilteringView::OnProblem1B2()
{
	MovingAverageFiltering(5); // 5x5 kernel
}


void CFilteringView::OnProblem1B3()
{
	MovingAverageFiltering(9); // 9x9 kernel
}


void CFilteringView::OnProblem2A1()
{
	MedianFiltering(3); // 3x3 window
}


void CFilteringView::OnProblem2A2()
{
	MedianFiltering(5); // 5x5 window
}


void CFilteringView::OnProblem2A3()
{
	MedianFiltering(9); // 9x9 window
}


void CFilteringView::OnProblem3()
{
	m_mfcImage1 = new CImage();
	int r, c;
	m_result = -1;

	// Load a 24 bit image and convert into an 8bit image. Then perform edge detection.
	m_mfcImage1->Load("lena_24.bmp");

	LONG width1 = m_mfcImage1->GetWidth();
	LONG height1 = m_mfcImage1->GetHeight();
	WORD bitcount1 = m_mfcImage1->GetBPP();
	int pitch1 = m_mfcImage1->GetPitch();

	m_width = width1;
	m_height = height1;


	COLORREF col;
	unsigned char gray;
	//unsigned char *imageBits = new unsigned char[m_width*m_height];
	m_imageBits = new unsigned char[m_width*m_height];
	double *Gx = new double[m_width*m_height];
	double *Gy = new double[m_width*m_height];
	double *G = new double[m_width*m_height];

	int max_gray = 0;
	int max_d = 0;

	// take R value only because R, G, B have the same value
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			col = m_mfcImage1->GetPixel(c,r);
			m_imageBits[r*m_width+c] = GetRValue(col)*0.299 + GetGValue(col)*0.587 + GetBValue(col)*0.114; 
		}
	}

	int w0 = 3;
	int w = floor((double)w0/2);
	int k, l;
	double temp, gradient;

	int Sx[3][3], Sy[3][3];
	Sx[0][0] = -1; Sx[0][1] = 0; Sx[0][2] = 1;
	//
	// Problem 3: Sobel ������(S_X)�� �ϼ��Ѵ�
	//
	Sx[1][0] = -2; Sx[1][1] = 0; Sx[1][2] = 2;
	Sx[2][0] = -1; Sx[2][1] = 0; Sx[2][2] = 1;


	
	Sy[0][0] = 1; Sy[0][1] = 2; Sy[0][2] = 1;
	//
	// Problem 3: Sobel ������(S_Y)�� �ϼ��Ѵ�
	//
	Sy[1][0] = 0; Sy[1][1] = 0; Sy[1][2] = 0;
	Sy[2][0] = -1; Sy[2][1] = -2; Sy[2][2] = -1;
	 
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_start);

	// Perform average filtering
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {

			//
			// Problem 3: Sobel �����ڸ� �̿��Ͽ� Gradient Maginitude�� ���Ѵ�
			//

			gradient = 0;

			for (k = -w; k <= w; k++) {

				for (l = -w; l <= w; l++) {

					if (r + k < 0 || c + l < 0 || r + k >= m_height || c + l >= m_width) gradient += 0;
					else gradient += m_imageBits[(r + k) * m_width + c + l] * (double)Sx[w + k][w + l];
				}
			}
		
			Gx[r * m_width + c] = gradient;

			gradient = 0;

			for (k = -w; k <= w; k++) {

				for (l = -w; l <= w; l++) {

					if (r + k < 0 || c + l < 0 || r + k >= m_height || c + l >= m_width) gradient += 0;
					else gradient += m_imageBits[(r + k) * m_width + c + l] * (double)Sy[w + k][w + l];
				}
			}

			Gy[r * m_width + c] = gradient;

			G[r * m_width + c] = sqrt(pow(Gx[r * m_width + c], 2) + pow(Gy[r * m_width + c], 2));
		}
	}
	QueryPerformanceCounter(&m_end);
	m_result = 1000*(m_end.QuadPart-m_start.QuadPart)/m_frequency.QuadPart;	


	// Threshold and assign the same value into R, G, B channels.
	// Check how the edge image is affected with different threshold values.
	int threshold = 100;
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			//m_mfcImage1->SetPixel(c,r,RGB(imageBits[r*m_width+c],imageBits[r*m_width+c],imageBits[r*m_width+c]));
			if (G[r*m_width+c] > threshold)
				temp = 255;
			else
				temp = 0;
			m_mfcImage1->SetPixel(c,r,RGB(temp,temp,temp));
		}
	}

	Invalidate();
	delete m_imageBits, Gx, Gy, G;
}

void CFilteringView::AverageFiltering(int w0)
{
	m_mfcImage1 = new CImage();
	int r, c;
	m_result = -1;

	// We load a 24 bit image, with R, G, B channels.
	// Since all R, G, B have the same value, this is effectively a grayscale image.
	// Loading and processing an 8 bit image may have some problem because of the pixel alignment
	// - image width and bit width may not be aligned correctly. This is because the image width
	//   needs to be a multiple of 4 bytes but the bit representation does not have this restriction.
	// We will not handle this issue in detail in this lab. Most of the images we will handle will be
	// in 24 bit format in practice.
	m_mfcImage1->Load("SaltPepper_24bit.bmp");

	LONG width1 = m_mfcImage1->GetWidth();
	LONG height1 = m_mfcImage1->GetHeight();
	WORD bitcount1 = m_mfcImage1->GetBPP();
	int pitch1 = m_mfcImage1->GetPitch();

	m_width = width1;
	m_height = height1;


	COLORREF col;
	unsigned char gray;
	unsigned char *imageBits = new unsigned char[m_width*m_height];
	unsigned char *imageBitsFiltered = new unsigned char[m_width*m_height];
	int max_gray = 0;
	int max_d = 0;

	// take R value only because R, G, B have the same value
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			col = m_mfcImage1->GetPixel(c,r);
			imageBits[r*m_width+c] = GetRValue(col); 
			// GetGValue() and GetBValue() to take the multi-channel values
		}
	}

	int w = floor((double)w0/2);
	int k, l;
	double **kernel = new double*[w0];
	for (r=0; r<w0; r++)
		kernel[r] = new double[w0];
	for (r=0; r<w0; r++)
		for (c=0; c<w0; c++)
			kernel[r][c] = (double) 1/((double)w0*w0); // 1/3*3... 1/5*5...
	double temp, mean;
	

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_start);

	// Perform average filtering
	
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			mean = 0;

			//
			// Problem 1-a1,a2,a3: imageBits �� Average Filtering ó���� �Ͽ� imageBitsFiltered �� �Ҵ��Ѵ�
			//
		
			for (k = -w; k <= w; k++) {

				for (l = -w; l <= w; l++) {

					if (r + k < 0 || c + l < 0 || r + k >= m_height || c + l >= m_width) mean += 0;
					else mean += imageBits[(r + k) * m_width + c + l] * kernel[w - k][w - l];
				}
			}
			if(mean < 0 || mean > 255)  imageBitsFiltered[r * m_width + c] = 0;
			else imageBitsFiltered[r * m_width + c] = (int)mean;

		}
	}
	QueryPerformanceCounter(&m_end);
	m_result = 1000*(m_end.QuadPart-m_start.QuadPart)/m_frequency.QuadPart;	

	// Assign the same value into R, G, B channels.
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			//m_mfcImage1->SetPixel(c,r,RGB(imageBits[r*m_width+c],imageBits[r*m_width+c],imageBits[r*m_width+c]));
			temp = imageBitsFiltered[r*m_width+c];
			m_mfcImage1->SetPixel(c,r,RGB(temp,temp,temp));
		}
	}

	Invalidate();
	delete imageBits, imageBitsFiltered;
	for (r=0; r<w0; r++)
		delete kernel[r];
	delete [] kernel;
}

void CFilteringView::MovingAverageFiltering(int w0)
{
	m_mfcImage1 = new CImage();
	int r, c;
	m_result = -1;

	// We load a 24 bit image, with R, G, B channels.
	// Since all R, G, B have the same value, this is effectively a grayscale image.
	// Loading and processing an 8 bit image may have some problem because of the pixel alignment
	// - image width and bit width may not be aligned correctly. This is because the image width
	//   needs to be a multiple of 4 bytes but the bit representation does not have this restriction.
	// We will not handle this issue in detail in this lab. Most of the images we will handle will be
	// in 24 bit format in practice.
	m_mfcImage1->Load("SaltPepper_24bit.bmp");

	LONG width1 = m_mfcImage1->GetWidth();
	LONG height1 = m_mfcImage1->GetHeight();
	WORD bitcount1 = m_mfcImage1->GetBPP();
	int pitch1 = m_mfcImage1->GetPitch();

	m_width = width1;
	m_height = height1;


	COLORREF col;
	unsigned char gray;
	m_imageBits = new unsigned char[m_width*m_height];
	m_imageBitsFiltered = new unsigned char[m_width*m_height];
	int max_gray = 0;
	int max_d = 0;

	// take R value only because R, G, B have the same value
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			col = m_mfcImage1->GetPixel(c,r);
			m_imageBits[r*m_width+c] = GetRValue(col); 
			// GetGValue() and GetBValue() to take the multi-channel values
		}
	}

	int w = floor((double)w0/2);
	int k, l;
	double **kernel = new double*[w0];
	for (r=0; r<w0; r++)
		kernel[r] = new double[w0];
	for (r=0; r<w0; r++)
		for (c=0; c<w0; c++)
			kernel[r][c] = (double) 1/((double)w0*w0);
	double temp = 0, mean, temp1 = 0, temp2 = 0;;
	

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_start);

	// Perform average filtering, using moving average
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {

			//
			// Problem 1-b1,b2,b3: imageBits �� Moving Average Filtering ó���� �Ͽ� imageBitsFiltered �� �Ҵ��Ѵ�
			//
			mean = 0;

			if (c == 0) { // If it's first column
				for (k = -w; k <= w; k++) {

					for (l = -w; l <= w; l++) {

						if (r + k < 0 || c + l < 0 || r + k >= m_height || c + l >= m_width) mean += 0;
						else mean += m_imageBits[(r + k) * m_width + c + l] * kernel[w - k][w - l];
					}
				}
				if (mean < 0 || mean > 255)  m_imageBitsFiltered[r * m_width + c] = 0;
				else m_imageBitsFiltered[r * m_width + c] = mean;
			}
			else {
				for (k = -w; k <= w; k++) {
					if (r + k < 0 || c - 1 -w < 0 || r + k >= m_height || c - 1 - w >= m_width) temp1 += 0;
					else temp1 += m_imageBits[(r + k) * m_width + c -1 -w] * kernel[w - k][w + w];
				}
				for (k = -w; k <= w; k++) {
					if (r + k < 0 || c + w < 0 || r + k >= m_height || c + w >= m_width) temp2 += 0;
					else temp2 += m_imageBits[(r + k) * m_width + c + w] * kernel[w - k][w - w];
				}
				mean = temp - temp1 + temp2;
				if (mean < 0 || mean > 255) m_imageBitsFiltered[r * m_width + c] = 0;
				else m_imageBitsFiltered[r * m_width + c] = mean;

			}

			temp = mean; // ���� mean ����
			temp1 = 0;
			temp2 = 0;

		}
	}
	QueryPerformanceCounter(&m_end);
	m_result = 1000*(m_end.QuadPart-m_start.QuadPart)/m_frequency.QuadPart;	

	// Assign the same value into R, G, B channels.
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			//m_mfcImage1->SetPixel(c,r,RGB(imageBits[r*m_width+c],imageBits[r*m_width+c],imageBits[r*m_width+c]));
			temp = m_imageBitsFiltered[r*m_width+c];
			m_mfcImage1->SetPixel(c,r,RGB(temp,temp,temp));
		}
	}

	Invalidate();
	delete m_imageBits, m_imageBitsFiltered;
	for (r=0; r<w0; r++)
		delete kernel[r];
	delete [] kernel;
}

void CFilteringView::MedianFiltering(int w0)
{
	m_mfcImage1 = new CImage();
	int r, c;
	m_result = -1;

	// We load a 24 bit image, with R, G, B channels.
	// Since all R, G, B have the same value, this is effectively a grayscale image.
	// Loading and processing an 8 bit image may have some problem because of the pixel alignment
	// - image width and bit width may not be aligned correctly. This is because the image width
	//   needs to be a multiple of 4 bytes but the bit representation does not have this restriction.
	// We will not handle this issue in detail in this lab. Most of the images we will handle will be
	// in 24 bit format in practice.
	m_mfcImage1->Load("SaltPepper_24bit.bmp");

	LONG width1 = m_mfcImage1->GetWidth();
	LONG height1 = m_mfcImage1->GetHeight();
	WORD bitcount1 = m_mfcImage1->GetBPP();
	int pitch1 = m_mfcImage1->GetPitch();

	m_width = width1;
	m_height = height1;


	COLORREF col;
	unsigned char gray;
	//unsigned char *imageBits = new unsigned char[m_width*m_height];
	//unsigned char *imageBitsFiltered = new unsigned char[m_width*m_height];
	m_imageBits = new unsigned char[m_width*m_height];
	m_imageBitsFiltered = new unsigned char[m_width*m_height];
	int max_gray = 0;
	int max_d = 0;

	// take R value only because R, G, B have the same value
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			col = m_mfcImage1->GetPixel(c,r);
			m_imageBits[r*m_width+c] = GetRValue(col); 
			// GetGValue() and GetBValue() to take the multi-channel values
		}
	}

	int K=w0*w0, i;
	int w = floor((double)w0/2);
	int k, l;
	double **kernel = new double*[w0];
	for (r=0; r<w0; r++)
		kernel[r] = new double[w0];
	for (r=0; r<w0; r++)
		for (c=0; c<w0; c++)
			kernel[r][c] = (double) 1/((double)w0*w0); // 1/3*3... 1/5*5...
	double temp, mean;
	
	unsigned char* medarray = (unsigned char*) new unsigned char[K];

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_start);

	// Perform median filtering
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {

			//		// QuickMedian() �Լ��� �̿��Ͽ� Sorting �Ѵ�. <-- �̰Ÿ� �ض�
			//
			i = 0;
			for (k = -w; k <= w; k++) {

				for (l = -w; l <= w; l++) {

					if (r + k < 0 || c + l < 0 || r + k >= m_height || c + l >= m_width) medarray[i] = 0;
					else medarray[i] = m_imageBits[(r+k)*m_width + c + l];
					i++;
				}
			}

			QuickMedian(medarray, 0, K-1, K / 2);
			
			if (medarray[K / 2] < 0 || medarray[K / 2] > 255)  m_imageBitsFiltered[r * m_width + c] = 0;
			else m_imageBitsFiltered[r * m_width + c] = medarray[K / 2];
		}
	}


	QueryPerformanceCounter(&m_end);
	m_result = 1000*(m_end.QuadPart-m_start.QuadPart)/m_frequency.QuadPart;	

	// Assign the same value into R, G, B channels.
	for (r=0; r<m_height; r++) {
		for (c=0; c<m_width; c++) {
			//m_mfcImage1->SetPixel(c,r,RGB(imageBits[r*m_width+c],imageBits[r*m_width+c],imageBits[r*m_width+c]));
			temp = m_imageBitsFiltered[r*m_width+c];
			m_mfcImage1->SetPixel(c,r,RGB(temp,temp,temp));
		}
	}

	Invalidate();
	delete m_imageBits, m_imageBitsFiltered, medarray;
	for (r=0; r<w0; r++)
		delete kernel[r];
	delete [] kernel;
}