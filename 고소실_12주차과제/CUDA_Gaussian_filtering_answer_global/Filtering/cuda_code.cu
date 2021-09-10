#pragma once

#include "cuda_code.cuh"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

#define Window 2
#define BLOCK_SIZE (1 <<5)

__constant__ float constant_gaussian_kernel[ 25 ];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian 필터링을 하는 커널
//	shared memory를 사용하지 않는다
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__global__ void Gaussian_kernel_no_shared(IN unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {
	/*Todo*/

	int id = blockDim.x * blockIdx.x + threadIdx.x;
	int c = 0, k = 0, l = 0;
	double mean = 0.0;
	int w = floor((double)5 / 2);

	for (c = 0; c < width; c++) {
		mean = 0.0;
		for (k = -w; k <= w; k++) {
			for (l = -w; l <= w; l++) {
				if (id + k < 0 || c + l < 0 || id + k >= height || c + l >= width) mean += 0;
				else mean += d_bitmaps[(id + k) * width + c + l] * constant_gaussian_kernel[(w + k) * 5 + w + l];
			}
		}
		d_Gaussian[id * width + c] = mean;
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian 필터링을 하는 커널
//	shared memory를 사용한다.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern __shared__ unsigned char sharedBuffer[];
__global__ void Gaussian_kernel_shared(INOUT unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {
	/*Todo*/
	const unsigned block_id = blockIdx.y * gridDim.x + blockIdx.x;
	const unsigned thread_id = threadIdx.y * blockDim.x + threadIdx.x;
	const unsigned id = block_id * BLOCK_SIZE + thread_id;

	int c = 0, k = 0, l = 0, i = 0;
	double mean = 0.0;
	int w = floor((double)5 / 2);
	int w0 = 5;

	if (thread_id == 0) { // thread_id on the upper part
		for (c = 0; c < width; c++) {
			for (k = 1; k < w + 1; k++) {
				if (id - k >= 0 && id - k <= height) sharedBuffer[(-k + w) * width + c] = d_bitmaps[(id - k) * width + c];
				else sharedBuffer[(-k + w) * width + c] = 0; // to reduce condition management
			}
		}
	}

	if (thread_id == BLOCK_SIZE - 1) { // thread_id on the below part
		for (c = 0; c < width; c++) {
			for (k = BLOCK_SIZE; k < BLOCK_SIZE + w; k++) {
				if (id + k - BLOCK_SIZE + 1 < height) sharedBuffer[(k + w) * width + c] = d_bitmaps[(id + k - BLOCK_SIZE + 1) * width + c];
				else sharedBuffer[(k + w) * width + c] = 0; // to reduce condition management
			}
		}
	}

	for (c = 0; c < width; c++) {
		sharedBuffer[(thread_id + w) * width + c] = d_bitmaps[(id) * width + c];
		// fill the line of the sharedBuffer (thread_id) with original data (id)
	}
	
	__syncthreads();
	// syncrhonization of threas

	for (c = 0; c < width; c++) {
		mean = 0.0;
		for (k = -w; k <= w; k++) {
			for (l = -w; l <= w; l++) {
				if (c + l < 0 || c + l >= width) mean += 0; // height conditions are already processed at the above part
				else mean += sharedBuffer[(thread_id + w + k) * width + c + l] * constant_gaussian_kernel[(w + k) * w0 + w + l];
				// gaussian filtering on the thread id + w
			}
		}
		d_Gaussian[(id) * width + c] = mean;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Constant variable 인 gaussian kernel을 설정하는 함수
//	후에 gaussian filtering 에서 사용한다.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Gaussain_Kernel(){
	float _1 = 1.0f / 256.0f;
	float _4 = _1 * 4;
	float _6 = _1 * 6;
	float _16 = _1 * 16;
	float _24 = _1 * 24;
	float _36 = _1 * 36;

	float *p_gaussian_kernel = new float[25];

	p_gaussian_kernel[0] = p_gaussian_kernel[4] = p_gaussian_kernel[20] = p_gaussian_kernel[24] = _1;
	p_gaussian_kernel[1] = p_gaussian_kernel[3] = p_gaussian_kernel[5] = p_gaussian_kernel[9]= _4;
	p_gaussian_kernel[15] = p_gaussian_kernel[19] = p_gaussian_kernel[21] = p_gaussian_kernel[23] = _4;
	p_gaussian_kernel[2] = p_gaussian_kernel[10] = p_gaussian_kernel[14] = p_gaussian_kernel[22] = _6;
	p_gaussian_kernel[6] = p_gaussian_kernel[8] = p_gaussian_kernel[16] = p_gaussian_kernel[18] = _16;
	p_gaussian_kernel[7] = p_gaussian_kernel[11] =p_gaussian_kernel[13] = p_gaussian_kernel[17] = _24;
	p_gaussian_kernel[12] = _36;

	cudaMemcpyToSymbol( constant_gaussian_kernel, p_gaussian_kernel, sizeof( float ) * 25 );

	delete[] p_gaussian_kernel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	커널을 실행하기 전 필요한 자료들 준비 및 커널을 실행할 디바이스를 설정
//	Shared_flag 입력 시 NO_SHARED 나 SHARED 중 한 개의 매크로를 넣으면
//	flag값에 맞는 커널을 실행
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Do_Gaussian_on_GPU(IN unsigned char *p_bitmaps, OUT unsigned char *p_Gaussian, long width, long height, int Shared_flag)
{
	/*Todo*/
	cudaError_t cudaStatus;
	CUDA_CALL(cudaSetDevice(0));

	unsigned char* d_bitmaps, * d_Gaussian;

	size_t mem_size;

	mem_size = width * height * sizeof(unsigned char);
	CUDA_CALL(cudaMalloc(&d_bitmaps, mem_size));
	CUDA_CALL(cudaMalloc(&d_Gaussian, mem_size));

	CUDA_CALL(cudaMemcpy(d_bitmaps, p_bitmaps, mem_size, cudaMemcpyHostToDevice));

	dim3 dimBlock(BLOCK_SIZE, 1);
	dim3 dimGrid(height / dimBlock.x, 1);

	Set_Gaussain_Kernel();
	int w = floor((double)5 / 2);
	CHECK_TIME_INIT_GPU();
	CHECK_TIME_START_GPU();

	switch (Shared_flag)
	{
	case NO_SHARED:
		Gaussian_kernel_no_shared << < dimGrid, dimBlock >> > (d_bitmaps, d_Gaussian, width, height);
		break;
	case SHARED:
		Gaussian_kernel_shared << < dimGrid, dimBlock, sizeof(unsigned char) * ((BLOCK_SIZE + 2 * w) * width) >> > (d_bitmaps, d_Gaussian, width, height);
		break;
	}

	CUDA_CALL(cudaStatus = cudaDeviceSynchronize());
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU();

	CUDA_CALL(cudaMemcpy(p_Gaussian, d_Gaussian, mem_size, cudaMemcpyDeviceToHost));
	cudaFree(d_bitmaps);
	cudaFree(d_Gaussian);

	return device_time;
}