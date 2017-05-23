#ifndef RBF_H
#define RBF_H

#include <vector>
#include <opencv2/core/core.hpp>

class RBF
{
private:
	cv::Mat m_data_inv;
	std::vector<cv::Point2f> m_oriSpace;
	std::vector<cv::Point2f> m_defSpace;
	std::vector<cv::Point2f> m_S;

    static void initDataMatrix(const std::vector<cv::Point2f>& origPList,
					           cv::Mat* data_inv);

    static bool generateSModificator(const std::vector<cv::Point2f>& oriSpace,
                              const std::vector<cv::Point2f>& defSpace,
                              const cv::Mat& data_inv,
                              std::vector<cv::Point2f>& S);

    static bool modifyPoints(std::vector<cv::Point2f>* pList,
                      const std::vector<cv::Point2f>& oriSpace,
                      const std::vector<cv::Point2f>& S);

public:
    RBF(){}
	~RBF(){}
	
	void setOriginalSpace(const std::vector<cv::Point2f>& oriSpace);
	
	bool setDeformedSpace(const std::vector<cv::Point2f>& defSpace);
	
	bool interpolate(const std::vector<cv::Point2f>& oriPoints,
                     std::vector<cv::Point2f>* defPoints);

    static bool interpolate(const std::vector<cv::Point2f>& oriSpace, 
                            const std::vector<cv::Point2f>& defSpace,
                            const std::vector<cv::Point2f>& oriPoints,
                            std::vector<cv::Point2f>* defPoints);
};

#endif // RBF_H
