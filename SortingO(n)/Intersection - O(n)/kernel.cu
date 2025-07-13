/*
Sorting of array in O(n) time
By Diego Peña Sadornil ver13/07/2025
*/

//Includes
#include "cuda_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include "device_launch_parameters.h"

//Functions declarations
__host__ void vectorGenerator(int* hst_vec, int tam, int min, int max);
__host__ void vectorPrinting(int* hst_vec, int tam);
__host__ void deviceProperties(int deviceID);

__global__ void sorting(int* dev_initial_vec, int* dev_final_vec, int tam);

// Main
int main(int argc, char** argv) {
	// Cuda Device
	int currentDeviceId;
	cudaGetDevice(&currentDeviceId);
	deviceProperties(currentDeviceId);

	int* hst_initial_vec, * hst_final_vec;
	int* dev_initial_vec, * dev_final_vec;
	int tam;
	int min;
	int max;
	printf("Insert the numbers of the array (smaller than the number of threads):");
	scanf("%d", &tam);
	printf("Insert the min int value of the array:");
	scanf("%d", &min);
	printf("Insert the max int value of the array:");
	scanf("%d", &max);
	hst_initial_vec = (int*)malloc(tam * sizeof(int));
	hst_final_vec = (int*)malloc(tam * sizeof(int));
	cudaMalloc((void**)&dev_initial_vec, tam * sizeof(int));
	cudaMalloc((void**)&dev_final_vec, tam * sizeof(int));
	vectorGenerator(hst_initial_vec, tam, min, max);
	printf("Initial vector:\n");
	vectorPrinting(hst_initial_vec, tam);
	cudaMemcpy(dev_initial_vec, hst_initial_vec, tam * sizeof(int), cudaMemcpyHostToDevice);
	sorting<<<1, tam>>>(dev_initial_vec, dev_final_vec, tam);
	cudaMemcpy(hst_final_vec, dev_final_vec, tam * sizeof(int), cudaMemcpyDeviceToHost);
	printf("Final vector:\n");
	vectorPrinting(hst_final_vec, tam);
}

// Device properties setter based on Universidad de Burgos version
__host__ void deviceProperties(int deviceID)
{
	int runtimeVersion;
	cudaRuntimeGetVersion(&runtimeVersion);
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, deviceID);
	// calculo del numero de cores (SP)
	int cudaCores = 0;
	int SM = deviceProp.multiProcessorCount;
	int major = deviceProp.major;
	int minor = deviceProp.minor;
	const char* archName;
	switch (major)
	{
	case 1:
		//TESLA
		archName = "TESLA";
		cudaCores = 8;
		break;
	case 2:
		//FERMI
		archName = "FERMI";
		if (minor == 0)
			cudaCores = 32;
		else
			cudaCores = 48;
		break;
	case 3:
		//KEPLER
		archName = "KEPLER";
		cudaCores = 192;
		break;
	case 5:
		//MAXWELL
		archName = "MAXWELL";
		cudaCores = 128;
		break;
	case 6:
		//PASCAL
		archName = "PASCAL";
		cudaCores = 64;
		break;
	case 7:
		//VOLTA (7.0) TURING (7.5)
		cudaCores = 64;
		if (minor == 0)
			archName = "VOLTA";
		else
			archName = "TURING";
		break;
	case 8:
		//AMPERE
		archName = "AMPERE";
		cudaCores = 64;
		break;
	case 9:
		//HOPPER
		archName = "HOPPER";
		cudaCores = 128;
		break;
	default:
		//UNKNOWN ARCHITECTURE
		archName = "UNKNOWN";
		cudaCores = 0;
	}

	// Properties
	printf("***************************************************\n");
	printf("DEVICE %d: %s\n", deviceID, deviceProp.name);
	printf("***************************************************\n");
	printf("> CUDA Toolkit                    \t: %d.%d\n", runtimeVersion / 1000, (runtimeVersion % 1000) / 10);
	printf("> CUDA Architecture               \t: %s\n", archName);
	printf("> Computing capacity            \t: %d.%d\n", major, minor);
	printf("> No. of Multiprocessor        \t: %d\n", SM);
	printf("> No. of CUDA Cores (%dx%d)       \t: %d\n", cudaCores, SM, cudaCores * SM);
	printf("> No. maximum threads (per block)\t: %d\n", deviceProp.maxThreadsPerBlock);
	printf("> Global memory (total)          \t: %zu MiB\n", deviceProp.totalGlobalMem / (1024 * 1024));
	printf("***************************************************\n");
}


__host__ void vectorGenerator(int* hst_vec, int tam, int min, int max) {
	for (int i = 0; i < tam; ++i) {
		hst_vec[i] = rand() % (max - min + 1) + min;
	}
}

__host__ void vectorPrinting(int* hst_vec, int tam) {
	for (int i = 0; i < tam; i++) {
		printf("%d\t", hst_vec[i]);
	}
	printf("\n");
}

__global__ void sorting(int* dev_initial_vec, int* dev_final_vec, int tam) {
	int threadId = threadIdx.x + threadIdx.y * blockDim.x;
	int smallerValues = 0;
	int fixedValue = dev_initial_vec[threadId];
	for (int pos = 0; pos < tam; pos++) {
		if (dev_initial_vec[pos] < fixedValue || (dev_initial_vec[pos] == fixedValue && pos < threadId)) {
			smallerValues += 1;
		}
	}
	dev_final_vec[smallerValues] = fixedValue;
}