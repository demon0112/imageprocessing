#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;


Mat frame;
Mat src;
Point pt=Point(0,0);
Mat op;
void Threshold_demo(int ,void*);
void processImage(int /*h*/, void*);
void mark(int ,void*);
int sliderPos = 70;

int main(int argc,char** argv)
{

src=imread(argv[1] ,1);
cvtColor(src,frame, CV_BGR2GRAY);


 Mat kernel = (Mat_<float>(3,3) <<
            0.85,  0.85, 0.85,
            0.85, -6.8, 0.85,
            0.85,  0.85, 0.85);
    Mat imgLaplacian;
    Mat sharp = frame; // copy source image to another temporary one
  filter2D(sharp, imgLaplacian, CV_32F, kernel);
    frame.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;  
  // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
  
    frame = imgResult; // copy back
    // Create binary image from source image
   
    threshold(frame, op, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
   
processImage(0,0);

   
  while(true)
  {
    int c;
    c = waitKey( 20 );
    if( (char)c == 27 )
      { break; }
   }
}
void processImage(int /*h*/, void*)
{
        // int contour_length_threshold=1;
    vector<vector<Point> > contours;
    vector<vector<Point> > contours1;
//vector<Vec4i> hierarchy;
//vector<vector<Point> >::iterator it;
   // Mat bimage =op  >= sliderPos;
//Canny(frame)
    findContours(op, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat cimage = Mat::zeros(op.size(), CV_8UC3);
/*for ( it = contours.begin(); it!=contours.end(); )
{
    if (it->size()<contour_length_threshold)
        it=contours.erase(it);
    else
        ++it;
}*/
    for(size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
   //cout<<count<<endl;
        if( count < 6  )
            continue;
        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
pt.x=box.center.x;
pt.y=box.center.y;
        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(src, contours, (int) i, Scalar::all(255), 0, 8);
        if(count>30 && count<350)
mark(0,0);

    }
   //imshow("contours", src);
imshow("jellyfish",src);
}
void mark(int ,void*)
{
//cout<<contours[i].size

drawMarker(src,pt,Scalar(0,0,255),MARKER_CROSS,10,1);
}


