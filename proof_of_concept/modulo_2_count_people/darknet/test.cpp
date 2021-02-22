#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "/usr/include/opencv4/opencv2/opencv.hpp"
using namespace std;
using namespace cv;
#include <boost/algorithm/string.hpp>

/*
This functions opens a video file and extracts the frames and put them into a vector of Mat(its the class for representing an img)
*/
void extract_frames(const string &videoFilePath,vector<Mat>& frames){

	try{
		//open the video file
    VideoCapture cap1;


  	VideoCapture cap("output.mp4");// open the video file
  	if(!cap.isOpened())  // check if we succeeded
  		CV_Error(cv::Error::StsError, "Can not open Video file");

  	//cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
  	for(int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT);frameNum++)
  	{
  		Mat frame;
  		cap >> frame; // get the next frame from video
  		frames.push_back(frame);
  	}
  }
  catch( cv::Exception& e ){
    cerr << e.msg << endl;
    exit(1);
  }

}

/*
It saves a vector of frames into jpg images into the outputDir as 1.jpg,2.jpg etc where 1,2 etc represents the frame number
*/
void save_frames(vector<Mat>& frames, const string& outputDir){
  vector<int> compression_params;
  compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
  int frameNumber=0;
	for(vector<Mat>::iterator frame = frames.begin(); frame != frames.end(); ++frame){
	  string filePath = outputDir + to_string(static_cast<long long>(frameNumber))+ ".jpg";
	  imwrite(filePath,*frame,compression_params);
	}

}



int main (){

	//
	// std::string text = "Let me split this into words";
	// std::vector<std::string> results;
	//
	// boost::split(results, text, [](char c){return c == ' ';});
	//
	// cout << results[2] << endl;



  system("wget https://hddn00.skylinewebcams.com/live.m3u8?a=5mk7240d38l3g7s7har1k0nm20");

	fstream my_file;
	my_file.open("live.m3u8?a=5mk7240d38l3g7s7har1k0nm20", ios::in);
	if (!my_file) {
		cout << "No such file";
	}
	else {
		char ch;

		while (1) {
			my_file >> ch;
			if (my_file.eof())
				break;

			cout << ch;
		}

	}
	my_file.close();

  system("ffmpeg -y -i sample.ts -c:v libx264 -c:a aac output.mp4");

  // cv::Mat image = cv::imread("data/dog.jpg");
  vector<Mat> frames;
  extract_frames("output.mp4",frames);
  //to save
  save_frames(frames, "frame");
  system("./darknet detect cfg/yolov3.cfg yolov3.weights frame0.jpg");
  return 0;
}
