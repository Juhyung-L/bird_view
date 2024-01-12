#ifndef PROJECTEDWINDOW_HPP_
#define PROJECTEDWUNDOW_HPP_

#include <QCloseEvent>
#include <QPushButton>

#include "bird_view/videoWindow.hpp"
#include "bird_view/videoWidget.hpp"
#include "bird_view/undistortionCamera.hpp"
#include "bird_view/projectionCamera.hpp"
#include "bird_view/util.hpp"

class ProjectedWindow : public VideoWindow
{
private:
    void init_ui();
    void set_next_frame() override;
    void enable_close();
    void closeEvent(QCloseEvent *event) override;

    bool close_flag = false;
    Camera current_camera;
    VideoWidget *projected_view;
    QPushButton *close_button;
    UndistortionCamera undistortion_camera;
    ProjectionCamera projection_camera;
    cv::Mat frame;
    cv::Mat frame_undistorted;
    cv::Mat frame_projected;

public:
    ProjectedWindow(QWidget *parent=nullptr);
    void start(const Camera& selected_camera);
};

#endif