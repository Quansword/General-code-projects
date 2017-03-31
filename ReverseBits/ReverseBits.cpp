#include <cstdint>
#include <iostream>

using std::cout;
using std::endl;

void ReverseBits(int8_t Data[], uint32_t NumberOfBitsToReverse)
{
	// Reverse the first NumberOfBitsToReverse in the specified array, leaving
	// the remainder unchanged.
	
	int tempArraySize, modBits;

	modBits = NumberOfBitsToReverse % 8; // Number of bits left after finding the number of full bytes that need to be reversed
	tempArraySize = (NumberOfBitsToReverse + (8 - modBits))/8; // Size of the temporary array is equal to the number of bytes needed to hold all the bits to reverse

	int8_t* tempArray = new int8_t[tempArraySize]; // tempArray will be used to reverse the bits that need to be reversed before they are added back into Data

	for (int i = 0; i < tempArraySize; i++)
	{
		tempArray[i] = Data[tempArraySize - (i + 1)]; // Copy the opposite entry from Data into tempArray
		tempArray[i] = (tempArray[i] << 4) | (tempArray[i] >> 4); // This line and the two below reverse the bits for each entry in tempArray
		tempArray[i] = ((tempArray[i] & 0x33) << 2) | ((tempArray[i] & 0xCC) >> 2);
		tempArray[i] = ((tempArray[i] & 0x55) << 1) | ((tempArray[i] & 0xAA) >> 1);
	}

	for (int i = 0; i < tempArraySize; i++)
	{
		tempArray[i] = tempArray[i] << (8 - modBits); // Serves to get rid of the bits that are not supposed to be reversed
		if (i < tempArraySize - 1)
		{
			tempArray[i] = tempArray[i] | ((tempArray[i + 1] & 0xFF) >> modBits); // Copies the bits from the next entry that will be shifted out
			Data[i] = tempArray[i];
		}
		else
		{
			Data[i] = ((Data[i] << modBits) >> modBits) | tempArray[i]; // For the last entry, the beginning of the final entry is prepped for combining the reversed and non-reversed bits before combining them
		}
		// For example, this process will eventually turn 110001110101 with three non-reversing bits into 001110101000
	}

	delete tempArray; // Cleanup

	// You may assume the array will always be adequately sized to support the
	// input.
}

//int main()
//{
//	int8_t a[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
//	ReverseBits(a, 12);
//
//	return 0;
//}