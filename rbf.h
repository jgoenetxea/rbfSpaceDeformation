#ifndef RBF_H
#define RBF_H

#include <vector>
#include <opencv2/core/core.hpp>

class RBF
{
private:
	cv::Mat m_data_inv;

    cv::Mat& initDataMatrix(const std::vector<cv::Point2f>& origPList);

    bool generateSModificator(const std::vector<cv::Point2f>& oriSpace,
                              const std::vector<cv::Point2f>& defSpace,
                              std::vector<cv::Point2f>& S);

    bool modifyPoints(std::vector<cv::Point2f>* pList,
                      const std::vector<cv::Point2f>& oriSpace,
                      const std::vector<cv::Point2f>& S);

public:
    RBF(){}
	~RBF(){}

    bool interpolate(const std::vector<cv::Point2f>& oriSpace, 
                     const std::vector<cv::Point2f>& defSpace,
                     const std::vector<cv::Point2f>& oriPoints,
                     std::vector<cv::Point2f>* defPoints);
};

#endif // RBF_H
