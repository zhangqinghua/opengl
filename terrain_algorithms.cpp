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
void makeTerrainFault(unsigned char heightData[], int size, int iterations, int filter) {
	// X边长 Y边长
	int side_x = pow(size, 0.5);
	int side_y = pow(size, 0.5);

	// the desired min/max heights
	// 一次生成最大高度,最小高度
	// 因为heightData数组的值类型是unsigned char, 最大只有255
	// 这条生成高度公式所产生的最大高度大约是maxDelta*10=250
	// 所以设置maxDelta=25
	int maxDelta = 25;
	int minDelta = 0;


	// Clear the height fTempBuffer
	for (int i = 0; i < size; i++ ) {
		heightData[i] = 0;
	}
	srand((int)time(0));
	// 循环N次以生成高度数据
	for (int currentIteration = 0; currentIteration < iterations; currentIteration++) {
		// calculate the height range (linear interpolation from iMaxDelta to iMinDelta) for this fault-pass
		int height = maxDelta - ( ( maxDelta - minDelta ) * currentIteration ) / iterations;

		// pick two points at random from the entire height map
		
		int x1 = rand() % side_x;
		int y1 = rand() % side_y;
		int x2 = x1;
		int y2 = y2;
		do {
			x2 = rand() % side_x;
			y2 = rand() % side_y;
		} while (x1 == x2 && y1 == y2);

		//iDirX1, iDirZ1 is a vector going the same direction as the line
		int dirX1 = x2 - x1;
		int dirY1 = y2 - y1;

		for (int y = 0; y < side_y; y++ ) {
			for (int x = 0; x < side_x; x++) {
				//iDirX2, iDirZ2 is a vector from iRandX1, iRandZ1 to the current point (in the loop)
				int dirX2 = x - x1;
				int dirY2 = y - y1;
				//if the result of ( iDirX2*iDirZ1 - iDirX1*iDirZ2 ) is "up" (above 0), then raise this point by iHeight
				if (dirX2 * dirY1 - dirY2 * dirX1 > 0) {
					heightData[y * side_y + x] += height;
				}
			}
		}

	}
}


int main() {

	unsigned char heightData[10 * 10];
	makeTerrainFault(heightData, 100, 10, 0.3);


	for (int y = 0; y < 10; y++) {
		for (int z = 0; z < 10; z++) {
			std::cout << (int)heightData[y * 10 + z] << (heightData[y * 10 + z] < 10 ? "   " : heightData[y * 10 + z] < 100 ? "  " : " ");
		}
		std::cout << std::endl;
	}

	int max = 0;
	for (int i = 0; i < 100; i++) {
		max = max < heightData[i] ? heightData[i] : max;
	}
	std::cout << max << std::endl;

	return 0;
}