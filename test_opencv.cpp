#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"

using namespace cv;
using namespace std;

void drawText(Mat & image);

int main()
{
    FILE file;
    Mat img = imread("/mnt/d/ziti.jpg", CV_LOAD_IMAGE_UNCHANGED);
    printf("可以\n");
    if(img.empty())
    {
        printf("fail");
        return 0;
    }
    Mat gray,img2;
    cvtColor(img,gray,CV_BGR2GRAY);
    //Sobel(gray,sobel,CV_8U,1,0,3);
    Mat binary;
    threshold(gray,binary,0,255,THRESH_OTSU|THRESH_BINARY);
    Mat rec4 = getStructuringElement(MORPH_RECT,Size(30,5));
    Mat rec1 = getStructuringElement(MORPH_RECT,Size(1,1));
    Mat rec3 = getStructuringElement(MORPH_RECT,Size(2,2));
    Mat rec2 = getStructuringElement(MORPH_RECT,Size(5,5));
    Mat dilate0;
    erode(binary,dilate0,rec4);
   /*  Mat dilate1;
    erode(dilate0,dilate1,rec1);
    Mat erode1;
    erode(dilate1,erode1,rec2);
    Mat dilate2;
    dilate(erode1,dilate2,rec3);
    Mat erode2;
    erode(dilate2,erode2,rec3);*/

    vector<RotatedRect> rects;
    vector<vector<Point>> counts;
    vector<Vec4i> hierarchy;
    printf("有输出\n");
    Mat erode2;
    bitwise_not(dilate0,erode2);
    findContours(erode2,counts,hierarchy,CV_RETR_LIST ,CHAIN_APPROX_SIMPLE,Point(0,0));

    for(int i =0; i<counts.size();i++)
    {
        //printf("有输出\n");
        if(contourArea(counts[i])<500)
            continue;
        Rect  rect1=boundingRect(counts[i]);
        char ch[256];
		sprintf(ch,"%d",i);
		string str=ch;
		cout<<"hierarchy  num" <<str<<" ："<<endl<<hierarchy[i]<<endl<<endl;
        //RotatedRect rect = minAreaRect(counts[i]);
        //int width = rect.boundingRect().width;
        //int heigh = rect.boundingRect().height;
       // Point2f P[4];
       // rect.points(P);
       /*  for (int j = 0; j <= 3; j++)
        {
            line(img, P[j], P[(j + 1) % 4], Scalar(0,255,0), 2);
        }*/
        rectangle(binary,rect1,Scalar(0,255,0),2);
        char file_name[20];
        sprintf(file_name,"/mnt/d/p/out%d.txt",i);
        int fd = open(file_name,O_RDWR |O_CREAT);
        if(fd < 0)
        {
            printf("open file");
            exit(1);
        }
        char name[20];
        sprintf(name,"/mnt/d/p/%d.tif",i);
        img(rect1).copyTo(img2);
        imwrite(name,img2);
       // waitKey(15);
       char *outText;
       tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
       if (api->Init(NULL, "chi_sim")) 
       {
           printf("Init fail\n");
       }
       Pix *image = pixRead(name);
       api->SetImage(image);
       outText = api->GetUTF8Text();
       if(write(fd,outText,strlen(outText))<0)
        {
        printf("write fail\n\r");
        }
        delete [] outText;
        close(fd);
    }
   
 
     
    //printf("youshuchu\n");
    imwrite("/mnt/d/p/img.jpg",img);
    imwrite("/mnt/d/p/gray.jpg",gray);
    imwrite("/mnt/d/p/binary.jpg",binary);
   // imwrite("/mnt/d/p/dilate1.jpg",dilate1);
    imwrite("/mnt/d/p/erode0.jpg",dilate0);
   // imwrite("/mnt/d/p/erode1.jpg",erode1);
    imwrite("/mnt/d/p/erode2.jpg",erode2);
   // imwrite("/mnt/d/p/dilate2.jpg",dilate2);
    //imwrite("/mnt/d/p/new.jpg", img);
    waitKey(0);
     
    return 0;
}

void drawText(Mat & image)
{
    putText(image, "Hello OpenCV",
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}
