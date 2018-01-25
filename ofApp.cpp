#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open("snow.mp4");

	this->cap_size = cv::Size(this->cap.get(CV_CAP_PROP_FRAME_WIDTH), this->cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	int frame_rate = 8;
	ofSetFrameRate(frame_rate);

	// this->writer = cv::VideoWriter("out.mp4", CV_FOURCC('H', '2', '6', '4'), frame_rate, this->cap_size);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	this->cap >> this->frame;
	if (this->frame.empty()) {

		return;
	}
	cv::Mat gray, canny, color_canny;
	cv::cvtColor(this->frame, gray, CV_RGB2GRAY);
	cv::Canny(gray, canny, 50, 150);
	
	for (int y = 0; y < canny.rows; y += 1) {

		unsigned char* value = &canny.at<unsigned char>(y, 0);
		for (int x = 0; x < canny.cols; x += 1) {
			
			if (*value > 0) {
				this->frame.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
			}
			value++;
		}
	}
	// writer << this->frame;
	cv::cvtColor(this->frame, this->frame, CV_BGR2RGB);

	this->fbo.begin();
	ofClear(0);

	ofSetColor(255);
	this->image.update();
	this->image.draw(0, 0);

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->fbo.draw(0, 0);
}

//========================================================================
int main() {

	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
}