#include "cv_dnn_centerface.h"

using namespace std;

Centerface::Centerface(std::string model_path)
{
	net = cv::dnn::readNetFromONNX(model_path);
}

Centerface::~Centerface()
{

}

void Centerface::detect(cv::Mat & image, std::vector<FaceInfo>& faces, cv::Size resize_shape, float scoreThresh, float nmsThresh)
{
	image_h = image.rows;
	image_w = image.cols;

	int resized_h = resize_shape.height;
	int resized_w = resize_shape.width;
	
	scale_w = (float)image_w / (float)resized_w;
	scale_h = (float)image_h / (float)resized_h;

	cv::Mat resized;

	//if (image_w == resized_w&&image_h == resized_h)
	//	resized = image;
	//else
	//	cv::resize(image, resized, resize_shape);

	dynamic_scale(resized_w, resized_h);

	cv::Mat inputBlob = cv::dnn::blobFromImage(image, 1.0, cv::Size(d_w, d_h), cv::Scalar(0, 0, 0), true);
	net.setInput(inputBlob);

	std::vector<cv::String> output_names = { "537", "538", "539","540" };
	std::vector<cv::Mat> out_blobs;
	net.forward(out_blobs, output_names);
	
	decode(out_blobs[0], out_blobs[1], out_blobs[2], out_blobs[3], faces,scoreThresh,nmsThresh);
	squareBox(faces);
}

void Centerface::nms(std::vector<FaceInfo>& input, std::vector<FaceInfo>& output, float nmsthreshold,int type)
{
	if (input.empty()) {
		return;
	}
	std::sort(input.begin(), input.end(),
		[](const FaceInfo& a, const FaceInfo& b)
		{
			return a.score < b.score;
		});

	float IOU = 0;
	float maxX = 0;
	float maxY = 0;
	float minX = 0;
	float minY = 0;
	std::vector<int> vPick;
	int nPick = 0;
	std::multimap<float, int> vScores;
	const int num_boxes = input.size();
	vPick.resize(num_boxes);
	for (int i = 0; i < num_boxes; ++i) {
		vScores.insert(std::pair<float, int>(input[i].score, i));
	}
	while (vScores.size() > 0) {
		int last = vScores.rbegin()->second;
		vPick[nPick] = last;
		nPick += 1;
		for (std::multimap<float, int>::iterator it = vScores.begin(); it != vScores.end();) {
			int it_idx = it->second;
			maxX = max(input.at(it_idx).x1, input.at(last).x1);
			maxY = max(input.at(it_idx).y1, input.at(last).y1);
			minX = min(input.at(it_idx).x2, input.at(last).x2);
			minY = min(input.at(it_idx).y2, input.at(last).y2);
			//maxX1 and maxY1 reuse 
			maxX = ((minX - maxX + 1) > 0) ? (minX - maxX + 1) : 0;
			maxY = ((minY - maxY + 1) > 0) ? (minY - maxY + 1) : 0;
			//IOU reuse for the area of two bbox
			IOU = maxX * maxY;
			if (type==NMS_UNION)
				IOU = IOU / (input.at(it_idx).area + input.at(last).area - IOU);
			else if (type == NMS_MIN) {
				IOU = IOU / ((input.at(it_idx).area < input.at(last).area) ? input.at(it_idx).area : input.at(last).area);
			}
			//if (IOU > threshold) {
			if (IOU > nmsthreshold) {
				it = vScores.erase(it);
			}
			else {
				it++;
			}
		}
	}

	vPick.resize(nPick);
	output.resize(nPick);
	for (int i = 0; i < nPick; i++) {
		output[i] = input[vPick[i]];
	}
}

void Centerface::decode(cv::Mat & heatmap, cv::Mat & scale, cv::Mat & offset, cv::Mat & landmarks, std::vector<FaceInfo>& faces, float scoreThresh, float nmsThresh)
{
	int fea_h = heatmap.size[2];
	int fea_w = heatmap.size[3];
	int spacial_size = fea_w*fea_h;

	float *heatmap_ = (float*)(heatmap.data);

	float *scale0 = (float*)(scale.data);
	float *scale1 = scale0+spacial_size;

	float *offset0 = (float*)(offset.data);
	float *offset1 = offset0 + spacial_size;

	std::vector<int> ids = getIds(heatmap_, fea_h, fea_w, scoreThresh);
	//std::cout << ids.size() << std::endl;
	
	std::vector<FaceInfo> faces_tmp;
	for (int i = 0; i < ids.size()/2; i++) {
			int id_h = ids[2*i];
			int id_w = ids[2*i+1];
			int index = id_h*fea_w + id_w;

			float s0 = std::exp(scale0[index]) * 4;
			float s1= std::exp(scale1[index]) * 4;
			float o0 = offset0[index];
			float o1= offset1[index];

			//std::cout << s0 << " " << s1 << " " << o0 << " " << o1 << std::endl;

			float x1 = std::max(0., (id_w + o1 + 0.5) * 4 - s1 / 2);
			float y1 = std::max(0., (id_h + o0 + 0.5) * 4 - s0 / 2);
			float x2 = 0, y2 = 0;
			x1 = std::min(x1, (float)d_w);
			y1= std::min(y1, (float)d_h);
			x2= std::min(x1 + s1, (float)d_w);
			y2= std::min(y1 + s0, (float)d_h);

			//std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;

			FaceInfo facebox;
			facebox.x1 = x1;
			facebox.y1 = y1;
			facebox.x2 =x2;
			facebox.y2 = y2;
			facebox.score = heatmap_[index];
		        facebox.area=(facebox.x2-facebox.x1)*(facebox.y2-facebox.y1);

			//float box_w = std::min(x1 + s1, (float)d_w)-x1;
			//float box_h = std::min(y1 + s0, (float)d_h)-y1;

			float box_w =x2 - x1;//s1?
			float box_h = y2 - y1;// s0 ?

			//std::cout << facebox.x1 << " " << facebox.y1 << " " << facebox.x2 << " " << facebox.y2 << std::endl;

			for (int j = 0; j < 5; j++) {
				float *xmap = (float*)landmarks.data + (2 * j+1)*spacial_size;
				float *ymap= (float*)landmarks.data + (2 * j)*spacial_size;
				facebox.landmarks[2*j] = x1 + xmap[index] * s1;
				facebox.landmarks[2*j+1]= y1 + ymap[index] * s0;
	
			}
			faces_tmp.push_back(facebox);
	}
	

	nms(faces_tmp, faces, nmsThresh);

	//std::cout << faces.size() << std::endl;

	for (int k = 0; k < faces.size(); k++) {
		faces[k].x1 *=d_scale_w*scale_w;
		faces[k].y1 *=d_scale_h*scale_h;
		faces[k].x2 *= d_scale_w*scale_w;
		faces[k].y2 *=d_scale_h*scale_h;

		for (int kk = 0; kk < 5; kk++) {
			faces[k].landmarks[2*kk]*= d_scale_w*scale_w;
			faces[k].landmarks[2*kk+1] *= d_scale_h*scale_h;
		}
	}

}

void Centerface::dynamic_scale(float in_w, float in_h)
{
	d_h = (int)(std::ceil(in_h / 32) * 32);
	d_w = (int)(std::ceil(in_w / 32) * 32);

	d_scale_h = in_h/d_h ;
	d_scale_w = in_w/d_w ;
}

std::vector<int> Centerface::getIds(float *heatmap, int  h, int w, float thresh)
{
	std::vector<int> ids;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (heatmap[i*w + j] > thresh) {
				std::array<int, 2> id = { i,j };
				ids.push_back(i);
				ids.push_back(j);
			}
		}
	}
	return ids;
}

void Centerface::squareBox(std::vector<FaceInfo>& faces)
{
	float w=0, h=0, maxSize=0;
	float cenx, ceny;
	for (int i = 0; i < faces.size(); i++) {
		w = faces[i].x2 - faces[i].x1;
		h = faces[i].y2 - faces[i].y1;

		maxSize = std::max(w, h);
		cenx = faces[i].x1 + w / 2;
		ceny = faces[i].y1 + h / 2;

		faces[i].x1 = std::max(cenx - maxSize / 2, 0.f);                 // cenx - maxSize / 2 > 0 ? cenx - maxSize / 2 : 0;
		faces[i].y1 = std::max(ceny-maxSize/2, 0.f);                     //ceny - maxSize / 2 > 0 ? ceny - maxSize / 2 : 0;
		faces[i].x2 = std::min(cenx + maxSize / 2, image_w - 1.f);  // cenx + maxSize / 2 > image_w - 1 ? image_w - 1 : cenx + maxSize / 2;
		faces[i].y2 = std::min(ceny + maxSize / 2, image_h - 1.f); //ceny + maxSize / 2 > image_h-1 ? image_h - 1 : ceny + maxSize / 2;
	}
}


