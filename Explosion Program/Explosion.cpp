#include <stdio.h>
#include <cmath>

// Function to fill the circle with an explosion, marking effected cells as the markValue
void FillCircle(unsigned char cells[100][100], float xCenter, float yCenter, float radius, unsigned char markValue)
{
	if (xCenter < 0 || yCenter < 0 || xCenter > 99 || yCenter > 99 || radius < 0) // Error catching
	{
		throw "Error: Location or size of circle is impossible.";
	}

	float x = xCenter;
	float y = yCenter;
	float a = 0;
	float b = 0;
	float c = 0;
	float length;

	for (int i = 0; i < 100; i++) // Check each cell
	{
		// Get the x value diffence between the cell position and the explosion xCenter coordinate
		if (i < x)
		{
			a = x - i;
		}
		else
		{
			a = i - x;
		}

		for (int j = 0; j < 100; j++)
		{
			// Get the y value diffence between the cell position and the explosion yCenter coordinate
			if (i < y)
			{
				b = y - j;
			}
			else
			{
				b = j - y;
			}

			// Use the pythagorean theorem to find the actual distance from the explosion center to the cell
			c = pow(a, 2) + pow(b, 2);
			length = sqrt(c);

			if (length <= radius) // If the cell is within the explosion radius, mark it as exploded
			{
				cells[j][i] = markValue;
			}
		}
	}
}

// Testing code
//int main()
//{
//	unsigned char cells[100][100];
//	FillCircle(cells, 5.25f, 20.0f, 2.25f, 150);
//
//	printf("%d\n", cells[20][3]);
//	printf("%d\n", cells[20][7]);
//	printf("%d\n", cells[20][8]);
//	printf("%d\n", cells[3][20]);
//	printf("%d\n", cells[0][0]);
//}