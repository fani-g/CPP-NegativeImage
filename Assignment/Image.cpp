#include <iostream>
#include <fstream>
#include <string>
#include "Color.h"
#include "Image.h"
#include "ppm\ppm.h"


using namespace std;
using namespace imaging;

// constructors and destructor

/*! Default constructor.
*
* By default, the dimensions of the image should be zero and the buffer must be set to nullptr.
*/
Image::Image(){
	width = 0;
	height = 0;
	buffer = nullptr;
}

/*! Constructor with width and height specification.
*
* \param width is the desired width of the new image.
* \param height is the desired height of the new image.
*/
Image::Image(unsigned int width, unsigned int height){
	this->width = width;
	this->height = height;
}

/*! Constructor with data initialization.
*
* \param width is the desired width of the new image.
* \param height is the desired height of the new image.
* \param data_ptr is the source of the data to copy to the internal image buffer.
*
* \see setData
*/
Image::Image(unsigned int width, unsigned int height, const Color * data_ptr) {
	this->width = width;
	this->height = height;
	buffer = new Color[width*height];
	for (unsigned int i = 0; i < width*height; i++)
	{
		buffer[i] = data_ptr[i];
	}
}

/*! Copy constructor.
*
* \param src is the source image to replicate in this object.
*/
Image::Image(const Image &src) {
	width = src.width;
	height = src.height;
	buffer = new Color[width*height];
	for (unsigned int i = 0; i < width*height; i++)
	{
		buffer[i] = src.buffer[i];
	}
}

/*! The Image destructor.
*/
Image::~Image() {
	if (buffer != nullptr) {
		delete[]buffer;
		cout << "Image destroyed." << endl;
	}
}

/*! Copy assignment operator.
*
* \param right is the source image.
*/
Image & Image::operator = (const Image & right) {
	if (&right == this)
	{
		return *this;
	}
	if (buffer != nullptr)
		delete[] buffer;
	width = right.width;
	height = right.height;
	buffer = new Color[width*height];
	for (unsigned int i = 0; i < width*height; i++)
	{
		buffer[i] = right.buffer[i];
	}
	return *this;
}

// data accessors

/*! Obtains a pointer to the internal data.
*
*  This is NOT a copy of the internal image data, but rather a pointer
*  to the internally allocated space, so DO NOT attempt to delete the pointer.
*/
Color * Image::getRawDataPtr() {
	return buffer;
}

/*! Obtains the color of the image at location (x,y).
*
*  The method should do any necessary bounds checking.
*
*  \param x is the (zero-based) horizontal index of the pixel to get.
*  \param y is the (zero-based) vertical index of the pixel to get.
*
*  \return The color of the (x,y) pixel as a Color object. Returns a black (0,0,0) color in case of an out-of-bounds x,y pair.
*/
Color Image::getPixel(unsigned int x, unsigned int y) const {
	Color xrwma = buffer[x + width*y];
	if (x > getWidth() || y > getHeight()) {
		return Color(0,0,0);
	}
	unsigned int redPosition = (3 * x + 3 * width*y)% 3;
	xrwma.r = xrwma[redPosition];
	xrwma.g = xrwma[redPosition+1];
	xrwma.b = xrwma[redPosition+2];
	return xrwma;

}

// data mutators

/*! Sets the RGB values for an (x,y) pixel.
*
*  The method should perform any necessary bounds checking.
*
*  \param x is the (zero-based) horizontal index of the pixel to set.
*  \param y is the (zero-based) vertical index of the pixel to set.
*  \param value is the new color for the (x,y) pixel.
*/
void Image::setPixel(unsigned int x, unsigned int y, Color & value){
	Color xrwma = buffer[x + width*y];
	if (x > getWidth() || y > getHeight()) {
		return;
	}
	unsigned int thesi_kokkinou = (3 * x + 3 * width*y) % 3;  
	xrwma[thesi_kokkinou] = value.r;
	xrwma[thesi_kokkinou + 1] = value.g;
	xrwma[thesi_kokkinou + 2] = value.b;
	buffer[x + width*y] = xrwma;
}

/*! Copies the image data from an external raw buffer to the internal image buffer.
*
*  The member function ASSUMES that the input buffer is of a size compatible with the internal storage of the
*  Image object and that the data buffer has been already allocated. If the image buffer is not allocated or the
*  width or height of the image are 0, the method should exit immediately.
*
*  \param data_ptr is the reference to the preallocated buffer from where to copy the data to the Image object.
*/
void Image::setData(const Color * & data_ptr) {
	if (width == 0 || height == 0 || buffer == nullptr ) {
		return;
	}
	for (unsigned int i = 0; i < width*height; i++)
	{
		buffer[i] = data_ptr[i];
	}
}


/*!
* Loads the image data from the specified file, if the extension of the filename matches the format string.
*
* Only the "ppm" extension is supported for now. The extension comparison should be case-insensitive. If the
* Image object is initialized, its contents are wiped out before initializing it to the width, height and data
* read from the file.
*
* \param filename is the string of the file to read the image data from.
* \param format specifies the file format according to which the image data should be decoded from the file.
* Only the "ppm" format is a valid format string for now.
*
* \return true if the loading completes successfully, false otherwise.
*/
bool Image::load(const std::string & filename, const std::string & format) {

	Image * im = nullptr;

	int pos = filename.find(".");					//find .
	string check = filename.substr(pos + 1);		//keep the substing after the . (that is ppm)

	if (check.compare(format) == 0) {

		int w = 0, h = 0;
		const char * fname = filename.c_str();
		//readIt is a float array that contains the image data.
		float * readIt = ReadPPM(fname, &w, &h);
		if (readIt == nullptr)
			return false;

		width = w;
		height = h;
		if (buffer == nullptr) 
			buffer = new Color[width*height];

		/*
		sizeof(Color) = 3 x sizeof(float). 
		Because of that, we need two variables to increment at the same time ,i and j.
		Each element of buffer is a Color object which holds 3 float values that represent the colors 
		Red,Green and Blue
		*/

		for (int i = 0, j = 0; i < 3 * w*h, j < w*h; i += 3, j++)  
		{
			//cout << "I'm inside for and i crashed? " << endl;
			buffer[j] = Color(readIt[i], readIt[i + 1], readIt[i + 2]);
		}


		if (width == 0 || height == 0 || buffer == nullptr)
		{
			//cout << "I'm inside if" << endl;
			im = new Image(w, h, buffer);
		}

		return true;
	}
	else {
		return false;
	}
}

/*!
* Stores the image data to the specified file, if the extension of the filename matches the format string.
*
* Only the "ppm" extension is supported for now. The extension comparison should be case-insensitive. If the
* Image object is not initialized, the method should return immediately with a false return value.
*
* \param filename is the string of the file to write the image data to.
* \param format specifies the file format according to which the image data should be encoded to the file.
* Only the "ppm" format is a valid format string for now.
*
* \return true if the save operation completes successfully, false otherwise.
*/
bool Image::save(const std::string & filename, const std::string & format) {


	int pos = filename.find(".");					//find .
	string check = filename.substr(pos + 1);		//keep the substing after the . (that is ppm)

	if (check.compare(format) == 0) {

		const char * fname = filename.c_str();
		//writeIt is a float array that contains the image data.
		float * writeIt = new float[3 * width *height];

		/*
		Similar to load 
		*/

		for (int i = 0, j = 0; i < 3 * width*height, j < width*height; i += 3, j++)
		{
			//cout << "maybe i crash here too? " << endl;
			writeIt[i] = buffer[j].r;
			writeIt[i + 1] = buffer[j].g;
			writeIt[i + 2] = buffer[j].b;
		}

		WritePPM(writeIt, width, height, fname);

		if (width == 0 || height == 0 || buffer == nullptr)
		{
			return false;
		}

		return true;
	}
	else {
		return false;					 //if it is not a ppm format
	}
}

