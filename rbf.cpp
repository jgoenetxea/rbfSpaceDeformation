#include "rbf.h"

#define MEASURE_TIME 0

cv::Mat& RBF::initDataMatrix(const std::vector<cv::Point2f>& origPList) {
	size_t n = origPList.size();
	cv::Mat data(n, n, CV_64F);
	std::vector<cv::Point2f> F(n);
	// Generate a matrix called 'data' where all the distances between 
    // landmarks are stored, and another list of values in an array 
    // called 'F'
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			// Using a linear function as RBF functions. data_i_j = Omega(r) = r
			cv::Point2f dif = origPList[i] - origPList[j];
			double mag = sqrt((dif.x * dif.x) + (dif.y * dif.y)); 
			data.at<double>(i,j) = mag;
		}
	}
	m_data_inv = data.inv(cv::DECOMP_LU);
	return m_data_inv;
}

bool RBF::generateSModificator(const std::vector<cv::Point2f>& oriSpace, 
                               const std::vector<cv::Point2f>& defSpace, 
                               std::vector<cv::Point2f>& S) {
	size_t n = oriSpace.size();
	std::vector <cv::Point2f> F(n);
	for (size_t i = 0; i < n; ++i) {
		F[i] = defSpace[i] - oriSpace[i];
	}

	S.clear();
	S.resize(n);
	for (size_t i = 0; i < n; ++i) {
		S[i].x = 0.0;
		S[i].y = 0.0;
		for (size_t j = 0; j < n; ++j) {
			S[i].x += m_data_inv.at<double>(i,j) * F[j].x;
			S[i].y += m_data_inv.at<double>(i,j) * F[j].y;
		}
	}

	return true;
}

bool RBF::modifyPoints(std::vector<cv::Point2f>* pList, 
                     const std::vector<cv::Point2f>& oriSpace,
                     const std::vector<cv::Point2f>& S) {
    std::vector<cv::Point2f>::iterator vertix;
    std::vector<cv::Point2f>::const_iterator land;
	for (vertix = pList->begin(); vertix != pList->end(); ++vertix) {
		cv::Point2f eval(0, 0);
		int i = 0;
		for (land = oriSpace.begin(); land != oriSpace.end(); ++land) {
			cv::Point2f dif = *vertix - *land;
			double mag = sqrt((dif.x * dif.x) + (dif.y * dif.y)); 
			eval = eval + S[i] * mag;
			++i;
		}
		*vertix = *vertix + eval;
	}

	return true;
}

bool RBF::interpolate(const std::vector<cv::Point2f>& oriSpace, 
                      const std::vector<cv::Point2f>& defSpace,
                      const std::vector<cv::Point2f>& oriPoints,
                      std::vector<cv::Point2f>* defPoints) {
	if (oriSpace.size() != defSpace.size()) {
		printf("Location list size do not match!\n");
		return false;
	}

	initDataMatrix(oriSpace);
	
    std::vector<cv::Point2f> S;
	generateSModificator(oriSpace, defSpace, S);

    (*defPoints) = oriPoints;
	modifyPoints(defPoints, oriSpace, S);

	return true;
}
