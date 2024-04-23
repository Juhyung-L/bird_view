#include <memory>
#include <chrono>

#include "gtest/gtest.h"
#include "rclcpp/rclcpp.hpp"
#include "cv_bridge/cv_bridge.h"

#include "bird_view/util.hpp"

using namespace std::placeholders;

class CameraFPSTest : public ::testing::Test
{
protected:
    CameraFPSTest() = default;
    ~CameraFPSTest() = default;

public:
    Camera right_camera;
    Camera left_camera;
    Camera back_camera;

    std::chrono::_V2::system_clock::time_point left_end;
    std::chrono::_V2::system_clock::time_point right_end;
    std::chrono::_V2::system_clock::time_point back_end;

    bool set_left_end{false};
    bool set_right_end{false};
    bool set_back_end{false};

    double left_avg_fps{0};
    double right_avg_fps{0};
    double back_avg_fps{0};

    int left_frame_count{0};
    int right_frame_count{0};
    int back_frame_count{0};

    void left_camera_callback(const sensor_msgs::msg::Image& /*img*/)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!set_left_end)
        {
            left_end = start;
            set_left_end = true;
            return;
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(start - left_end);
        left_avg_fps = (left_avg_fps * left_frame_count + duration.count()) / ++left_frame_count;
        left_end = start;     
    }

    void right_camera_callback(const sensor_msgs::msg::Image& /*img*/)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!set_right_end)
        {
            right_end = start;
            set_right_end = true;
            return;
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(start - right_end);
        right_avg_fps = (right_avg_fps * right_frame_count + duration.count()) / ++right_frame_count;
        right_end = start;
    }

    void back_camera_callback(const sensor_msgs::msg::Image& /*img*/)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!set_back_end)
        {
            back_end = start;
            set_back_end = true;
            return;
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(start - back_end);
        back_avg_fps = (back_avg_fps * back_frame_count + duration.count()) / ++back_frame_count;
        back_end = start;
    }
};


TEST_F(CameraFPSTest, RUN)
{
    auto node = std::make_shared<rclcpp::Node>("camera_fps_test_ndoe");

    // left camera
    auto left_cam_cb_grp = node->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    rclcpp::SubscriptionOptions left_cam_options;
    left_cam_options.callback_group = left_cam_cb_grp;
    auto left_cam_sub = node->create_subscription<sensor_msgs::msg::Image>(
        "/camera_left/image_raw", rclcpp::SensorDataQoS(), std::bind(&CameraFPSTest::left_camera_callback, this, _1), left_cam_options);

    // right camera
    auto right_cam_cb_grp = node->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    rclcpp::SubscriptionOptions right_cam_options;
    right_cam_options.callback_group = right_cam_cb_grp;
    auto right_cam_sub = node->create_subscription<sensor_msgs::msg::Image>(
        "/camera_right/image_raw", rclcpp::SensorDataQoS(), std::bind(&CameraFPSTest::right_camera_callback, this, _1), right_cam_options);

    // back camera
    auto back_cam_cb_grp = node->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    rclcpp::SubscriptionOptions back_cam_options;
    back_cam_options.callback_group = back_cam_cb_grp;
    auto back_cam_sub = node->create_subscription<sensor_msgs::msg::Image>(
        "/camera_back/image_raw", rclcpp::SensorDataQoS(), std::bind(&CameraFPSTest::back_camera_callback, this, _1), back_cam_options);

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    auto duration = std::chrono::seconds(5); // run the test for 5 seconds
    auto timer = node->create_wall_timer(duration, [&executor]()
        {
            executor.cancel();
        });
    
    executor.spin(); // blocks until executor.cancel() is called

    double expected_fps = 30.0;
    double accepted_tolerance = 5.0;

    ASSERT_LE(std::abs(left_avg_fps - expected_fps), accepted_tolerance);
    ASSERT_LE(std::abs(right_avg_fps - expected_fps), accepted_tolerance);    
    ASSERT_LE(std::abs(back_avg_fps - expected_fps), accepted_tolerance);    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    rclcpp::init(argc, argv);

    bool all_success = RUN_ALL_TESTS();

    rclcpp::shutdown();

}