#ifndef AIS2203_PROJECT_SPHERO_VARIOUS_HPP
#define AIS2203_PROJECT_SPHERO_VARIOUS_HPP

#include <opencv2/opencv.hpp>
#include <boost/beast/core/detail/base64.hpp>

#include <string>
#include <vector>

namespace various {

    cv::Mat convertStringToFrame(const std::string& base64_frame) {

        if (base64_frame.empty()) {
            std::cerr << "Input base64 string is empty.";
            return {};
        }

        // Decode Base64 to binary
        std::string decoded;
        decoded.resize(boost::beast::detail::base64::decoded_size(base64_frame.size()));
        auto result = boost::beast::detail::base64::decode(
                &decoded[0], base64_frame.data(), base64_frame.size());
        decoded.resize(result.first); // resize to actual decoded size


        if (decoded.empty()) {
            std::cerr << "Decoding base64 string failed.";
            return {};
        }

        // Convert binary data to a cv::Mat
        std::vector<uchar> data(decoded.begin(), decoded.end());
        cv::Mat image = cv::imdecode(data, cv::IMREAD_UNCHANGED);

        if (image.empty()) {
            std::cerr << "Failed to create image from decoded string";
            return {};
        }

        return image;
    }

}

#endif // AIS2203_PROJECT_SPHERO_VARIOUS_HPP
