#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <time.h> 
#include <exception>
#include <thread> 
#ifdef __linux__
	#include <unistd.h>
#else
	#include <Windows.h>
#endif

 
using namespace std;
using namespace cv;
 
void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale );
void generateSequence();
void updatePoints(Mat frame);
void showSequence(Mat img);
void zerar();
void acertou();
void win();

string cascadeName;
const char *title = "MEU PAU DE OCULOS";
Mat pau;

int pontos = 0;
int vez=0;
int vezAnterior=0;

int indice = 0;
int indexUser = 0;
bool pronto = false;

vector<int> randons;

bool active = false;

clock_t start;
int main( int argc, const char** argv )
{

	
    VideoCapture capture;
    Mat frame;
    CascadeClassifier cascade;
    double scale=2;

	generateSequence();
	
    
    cascadeName = "C:/opencv/sources/samples/winrt_universal/VideoCaptureXAML/video_capture_xaml/video_capture_xaml.Windows/Assets/haarcascade_frontalface_alt.xml";
    
 if( !cascade.load( cascadeName ) )
    {
        cerr << "ERRO" << endl;
        return -1;
    }

    try {
        if(!capture.open(0))
            cout << "Capture from camera #0 didn't work" << endl;
    }
    catch(exception& e)
    {
        cout << " Excecao capturada " << e.what() << endl;
    }
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade:\n " << cascadeName << endl;
        return -1;
    }
     
    
 
    if( capture.isOpened() )
    {
		
        cout << "O jogo vai começar..." << endl;
        for(;;)
        {
            try {
                capture >> frame;
				flip(frame,frame,1);
            }
            catch (cv::Exception& e)
            {
                std::cout << " Excecao2 capturada frame: " << e.what() << std::endl;
				#ifdef __linux__ 
					usleep(1000000);
				#else
					Sleep(1000000);
				#endif
                continue;
            }
            catch (std::exception& e)
            {
                std::cout << " Excecao3 capturada frame: " << e.what() << std::endl;
				#ifdef __linux__ 
					usleep(1000000);
				#else
					Sleep(1000000);
				#endif
                continue;
            }
 
            if( frame.empty() )
                break;
 
 
        
			//cout << "Width: " << frame.size().width << endl;
			//cout << "Height: " << frame.size().height << endl;

			line(frame, Point(0, (frame.size().height/2)), Point(frame.size().width, (frame.size().height / 2)), Scalar(0, 0, 0), 2, 8);
			line(frame, Point(frame.size().width/2, 0), Point(frame.size().width/2,frame.size().height), Scalar(0, 0, 0), 2, 8);

			showSequence(frame);
			if (vez == 7) {
				indexUser = 0;
				zerar();
				win();
				cout << "GANHOU!!!!" << endl;
				capture.release();
			}

			if (vezAnterior == indice) {
				//cout << "Vez anterior: " << vezAnterior << endl;
				//cout << "Index: " << index << endl;
				if (((clock() - start) / CLOCKS_PER_SEC) > 5) {
					//cout << "Vez: " << vez << endl;
					if (indice == vez) {
						pronto = true;
						indice = 0;
					}
					
					else if (indice == 6) {
						indice = 0;
						pronto = true;
					}
					else {
						indice++;
						pronto = false;
					}
					start = clock();
					//vezAnterior++;
				}
			}
			
			detectAndDraw( frame, cascade, scale );
 
            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
 
    return 0;
}
 
/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */

 
void drawTransparency2(Mat frame, Mat transp, int xPos, int yPos) {
    Mat mask;
    vector<Mat> layers;
     
    split(transp, layers); // seperate channels
    Mat rgb[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent 
    Mat roi1 = frame(Rect(xPos, yPos, transp.cols, transp.rows));
    Mat roi2 = roi1.clone();
    transp.copyTo(roi2.rowRange(0, transp.rows).colRange(0, transp.cols), mask);
    printf("%p, %p\n", roi1.data, roi2.data);
    double alpha = 0.2;
    addWeighted(roi2, alpha, roi1, 1.0 - alpha, 0.0, roi1);
}
 
void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale)
{
    vector<Rect> faces;
    Scalar color = Scalar(250,250,250);
    Mat gray, smallImg;
 
    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
 
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
 
    for ( size_t i = 0; i < faces.size(); i++ )
    {
		
        Rect r = faces[i];
        Point center;
 
        //printf("xy face = %d x %d\n", r.x, r.y);
		//printf("xy frame= %d x %d\n\n", img.size().width, img.size().height);

		rectangle(img, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
			Point(cvRound((r.x + r.width - 1) * scale), cvRound((r.y + r.height - 1) * scale)),
			color, 1, 8, 0);
		
		if ((r.x < 250 / 2) && (r.y < 155 / 2)) {
			//mciSendString("play mp3", NULL, 0, NULL);
			//system("start acertou-mizeravijk.mp3");
			//cout << "Quadrante 1" << endl;

			if (pronto){
				if (((clock() - start) / CLOCKS_PER_SEC) > 3) {
					if (randons[indexUser] == 0) {
						//cout << "Acertou" << endl;
						pontos += 3;
						acertou();
						if (indexUser == vez) {
							indexUser = 0;
							pronto = false;
							vez++;
						}
						else {
							indexUser++;
						}
						start = clock();
					}
					else {
						//cout << "Errou" << endl;
#ifdef __linux__
						system("mplayer faustao-errou.mp3");
#else
						system("start faustao-errou.mp3");
#endif
						zerar();
					}
				}
			}
		}else if ((r.x > 250/2) && (r.y < 155/2)) {
			//cout << "Quadrante 2" << endl;
			if (pronto) {
				if (((clock() - start) / CLOCKS_PER_SEC) > 3) {
					if (randons[indexUser] == 1) {
						//cout << "Acertou" << endl;
						pontos += 3;
						acertou();
						if (indexUser == vez) {
							indexUser = 0;
							pronto = false;
							vez++;
						}
						else {
							indexUser++;
						}
						
						start = clock();
						//thread show(showSequence, img);
						//show.join();
						//showSequence(img);
					}
					else {
#ifdef __linux__
						system("mplayer faustao-errou.mp3");
#else
						system("start faustao-errou.mp3");
#endif
						zerar();
					}
				}
			}
		}
		else if ((r.x < 250 / 2) && (r.y > 155 / 2)) {
			//cout << "Quadrante 3" << endl;
			if (pronto) {
				if (((clock() - start) / CLOCKS_PER_SEC) > 3) {
					if (randons[indexUser] == 2) {
						//cout << "Acertou" << endl;
						pontos += 3;
						acertou();
						if (indexUser == vez) {
							indexUser = 0;
							pronto = false;
							vez++;
						}
						else {
							indexUser++;
						}
						start = clock();
						//thread show(showSequence, img);
						//show.join();
						//showSequence(img);
					}
					else {
#ifdef __linux__
						system("mplayer faustao-errou.mp3");
#else
						system("start faustao-errou.mp3");
#endif
						zerar();
					}
				}
			}
		}
		else if ((r.x > 250 / 2) && (r.y > 155 / 2)) {
			//cout << "Quadrante 4" << endl;
			if (pronto) {
				if (((clock() - start) / CLOCKS_PER_SEC) > 3) {
					if (randons[indexUser] == 3) {
						//cout << "Acertou" << endl;
						pontos += 3;
						acertou();
						if (indexUser == vez) {
							indexUser = 0;
							pronto = false;
							vez++;
						}
						else {
							indexUser++;
						}
						start = clock();
						//thread show(showSequence, img);
						//show.join();
						//showSequence(img);
					}
					else {
#ifdef __linux__
						system("mplayer faustao-errou.mp3");
#else
						system("start faustao-errou.mp3");
#endif
						zerar();
					}
				}
			}
		}
		
		if (indexUser == 7) {
			indexUser = 0;
			zerar();
			win();
			cout << "GANHOU" << endl;
		}
		updatePoints(img);

		
 
    }
    if (!pau.empty())
        drawTransparency2(img, pau, 100, 100);
    imshow( title, img );
}


void generateSequence() {
	randons.clear();
	for (int i = 0; i < 7; i++) {
		randons.push_back(rand() % 4);
	}

/*	cout << "SEQUENCE: ";

	for (int number : randons) {
		cout << number;
	}
	cout << endl;
	*/
	
}

void showSequence(Mat img) {
	if (!pronto) {
		if (randons[indice] == 0) {
			rectangle(img, Point(0, 0), Point(img.size().width / 2, img.size().height / 2), Scalar(204, 255, 51), 1, 8, 0);
		}
		else if (randons[indice] == 1) {
			rectangle(img, Point(img.size().width / 2, 0), Point(img.size().width, img.size().height / 2), Scalar(204, 255, 51), 1, 8, 0);
		}
		else if (randons[indice] == 2) {
			rectangle(img, Point(0, img.size().height / 2), Point(img.size().width / 2, img.size().height), Scalar(204, 255, 51), 1, 8, 0);
		}
		else if (randons[indice] == 3) {
			rectangle(img, Point(img.size().width / 2, img.size().height / 2), Point(img.size().width, img.size().height), Scalar(204, 255, 51), 1, 8, 0);
		}
		vezAnterior = indice;
	}
/*pronto = false;
	for (int i = 0; i <= vez; i++) {
		
			

			vezAnterior = vez;
			
			


			//active = false;
			//Sleep(5000);
			//thread wait(waitSeconds, 5);
			//wait.join();
		}
	
	pronto = true;*/
	return;
}

void updatePoints(Mat frame) {
	std::stringstream ss;
	ss << "Pontuacao: " << pontos;
	String point = ss.str();
	
	putText(frame, point, Point(10, 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
	
}

void zerar() {
	

	
	

	pronto = false;
	
	pontos = 0;
	vez = 0;
	vezAnterior = 0;

	indice = 0;
	indexUser = 0;
	
	active = false; 

	generateSequence();
}

void acertou() {
	#ifdef __linux__
		system("mplayer acertou-mizeravijk.mp3");
	#else
		system("start acertou-mizeravijk.mp3");
	#endif
}

void win() {
	#ifdef __linux__
		system("mplayer vitoria_ayrton_senna.mp3");
	#else
		system("start vitoria_ayrton_senna.mp3");
	#endif
}