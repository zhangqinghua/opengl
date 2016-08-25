#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
/**
 * Create a height data set using the "Fault Formation"
 *
 * size: Desired size of the height map
 * iterations: Number of detail passes to make
 * filter: Strength of the filter
 */
void makeTerrainFault(unsigned char heightData[], int w, int h, int iterations, int filter) {
	// the desired min/max heights
	// 一次生成最大高度,最小高度
	// 因为heightData数组的值类型是unsigned char, 最大只有255
	// 这条生成高度公式所产生的最大高度大约是maxDelta*10=250
	// 所以设置maxDelta=25
	int maxDelta = 25;
	int minDelta = 0;


	// Clear the height fTempBuffer
	for (int i = 0; i < w * h; i++ ) {
		heightData[i] = 0;
	}

	srand((int)time(0));
	// 循环N次以生成高度数据
	for (int currentIteration = 0; currentIteration < iterations; currentIteration++) {
		// calculate the height range (linear interpolation from iMaxDelta to iMinDelta) for this fault-pass
		int height = maxDelta - ( ( maxDelta - minDelta ) * currentIteration ) / iterations;

		// pick two points at random from the entire height map
		
		int x1 = rand() % w;
		int y1 = rand() % h;
		int x2 = x1;
		int y2 = y2;
		do {
			x2 = rand() % w;
			y2 = rand() % h;
		} while (x1 == x2 && y1 == y2);

		//iDirX1, iDirZ1 is a vector going the same direction as the line
		int dirX1 = x2 - x1;
		int dirY1 = y2 - y1;

		for (int y = 0; y < h; y++ ) {
			for (int x = 0; x < w; x++) {
				//iDirX2, iDirZ2 is a vector from iRandX1, iRandZ1 to the current point (in the loop)
				int dirX2 = x - x1;
				int dirY2 = y - y1;
				//if the result of ( iDirX2*iDirZ1 - iDirX1*iDirZ2 ) is "up" (above 0), then raise this point by iHeight
				if (dirX2 * dirY1 - dirY2 * dirX1 > 0) {
					heightData[y * h + x] += height;
				}
			}
		}

	}
}


/**
 * 假设有 {0, 255, 0, 255, 255} 这个数组arr
 * 则计算arr[1] = 178.5 = arr[0] * filter + (1 - filter) * arr[1];
 * 		 arr[2] = 53.55 = arr[1] * filter + (1 - filter) * arr[2];	
 */
void FilterHeightBand(unsigned char heightData[], int start, int offset, int count, float filter){
	float v = heightData[start];
	int j = start + offset;
	
	for (int i = 0; i < count-1; i++) {
		heightData[j] = filter * v + (1 - filter) * heightData[j];
		v = heightData[j];
		j += offset;
	}
}

/**
 * 
 */
void FilterHeightFiled(unsigned char heightData[], int width, int height, float filter){
	for (int i = 0; i < height; i++) {
		FilterHeightBand(heightData, i * height, 1, width, filter);
		FilterHeightBand(heightData, i * height + width, -1, width, filter);
	}

	for (int i = 0; i < width; i++) {
		FilterHeightBand(heightData, i * width, width, height, filter);
		FilterHeightBand(heightData, i * width + height, -width, height, filter);
	}

}




int main() {
	int width = 10;
	int height = 20;
	unsigned char heightData[width * height];
	makeTerrainFault(heightData, width, height, 10, 0.3);


	for (int y = 0; y < height; y++) {
		for (int z = 0; z < width; z++) {
			//std::cout << (int)heightData[y * height + z] << (heightData[y * height + z] < 10 ? "   " : heightData[y * height + z] < 100 ? "  " : " ");
		}
		std::cout << std::endl;
	}

	// int max = 0;
	// for (int i = 0; i < 100; i++) {
	// 	max = max < heightData[i] ? heightData[i] : max;
	// }
	// std::cout << max << std::endl;

	return 0;
}
