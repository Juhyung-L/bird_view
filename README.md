# birdview_gui
GUI application that shows the bird's eye view using 3 cameras attached to the left, right, and back sides of a vehicle

This application was developed for my university's (University of Rochester) senior design project in which my team and I were tasked on designing a device that can help our wheelchair-bound client in visualizing his surroundings.

[![Watch the video](https://img.youtube.com/vi/UO8QxPaMO_c/maxresdefault.jpg)](https://youtu.be/UO8QxPaMO_c)

The image processing pipeline is as follows:
Firstly, the raw image is undistorted (we used fisheye cameras, which have a ton of distortion).
![undistortion](https://github.com/Juhyung-L/bird_view/assets/102873080/8e8d6454-a3aa-4dd7-9a8e-bfd10ca94bb9)
![undistorted](https://github.com/Juhyung-L/bird_view/assets/102873080/34d78e27-1080-4756-9e13-2d4188215f3b)

Then, perspective transformation is applied to the image to turn them into a bird's eye view.

[projection.webm](https://github.com/Juhyung-L/bird_view/assets/102873080/f362badb-3cb8-4964-90de-dec737c853e9)

(Clicking the four corners of the calibration board is needed to calculate the projection matrix, which is used for perspective transformation)
![back_projection](https://github.com/Juhyung-L/bird_view/assets/102873080/fa124adf-6058-4cf0-865b-dc0b3292de32)

The same steps are applied to image frames from all 3 cameras and the images are combined together at the end to produce the bird's eye view.

Detailed explanation at: https://juhyungsprojects.blogspot.com/2023/12/birds-eye-view-for-wheelchair.html
