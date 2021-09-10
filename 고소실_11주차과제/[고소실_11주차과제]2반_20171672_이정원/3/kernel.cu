#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#define	IN
#define OUT
#define INOUT

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

#define N_SIZE (1 << 26)													// 전체 데이터 사이즈
#define NF_SIZE (1 << 6)													// Nf 크기

#define BLOCK_SIZE (1 << 6)													// CUDA 커널 thread block 사이즈


#define BLOCK_WIDTH (1 << 3)
#define BLOCK_HEIGHT (BLOCK_SIZE / BLOCK_WIDTH)

#define N_ITERATION (1 << 0)												// 실험 반복 횟수

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

int N;
int Nf;

int *h_ArrayElements;
int *h_SumOfArrayElements_CPU;
int *h_SumOfArrayElements_GPU;

cudaError_t Sum_n_elements_GPU(IN int *p_ArrayElements, OUT int *p_SumOfElements_GPU, int Nf);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	배열의 index - Nf 부터 index + Nf 데이터 까지의 합을 계산하는 커널 코드
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__global__ void Sum_n_elements_Kernel(IN int *d_ArrayElements, OUT int *d_SumOfArrayElements, int N, int Nf) {
    /*Todo*/

    int tid = blockDim.x * blockIdx.x + threadIdx.x;

    d_SumOfArrayElements[tid] = 0;
    for (int k = tid - Nf; k <= tid + Nf; k++) {
        if (k < 0 || k >= N) continue;
        d_SumOfArrayElements[tid] += d_ArrayElements[k];
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	배열의 index - Nf 부터 index + Nf 데이터 까지의 합을 계산하는 C 코드
//	GPU kernel의 결과와 비교를 통해 옳은 계산을 하였는지 판단하는 데이터로 활용
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Sum_n_elements_CPU(IN int *p_ArrayElements, OUT int *p_SumOfElements_CPU, int Nf) {
    /*Todo*/
 
    for (int i = 0; i < N; i++) {
        p_SumOfElements_CPU[i] = 0;
        for (int k = i - Nf; k <= i + Nf; k++) {
            if (k < 0 || k >= N) continue;
            p_SumOfElements_CPU[i] += p_ArrayElements[k];
        }
    }
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	주어진 bin 파일을 읽는 코드
//	첫 4바이트는 전체 데이터의 개수, 다음 4바이트는 Nf의 크기, 그 이후 N개의 int형 데이터가 저장
//	데이터는 -100 ~ 100 까지의 범위 안의 정수
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_bin_file() {
    printf("***Binary File Read Start!!\n");
    FILE *fp = fopen("Cuda_HW3_input.bin", "rb");
    fread(&N, sizeof(int), 1, fp);
    fread(&Nf, sizeof(int), 1, fp);

    h_ArrayElements = (int *)malloc(N * sizeof(int));
    h_SumOfArrayElements_CPU = (int *)malloc(N * sizeof(int));
    h_SumOfArrayElements_GPU = (int *)malloc(N * sizeof(int));

    fread(h_ArrayElements, sizeof(int), N, fp);

    fclose(fp);
    printf("***Binary File Read End!!\n\n");
}

void init_bin_file(IN int n, IN int nf) {
    printf("***Binary File Create Start!!\n");
    srand((unsigned)time(NULL));
    FILE *fp = fopen("Cuda_HW3_input.bin", "wb");
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(&nf, sizeof(int), 1, fp);

    int i, input;

    for (i = 0; i < n; i++) {
        input = (int)((float)rand() / RAND_MAX * 200 - 100);
        fwrite(&input, sizeof(int), 1, fp);
    }

    fclose(fp);
    printf("***Binary File Create End!!\n\n");
}

int main()
{
    int i;
    init_bin_file(N_SIZE, NF_SIZE);
    read_bin_file();

    TIMER_T CPU_time = 0.0f, GPU_time_NO_SHARED = 0.0f;

    printf("N: %d, NF: %d, BLOCK_SIZE: %d\n", N, Nf, BLOCK_SIZE);

    for (i = 0; i < N_ITERATION; i++) {
        CHECK_TIME_START;
        Sum_n_elements_CPU(h_ArrayElements, h_SumOfArrayElements_CPU, Nf);
        CHECK_TIME_END(compute_time);
        CPU_time += compute_time;
     
        Sum_n_elements_GPU(h_ArrayElements, h_SumOfArrayElements_GPU, Nf);
     
        GPU_time_NO_SHARED += device_time;
    }

    for (i = 0; i < N; i++) {
        if (h_SumOfArrayElements_CPU[i] != h_SumOfArrayElements_GPU[i]) {
            printf("%d : CPU : %d,\tGPU : %d\n", i, h_SumOfArrayElements_CPU[i], h_SumOfArrayElements_GPU[i]);
            break;
        }
    }
    if (i == N)
        printf("***Kernel execution Success!!\n\n");

    printf("***CPU compute time : %.3f ms\n", CPU_time / N_ITERATION);
    printf("***GPU NO SHARED compute time : %.3f ms\n", GPU_time_NO_SHARED / N_ITERATION);


    // Write the output array into the output file.
    FILE *fp = fopen("Cuda_HW3_output.bin", "wb");
    if (!fp) {
        fprintf(stderr, "Error: cannot open the output file...\n");
        exit(-1);
    }

    fwrite(&N, sizeof(int), 1, fp);
    fwrite(&Nf, sizeof(int), 1, fp);

    for (i = 0; i < N; i++)
        fwrite(&h_SumOfArrayElements_GPU[i], sizeof(int), 1, fp);

    fclose(fp);



    free(h_ArrayElements);
    free(h_SumOfArrayElements_CPU);
    free(h_SumOfArrayElements_GPU);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	커널을 실행하기 전 필요한 자료들 준비 및 커널을 실행할 디바이스를 설정
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cudaError_t Sum_n_elements_GPU(IN int *p_ArrayElements, OUT int *p_SumOfElements_GPU, int Nf) {
    cudaError_t cudaStatus;
    /*Todo*/
    CHECK_TIME_INIT_GPU();

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }/////////////  if(cu.....  ==CUDA_CALL
    /*Todo*/

    int(*d_Array), (*d_Sum);

    size_t size;

    size = N * sizeof(int);

    CUDA_CALL(cudaMalloc(&d_Array, size))

        CUDA_CALL(cudaMemcpy(d_Array, p_ArrayElements, size, cudaMemcpyHostToDevice))


        CUDA_CALL(cudaMalloc(&d_Sum, size))

    size = N;

    CHECK_TIME_START_GPU()
        // Assume that width and height are multiples of BLOCK SIZE.
        dim3 dimBlock(BLOCK_SIZE, 1);
    dim3 dimGrid(N / dimBlock.x, 1);

    Sum_n_elements_Kernel << < dimGrid, dimBlock >> > (d_Array, d_Sum, N, Nf);

    CHECK_TIME_END_GPU(device_time)
    
    size = N * sizeof(int);

    CUDA_CALL(cudaGetLastError())

        // cudaDeviceSynchronize waits for the kernel to finish, and returns
        // any errors encountered during the launch.
        CUDA_CALL(cudaDeviceSynchronize())
        CUDA_CALL(cudaMemcpy(p_SumOfElements_GPU, d_Sum, size, cudaMemcpyDeviceToHost))


        CHECK_TIME_DEST_GPU();

Error:
    cudaFree(d_Array);
    cudaFree(d_Sum);

    return cudaStatus;
}
