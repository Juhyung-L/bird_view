import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess, EmitEvent, RegisterEventHandler, Shutdown
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.actions import TimerAction
# from launch.events import Shutdown
from launch.event_handlers import OnExecutionComplete, OnProcessExit

def generate_launch_description():
    pkg_share = get_package_share_directory('simulation_launch')

    world_file_path = '' # empty world
    robot_model_file_path = os.path.join(pkg_share, 'urdf', 'wheelchair.urdf')

    # launch configuration variables
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    x_pose = LaunchConfiguration('x_pose', default='0.0')
    y_pose = LaunchConfiguration('y_pose', default='0.0')
    world_file = LaunchConfiguration('world_file', default=world_file_path)
    robot_model_file = LaunchConfiguration('robot_model_file', default=robot_model_file_path)
    
    robot_world_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, 'launch', 'robot_world.launch.py')
        ),
        launch_arguments={
            'x_pose': x_pose,
            'y_pose': y_pose,
            'use_sim_time': use_sim_time,
            'robot_model_file': robot_model_file,
            'world_file': world_file
        }.items()
    )

    test_camera_fps_gtest = '/home/dev_ws/build/bird_view/test/test_camera_fps'

    test_camera_fps = TimerAction(
        period=5.0,
        actions=[ExecuteProcess(
            cmd=[test_camera_fps_gtest],
            output='screen',
            shell=True
        )]
    )

    return LaunchDescription([
        robot_world_launch,
        test_camera_fps,
        RegisterEventHandler(
            event_handler=OnExecutionComplete(
                target_action=test_camera_fps,
                on_completion=[Shutdown()]
            )
        )
    ])

