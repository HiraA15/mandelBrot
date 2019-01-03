#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <cmath>

int coloredA[512][512][3] = {{{0}}};

int findMandelBrot(double crc, double cuc, int max_iterations)
{
	int i = 0;
	double rrr = 0.0, uuu = 0.0;
	while (i < max_iterations && rrr*rrr+uuu*uuu < 4.0)
	{
		double temp = rrr*rrr-uuu*uuu+crc;
		uuu = 2.0*rrr*uuu+cuc;
		rrr = temp;
		i++;
	}
	return i;
}

double mapReal(int x, int imageW, double minR, double maxR)						//maps to real 
{
	double interval = maxR - minR;
	return x*(interval / imageW) + minR;
}

double mapUnicorn(int y, int imageH, double minI, double maxI)					//maps to the imaginary
{
	double interval = maxI - minI;
	return y*(interval / imageH) + minI;
}

void printTV(int yS, int yE, int imageH, int imageW, double minR, double maxR, double minI, double maxI, int maxN)
{
	for(int y = yS; y <= yE; y++)											//prints the rgb values to a 3D array
	{
		for(int x = 0; x <= imageW; x++)									//the array will hold the values until the thread
		{
			double crc = mapReal(x, imageW, minR, maxR);					//calls are complete and have been timed
			double cuc = mapUnicorn(y, imageH, minI, maxI);					//then the values will be printed back out to the image

			int k = findMandelBrot(crc, cuc, maxN);

			int r = (k %256);
			int g = (k %256);
			int b = (k %256);
			
			coloredA[y][x][0] = r;
			coloredA[y][x][1] = g;
			coloredA[y][x][2] = b;		
		}																						
	}														
}

int main()
{
	std::ifstream fIn("input.txt");
	int imageW, imageH, maxN;
	double minR, maxR, minI, maxI;

	if(!fIn)
	{
		std::cout << "Can't open the file, Felicia." << std::endl;		//if it can't find the file
		std::cin.ignore();

		return 0;
	}

	fIn >> imageW >> imageH >> maxN;
	fIn >> minR >> maxR >> minI >> maxI;
	fIn.close();

	std::ofstream fOut("output_image.ppm");

	fOut << "P3" << std::endl;
	fOut << imageW << " " << imageH << std::endl;			//these end up giving the dimensions
	fOut << "256" << std::endl;
		
	char ch; 
	std::cout << "Enter a character so the timer can start! Quickly!" && std::cin >> ch;
	
	for(int i = 0; i <= 5; ++i)								//generates image 5 times in addition to initial generation, so 6 total
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();		//where the timer starts

		std::thread uno([&](){printTV(0, (imageH/4), imageH, imageW, minR, maxR, minI, maxI, maxN);});					//prints to Q1 of array
		std::thread dos([&](){printTV((imageH/4) + 1, (imageH/2), imageH, imageW, minR, maxR, minI, maxI, maxN);});		//prints to Q2 of array
		std::thread tres([&](){printTV((imageH/2) + 1, (imageH/4) * 3, imageH, imageW, minR, maxR, minI, maxI, maxN);});//prints to Q3 of array
		std::thread quatro([&](){printTV((imageH/4) * 3 + 1, imageH, imageH, imageW, minR, maxR, minI, maxI, maxN);});	//prints to Q4 of array

		uno.join();																			//joining each thread of the array
		dos.join();
		tres.join();
		quatro.join();
	
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();		//ending the timer
	
		typedef std::chrono::duration<int,std::milli> msecs_t ;
		msecs_t duration(std::chrono::duration_cast<msecs_t>(end-start));
		std::cout << duration.count() << " milliseconds to create this image.\n";
	}
																						
	for(int y = 0; y < imageH; y++)
	{
		for(int x = 0; x < imageW; x++)
		{
			fOut << coloredA[y][x][0] << " " << coloredA[y][x][1] << " " << coloredA[y][x][2] << " ";
		}
		fOut << std::endl;								 
	}												
	fOut.close();
	
	std::cout << "Yay, the program worked." << std::endl;		//what is output if everything worked correctly

	return 0;
}