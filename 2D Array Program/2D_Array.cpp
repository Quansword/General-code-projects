#include <stdio.h>

class Array2D
{
	int width, height;
	int** curArray; // The array
public:
	Array2D(int sizeX, int sizeY); // Creates the array
	void Resize(int sizeX, int sizeY); // Resizes the array

	int* &operator [](int i) // Allows parts of array to be get and set like a normal 2D array
	{
		return curArray[i];
	}
};

Array2D::Array2D(int sizeX, int sizeY) // Function for creating the array
{
	if (sizeX < 1 || sizeY < 1) // Error catching for if array is impossible size
	{
		throw "Error: An array can not have a width or height less than 1.";
	}

	width = sizeX;
	height = sizeY;

	curArray = new int*[height]; // Allows for resizeable array
	for (int i = 0; i < height; i++) // Loop that creates the array
	{
		curArray[i] = new int[width];
	}

	for (int j = 0; j < height; j++) // Loop that sets the array to 0 as the start value
	{
		for (int k = 0; k < width; k++)
		{
			curArray[j][k] = 0;
		}
	}
}

void Array2D::Resize(int sizeX, int sizeY) // Function to resize the array
{
	if (sizeX < 1 || sizeY < 1) // Error catching for if array is impossible size or the same size
	{
		throw "Error: An array can not have a width or height less than 1.";
	}
	else if (sizeX == width && sizeY == height)
	{
		throw "The array is already this size.";
	}

	int** newArray = new int*[sizeY]; // Temp new array that old array will be copied into
	for (int i = 0; i < sizeY; i++)
	{
		newArray[i] = new int[sizeX];
	}

	for (int j = 0; j < sizeY; j++) // Setting the new array to 0 initially
	{
		for (int k = 0; k < sizeX; k++)
		{
			newArray[j][k] = 0;
		}
	}

	if (sizeX < width) // If the new array width is smaller than the old array width
	{
		if (sizeY < height) // If the new array height is smaller than the old array height
		{
			for (int l = 0; l < sizeY; l++) // Uses new array height as to not copy into a nonexistent cell of the new array
			{
				for (int m = 0; m < sizeX; m++) // Uses new array width as to not copy into a nonexistent cell of the new array
				{
					newArray[l][m] = curArray[l][m]; // Copy the parts of the old array within the bounds of the new array into the new array
				}
			}
		}
		else // If the new array height is larger than the old array height
		{
			for (int l = 0; l < height; l++) // Uses old array height as to not copy from a nonexistent cell of the old array
			{
				for (int m = 0; m < sizeX; m++) // Uses new array width as to not copy into a nonexistent cell of the new array
				{
					newArray[l][m] = curArray[l][m];
				}
			}
		}
	}
	else // If the new array width is larger than the old array width
	{
		if (sizeY > height) // If the new array height is larger than the old array height
		{
			for (int l = 0; l < height; l++) // Uses old array height as to not copy from a nonexistent cell of the old array
			{
				for (int m = 0; m < width; m++) // Uses old array width as to not copy from a nonexistent cell of the old array
				{
					newArray[l][m] = curArray[l][m];
				}
			}
		}
		else // If the new array height is smaller than the old array height
		{
			for (int l = 0; l < sizeY; l++) // Uses new array height as to not copy into a nonexistent cell of the new array
			{
				for (int m = 0; m < width; m++) // Uses old array width as to not copy from a nonexistent cell of the old array
				{
					newArray[l][m] = curArray[l][m];
				}
			}
		}
	}

	// Set new width and height
	width = sizeX;
	height = sizeY;

	// Set new array to the current array and delete the old array
	int** oldArray = curArray;

	curArray = newArray;
	delete oldArray;
}