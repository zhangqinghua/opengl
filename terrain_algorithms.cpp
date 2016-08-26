#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>



/**111
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
		FilterHeightBand(heightData, i * width, 1, width, filter);
		FilterHeightBand(heightData, (i + 1) * width - 1, -1, width, filter);
	}

	for (int i = 0; i < width; i++) {
		FilterHeightBand(heightData, i, width, height, filter);
		FilterHeightBand(heightData, width * height - i, -width, height, filter);
	}

}

/**
 * Create a height data set using the "Fault Formation"
 * 生成小地形（小的山丘等）
 *
 * size: Desired size of the height map
 * iterations: Number of detail passes to make
 * filter: Strength of the filter
 */
void MakeTerrainFault(unsigned char heightData[], int width, int height, int iterations, float filter) {
	// the desired min/max heights
	// 一次生成最大高度,最小高度
	// 因为heightData数组的值类型是unsigned char, 最大只有255
	// 这条生成高度公式所产生的最大高度大约是maxDelta*10=250
	// 所以设置maxDelta=25
	int maxDelta = 25;
	int minDelta = 0;


	// Clear the height fTempBuffer
	int size = width * height;
	for (int i = 0; i < size; i++ ) {
		heightData[i] = 0;
	}

	srand((int)time(0));
	// 循环N次以生成高度数据
	for (int currentIteration = 0; currentIteration < iterations; currentIteration++) {
		// calculate the height range (linear interpolation from iMaxDelta to iMinDelta) for this fault-pass
		int iHeight = maxDelta - ( ( maxDelta - minDelta ) * currentIteration ) / iterations;

		// pick two points at random from the entire height map
		
		int x1 = rand() % width;
		int y1 = rand() % height;
		int x2 = x1;
		int y2 = y2;
		do {
			x2 = rand() % width;
			y2 = rand() % height;
		} while (x1 == x2 && y1 == y2);

		//iDirX1, iDirZ1 is a vector going the same direction as the line
		int dirX1 = x2 - x1;
		int dirY1 = y2 - y1;

		for (int y = 0; y < height; y++ ) {
			for (int x = 0; x < width; x++) {
				//iDirX2, iDirZ2 is a vector from iRandX1, iRandZ1 to the current point (in the loop)
				int dirX2 = x - x1;
				int dirY2 = y - y1;
				//if the result of ( iDirX2*iDirZ1 - iDirX1*iDirZ2 ) is "up" (above 0), then raise this point by iHeight
				if (dirX2 * dirY1 - dirY2 * dirX1 > 0) {
					heightData[y * width + x] += iHeight;
				}
			}
		}
	}
	FilterHeightFiled(heightData, width, height, filter);
}




int main() {
	int width = 40;
	int height = 30;
	unsigned char heightData[width * height];




	MakeTerrainFault(heightData, width, height, 60, 0.5);


	for (int y = 0; y < height; y++) {
		for (int z = 0; z < width; z++) {
			std::cout << (int)heightData[y * width + z] << (heightData[y * width + z] < 10 ? "   " : heightData[y * width + z] < 100 ? "  " : " ");
		}
		std::cout << std::endl;
	}

	int max = 0;
	int min = 255;
	for (int i = 0; i < width * height; i++) {
		max = max < heightData[i] ? heightData[i] : max;
	}

	for (int i = 0; i < width * height; i++) {
		min = min > heightData[i] ? heightData[i] : min;
	}
	std::cout << "max: " << max << std::endl;
	std::cout << "min: " << min << std::endl;
	return 0;
}
