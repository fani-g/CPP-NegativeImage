#include <fstream>
#include <string>
#include <iostream>
#include "Color.h"
#include "Image.h"
#include "ppm\ppm.h"


using namespace std;
using namespace imaging;

	

int main(int argc, char *argv[])
{
	char * filename = new char[50];
	int w = 0, h = 0;

	Image im(w, h);


	//B way.There is no file as an argument
	if (argc == 1) {
		cout << "File name of the image to load: ";
		cin >> filename;								//the user inserts the name of the file
		bool loaded = im.load(filename, "ppm");
		//cout << "bool load value = " << loaded << endl;
		string s2(filename);
		string s3(s2, 0, s2.size() - 4);				//Takes the file name without the .ppm
		string extension = "_neg";					//Put the _neg extension to the name of the file
		string fname = s3 + extension + ".ppm";     //Put the .ppm extension to the name of the file
		const char *name = fname.c_str();

		if (loaded) {
			Color xrw(1, 1, 1);
			for (int i = 0; i < im.getWidth(); i++)
				for (int j = 0; j < im.getHeight(); j++)		  //The negavitive of the picture is created here
					im.setPixel(i, j, xrw - im.getPixel(i, j));	 //p'(x,y) = (1,1,1)- p(x,y)

			bool saved = im.save(name, "ppm");
			//cout << "bool save value = " << saved << endl;

		}

		cout << "Image dimensions are: " << im.getWidth() << "X" << im.getHeight() << "\n";

		system("pause");
		return 0;

	//A way.The file name is provided as an argument in the application
		
	}
		else if (argc == 2) {
			//First argument is the name of the program.In our case neg
			filename = argv[1];						//the name of the file is the second argument 
			bool loaded = im.load(filename, "ppm");
			//cout << "bool load value = " << loaded << endl;
			string s2(filename);
			string s3(s2, 0, s2.size() - 4);				//Takes the file name without the .ppm
			string extension = "_neg";					//Put the _neg extension to the name of the file
			string fname = s3 + extension + ".ppm";     //Put the .ppm extension to the name of the file
			const char *name = fname.c_str();

			if (loaded) {
				Color xrw(1, 1, 1); //black Color
				for (int i = 0; i < im.getWidth(); i++)
					for (int j = 0; j < im.getHeight(); j++)			//The negavitive of the picture is created here
						im.setPixel(i, j, xrw - im.getPixel(i, j));		//p'(x,y) = (1,1,1)- p(x,y)

				bool saved = im.save(name, "ppm");
				//cout << "bool save value = " << saved << endl;

				cout << "Image dimensions are: " << im.getWidth() << "X" << im.getHeight() << "\n";

			}
			system("pause");
			return 0;
		}
			
			else {
				cout << "Too many arguments";
				return  0;
			}
			

		}
	

