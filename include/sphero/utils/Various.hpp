#ifndef AIS2203_PROJECT_SPHERO_VARIOUS_HPP
#define AIS2203_PROJECT_SPHERO_VARIOUS_HPP

#include <opencv2/opencv.hpp>
#include <boost/beast/core/detail/base64.hpp>

#include <string>
#include <vector>

namespace various {

    cv::Mat convertStringToFrame(const std::string& base64_frame) {
        // Decode Base64 to binary
        std::string decoded;
        decoded.resize(boost::beast::detail::base64::decoded_size(base64_frame.size()));

        auto result = boost::beast::detail::base64::decode(
                &decoded[0], base64_frame.data(), base64_frame.size());
        decoded.resize(result.first); // resize to actual decoded size

        // Convert binary data to cv::Mat
        std::vector<uchar> buffer(decoded.begin(), decoded.end());
        cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

        return frame;
    }
}

#endif // AIS2203_PROJECT_SPHERO_VARIOUS_HPP
