#include "matrix.h"
#include <bits/stdc++.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <fstream>
#include <string>

using namespace mat;
using namespace std;

struct arg_struct
{
   int arg1;
   int arg2;
};

typedef void * (*THREADFUNCPTR)(void *);


class Image{
    public:
        Matrix<int> data;
        string imageType;
        int totalPixels;
        int x_size;
        int y_size;

        Image(){}
        Image(string image_path){
            // this->data = _image;
            // this-> imageType = _imageType;
            // this->totalPixels = _image.get_num_cols() * _image.get_num_rows();
            openImage(image_path);
        }

        void openImage(string image_path){
            string mytext;
            ifstream MyReadFile(image_path);
            getline(MyReadFile,mytext);
            getline(MyReadFile,mytext);
            cout << mytext << "\n";
            string s_y_size = mytext.substr(0,3);
            cout <<"x_size: "<< s_y_size;
            string s_x_size = mytext.substr(4,3);
            cout << "y_size: "<<s_x_size;
            this->x_size = atoi( s_x_size.c_str() );
            this->y_size = atoi( s_y_size.c_str() );
            cout << "image size: " << this->x_size << "\n";

            getline(MyReadFile,mytext);
            cout << mytext << "\n";
            this->data.set_size(this->x_size,this->y_size);
            cout<< "y_size: "<< y_size;
            int y = 0;
            int x = 0;
            while (getline(MyReadFile, mytext)) {
                // cout<<"here";
                int begin_position = 0;
                int end_position = 0;
                for(int i = 0; i < mytext.length(); i++){
                    end_position = i;
                    string substring;
                    int actual_pixel;

                    if(
                        (mytext.at(i) == ' '|| mytext.at(i) == '\n')
                        && y < this->y_size 
                        && x < this->x_size
                    ){
                        cout<<"here";
                        substring = (mytext.substr(begin_position, (end_position-begin_position)));
                        actual_pixel = atoi( substring.c_str() );
                        // cout << "X: " << x << "y: " << y << "\n";
                        this->data.at(x,y) = actual_pixel;
                        cout << actual_pixel;
                        begin_position = end_position+1;
                        cout << "\n";
                        y++;
                    }
                }
                x++;
                y=0;
            }

            // Close the file
            MyReadFile.close();
        }

        void saveImage(string file_path){
            ofstream myfile(file_path);
            myfile<< "P2"<< endl;
            myfile << to_string(this->y_size) <<" " << to_string(this->x_size) << endl;
            myfile<< "255"<< endl;

            if(myfile.is_open())
            {
                string str;
                int number =0;
                for (int i = 0; i < this->x_size; i++){
                    str = "";
                    for ( int j = 0; j< this->y_size; j++){

                        str.append(to_string(this->data.at(i,j)));
                        str.append(" ");
                    }
                    // str.append("\n");
                    cout << str << "\n";
                    myfile<<str<< endl;
                    cout << "j " << i;
                    // return;
                }
                myfile.close();
            }
            else cerr<<"Unable to open file";
        }

        void saltAndPepper(int nThreads){
            //TODO
            
            int numCols = data.get_num_cols();
            int numRows = data.get_num_rows();
            int n_cores = (int)(thread::hardware_concurrency() );
            n_cores = 3;
            int        rc; 
            // pthread_t threads[n_cores];
            // struct arg_struct *args[2];
            vector<arg_struct> args;
            args.reserve(n_cores);
            vector<thread> t1;
            int numberOfRows = (int)(numRows / n_cores);
            cout << "number of threads: "<< n_cores;
            cout << "number of rows: " <<numberOfRows <<"\n";
            for(int  i = 0; i < n_cores; i++){
                args[i].arg1 = i * numberOfRows;
                if ((i*numberOfRows + numberOfRows) > numberOfRows){
                    args[i].arg2 = numberOfRows-1;
                }
                else{
                    args[i].arg2 = i*numberOfRows + numberOfRows ;
                }
                // rc = pthread_create( &threads[i], NULL, (THREADFUNCPTR)&Image::saltAndPepperAsync,(void*)&args[i]);
                cout << "fora: " << this->data.at(1,1)<< "\n";
                some_threads_2.push_back(thread(&Image::saltAndPepperAsync,this, args[i]));
            }

            for (auto& t: some_threads_2) t.join();
        }

        vector<Matrix<int>> getSubImages(int NThreads){
            int numCols = data.get_num_cols();
            int numRows = data.get_num_rows();
            int totalPixels = numCols * numRows;
            
            for (int i = 0 ; i < NThreads; i++){

            }
        }

        int median(Matrix<int> subMatrix){
            int numCols = subMatrix.get_num_cols();
            int numRows = subMatrix.get_num_rows();
            int vectorSize = numCols * numRows;
            int numbersArray[numCols + numRows];
            int out_values = 0;
            int actualPosition = 0;
            vector<int> numberVector(numbersArray, numbersArray+vectorSize);
            for (int i = 0 ; i < numCols ; i++){
                for (int  j = 0; j < numRows; j ++){
                    if (subMatrix.at(i,j) == -1 ){
                        out_values --;
                        continue;
                    }
                    numberVector[actualPosition] = subMatrix.at(i,j);
                    actualPosition ++;
                }
            }
            sort(numberVector.begin(), numberVector.begin()+actualPosition);
            return numberVector[(int)actualPosition/2];
        }

        Matrix<int> getSubMatrix(int x, int y, int matrixSize){
            Matrix<int> newMatrix;
            cout << "geting submatrix";
            newMatrix.set_size(matrixSize, matrixSize);
            int xStartPoint = x - (int)(matrixSize  / 2 );
            int yStartPoint = y - (int)(matrixSize / 2 );
            cout << "xStartPoint value: " << xStartPoint;
            int r; 

            for( int i = 0 ; i < matrixSize ; i++){
                for ( int j = 0 ; j < matrixSize; j ++){
                    if(
                        (xStartPoint + i < 0 || yStartPoint +j < 0) || 
                        (
                        i - xStartPoint >this->data.get_num_cols() || 
                        j - yStartPoint >= this->data.get_num_rows()
                        )||
                        (
                        i + xStartPoint >= this->data.get_num_cols() || 
                        j + yStartPoint >= this->data.get_num_cols()
                        )
                    ){
                        newMatrix.at(i,j) = -1;
                    }
                    else{
                        // cout << "i: " << i << "j  :" <<j << "here\n";
                        // cout << "x point: "<< xStartPoint << "\n";
                        // cout << "y point: "<< yStartPoint << "\n";
                        newMatrix.at(i,j) = this->data.at(xStartPoint+i,yStartPoint+j);
                        cout <<"aaa"<< this->data.at(xStartPoint+i,yStartPoint+j);
                        cout<<"done;";
                    }
                }
            }
            cout << "\n";
            cout << newMatrix.serialize();
            return newMatrix;
        }
    
        void pixelToMedian(int x, int y){
            this->data.at(x,y) = median(this->getSubMatrix(x,y,3));
        }

        private:
            void foo_func() { std::cout << "Hello\n\n\n\n"; }
            std::vector<std::thread> some_threads;

            void saltAndPepperAsync(arg_struct a){
                cout << "thread created\n";
                int x = a.arg1;
                int xf = a.arg2;
                int numCols = this->data.get_num_cols();
                int numRows = this->data.get_num_rows();
                for (int i = x; i < xf; i++ ){
                    for ( int j = 0; j < numCols; j++){
                        cout << "medianvaluew: "<< this->median(getSubMatrix(i,j,3));

                        cout << getSubMatrix(i,j,3).serialize();
                        this->data.at(i,j) = this->median(getSubMatrix(i,j,3));
                    }
                }
            }
            std::vector<std::thread> some_threads_2;

};



int main()
{
	auto x = Matrix<int>(9, 9);
	auto y = Matrix<int>(10, 10);
	
	for(int i = 0; i < x.get_num_rows(); i++)
	{
		for(int j = 0; j < x.get_num_cols(); j++)
		{
            if(j < 2){
                x.at(i, j) = -1;
                continue;
            }
			x.at(i, j) = 0 + i;
		}
	}
	
	Image image("ginimo.PGM");

    // image.median(image.data);
    // cout << x.serialize();
    // image.median((image.getSubMatrix(0,2,3)));
    // image.saltAndPepper(2);
    // image.getSubMatrix(0,0,3);
    // cout << image.data.serialize() << "\n";
    image.saltAndPepper(3);
    cout << "\n" << "\n";
    // cout << image.data.serialize() << "\n";
    image.saveImage("teste2.PGM");

	
	return 0;
}