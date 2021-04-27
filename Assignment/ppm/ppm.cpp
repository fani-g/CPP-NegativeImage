#include <fstream>
#include <string>
#include <iostream>
#include "ppm.h"

using namespace std;


namespace imaging {


	/*! Reads a PPM image file and returns a pointer to a newly allocated float array containing the image data.
	*
	* Values in the returned array are in the range [0,1] and for each pixel, three values are stored.
	*
	*  \param filename is the null-terminated string of the name of the file to open.
	*  \param w is the address of an integer parameter to return the width of the image into.
	*  \param h is the address of an integer parameter to return the height of the image into.
	*
	*  \return a pointer to a new float array of 3 X w X h floats, that contains the image data.
	*  If the reading operation failed the function returns nullptr.
	*/

	float* ReadPPM(const char * filename, int * w, int * h) {

		char ch;		
		char type[3];	//char array which holds the type of the file
		int max = -1;	//maximum allowed value that can be stored in a pixel

		ifstream iFile(filename, ios::in | ios::binary);  //open input file
		if (!iFile) {
			cout << "Cannot open file\n";
			return NULL;
		}

		iFile >> type >> *w >> *h >> max;					//reads the header 
		string s1(type);
		if (s1.compare("P6")) {
			cout << "The file type must be P6 " << endl;
			return NULL;
		}

		if (*w == 0) {
			cout << "The width of picture is missing ";
			return NULL;
		}
		if (*h == 0) {
			cout << "The heigth of picture is missing ";
			return NULL;
		}
		if (max > 255) {
			cout << "maximum value of storage is more than 255 ";
			return NULL;
		}
		if (max == -1) {
			cout << "the maximum value of storage is missing ";
			return NULL;
		}


		float *buffer2 = new float[(3 * (*w)*(*h))];
		iFile.get(ch);										// reads the new line character
		char uch = (unsigned char)ch;						// casting to have characters from 0 to 255

		int i = 0;
		while (iFile.get(ch)) {
			float f = 0.0;
			if ((int)ch < 0) {							//if the value of the character is a negative integer
				int temp = (int)ch + 256;				//Converting into unsigned char value
				f = (float)(temp) / 255.0f;				// divide by 255 to have floats from 0.0 to 1.0

			}
			else {
				uch = (int)ch;							//if the value of the character is a positive integer
				f = (float)(uch) / 255.0f;              // divide by 255 to have floats from 0.0 to 1.0
			}
			buffer2[i] = f;								//store to the buffer2 the float values
			i++;
		}//end of while


		iFile.close();								//Close file

		return buffer2;								//returns the pointer buffer2 

	}

	/*! Writes an image buffer as a PPM file.
	*
	*  \param data contains the image buffer. The data are RGB float tripplets with values in the range [0,1].
	*  \param w is the width of the image in pixels.
	*  \param h is the height of the image in pixels.
	*  \param filename is the null-terminated string of the name of the file to create.
	*
	*  \return true if the image save operation was successful, false otherwise. If the data parameter is nulltr, the
	*  function returns immediately with a false return value.
	*/

	bool WritePPM(const float * data, int w, int h, const char * filename) {

		if (data == nullptr)
		{
			return false;
		}

		ofstream oFile(filename, ios::out | ios::binary);			//open output file

		if (!oFile.is_open()) {
			printf("Could not open ofstream\n");
			return false;
		}

		oFile << "P6\n" << w << "\n" << h << "\n" << "255" << endl;     //writes the header of the ppm file 

		unsigned char * buff = new unsigned char[3 * w*h];        //a buffer of characters
		for (int i = 0; i < 3 * w*h; i++)
		{
			buff[i] = data[i] * 255;							//(float values from 0.0 to 1.0 )* 255 
		}														//gives us the int value of the character

		//writes a sequential block of bytes which is capacity 3 x w x h
		oFile.write(reinterpret_cast<char*>(&buff[0]), 3 * w*h); 
		oFile.clear();
		oFile.close();											//close file
		return true;
	}
}

