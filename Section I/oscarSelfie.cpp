#include "opencv2/highgui/highgui.hpp"
#include"opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat frame;
Mat img_display;
Mat img;  Mat result;
Mat templ;
int p,q;

 String face_cascade_name = "haarcascade_frontalface_alt.xml";
 String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 string window_name = "oscarSelfie - Face detection";
int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );
void detectAndDisplay( Mat img_display );

/** @function main */
int main( int argc, char** argv )
{
  //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the image and template
    frame = imread(argv[1], CV_LOAD_IMAGE_COLOR); 
    templ = imread( argv[2], 1 );


 
while( true )
     { 
     //-- 3. Apply the classifier to the frame
       if( !frame.empty() )

       {
    detectAndDisplay(frame );

       }
       else
       { 
     cout<<" --(!) No image-- Break!"<<endl; 
       
      }

       int c = waitKey(0);
       if( (char)c == 'c' ) 
{
 break;
 }
      
   int a = waitKey(0);
   return 0;

  waitKey(0);
  return 0;
}
}
/*function detectAndDisplay */
void detectAndDisplay( Mat img_display )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( img_display, frame_gray, CV_BGR2GRAY );
 

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1,1, 0|CV_HAAR_SCALE_IMAGE, Size(100,100) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );

//imshow("ellen",faceROI);
//cout<<i<<center<<endl;;
//waitKey(1000);
 MatchingMethod( 0, 0 );
std::vector<Rect> eyes;


    //-- In each face, detect eyes
  if(center.x<(p+15) && center.x>(p-15))
{
if(center.y<(q+15) && center.y>(q-15))
{
 eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 4, 0 |CV_HAAR_SCALE_IMAGE, Size(25, 25) );

    for( size_t j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
    Vec3f intensity = frame.at<Vec3f>(center.y,center.x);
int blue = intensity.val[0];
int green = intensity.val[1];
int red = intensity.val[2];
cout<<blue<<endl;
cout<<green<<endl;
cout<<red<<endl;
}
}
}

  
  
  //-- Show what you got
 // imshow( window_name, img_display);
//	int p=waitKey(0);
 }

}

/*
 function MatchingMethod brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat exp;
  frame.copyTo( exp );

  /// Create the result matrix
  int result_cols = frame.cols - templ.cols + 1;
  int result_rows = frame.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( frame, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values.
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc;
  }
  else
    { matchLoc = maxLoc; }

  /// Show me what you go
  //rectangle( exp, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

//cout<<( matchLoc.x + (templ.cols/2) )<<( matchLoc.y + (templ.rows /2) ) <<endl;
p=matchLoc.x + (templ.cols/2);
q=matchLoc.y + (templ.rows /2);
  imshow( "Final Image", exp );
    //detectAndDisplay(frame );
 // imshow( "result", result );
  
  return;
}
 

