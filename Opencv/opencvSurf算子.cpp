#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;
using namespace std;


/** @function main */
int main( )
{

  Mat img_object = imread("x:\\detect.jpg", CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_scene = imread( "x:\\sence.jpg", CV_LOAD_IMAGE_GRAYSCALE );

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian =500;
  
  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  double t=getTickCount();
  detector.detect( img_object, keypoints_object );
  detector.detect( img_scene, keypoints_scene );
  //-- Step 2: Calculate descriptors (feature vectors)

  cout<<"KEYPOINTS:"<<keypoints_object[0].pt<<endl;

  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );//calculate each keypoint`s descriptor vector ,which has 64 element depth
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );
  printf("keypoints amount ,object:%d  scene:%d\n",keypoints_object.size(),keypoints_scene.size());
  printf("Origin object rows:%d cols:%d EXTRATOR object rows:%d cols:%d\n",img_object.rows,img_object.cols,descriptors_object.rows,descriptors_object.cols);
  printf("Origin object rows:%d cols:%d EXTRATOR object rows:%d cols:%d\n",img_scene.rows,img_scene.cols,descriptors_scene.rows,descriptors_scene.cols);
 
  cout<<"EXTRATOR of object:"<<descriptors_object.row(0)<<endl;
  cout<<"EXTRATOR of scene:"<<descriptors_scene.at<float>(0)<<" "<<descriptors_scene.at<float>(1)<<descriptors_scene.at<float>(2)<<endl;
 
  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );
  cout<<"Matches amount:"<<matches.size()<<endl;//show me the amount of all matches
  cout<<"Matches distance from 1st to 3rd:"<<matches[0].distance<<" "<<matches[1].distance<<matches[2].distance<<endl;
  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  
  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 2.5*min_dist )
     { good_matches.push_back( matches[i]); }
  }
  int a;
  if(good_matches.size()==0)
  {
    cout<<"Nothing detected ,enter something to exit"<<endl;
    cin>>a;
    return 0;
  }
  Mat img_matches;
  drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Localize the object
  std::vector<Point2f> obj;
  std::vector<Point2f> scene;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
  }

  Mat H = findHomography( obj, scene, CV_RANSAC );

  //-- Get the corners from the image_1 ( the object to be "detected" )
  std::vector<Point2f> obj_corners(4);
  obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
  obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
  std::vector<Point2f> scene_corners(4);

  perspectiveTransform( obj_corners, scene_corners, H);

  cout<<(getTickCount()-t)/getTickFrequency()<<endl;
  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
  line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

  //
  imshow( "Good Matches & Object detection", img_matches );

  waitKey(0);
  return 0;
  }
