FROM nvidia/cuda:12.6.0-devel-ubuntu24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

# ROS2 Jazzy
RUN apt-get update && apt-get install -y curl gnupg && \
    curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
        -o /usr/share/keyrings/ros-archive-keyring.gpg && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
        http://packages.ros.org/ros2/ubuntu noble main" \
        > /etc/apt/sources.list.d/ros2.list && \
    apt-get update && apt-get install -y \
        ros-jazzy-ros-base \
        ros-jazzy-rosbag2-cpp \
        ros-jazzy-sensor-msgs \
        ros-jazzy-tf2 \
        ros-jazzy-tf2-msgs \
        ros-jazzy-pcl-conversions \
        ros-jazzy-pcl-ros \
        ros-jazzy-cv-bridge \
        ros-jazzy-image-transport \
        ros-jazzy-nav-msgs \
        ros-jazzy-std-msgs \
        ros-jazzy-rosbag2-transport \
        ros-jazzy-rosbag2-storage \
        ros-jazzy-rcl-interfaces \
        python3-vcstool \
        python3-colcon-common-extensions

# system deps
RUN apt-get update && apt-get install -y \
    libeigen3-dev \
    libopencv-dev \
    libsuitesparse-dev \
    libglfw3-dev \
    libgl-dev \
    freeglut3-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    liblz4-dev \
    libzstd-dev \
    libqglviewer-dev-qt5 \
    libgl1-mesa-dri \
    mesa-utils \
    cmake \
    build-essential \
    git \
    emacs

# -------------------------------------------------------------------
FROM base AS cpu

WORKDIR /ws/src
RUN git clone -b ros2 https://gitlab.com/srrg-software/srrg_cmake_modules.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_core.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_solver.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_qgl_viewport.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_config_visualizer.git

WORKDIR /ws
RUN . /opt/ros/jazzy/setup.sh && \
    colcon build \
        --cmake-args -DHAVE_CUDA=OFF -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 \
        --packages-select \
            srrg_cmake_modules srrg2_core_msgs srrg2_core srrg2_core_ros \
            srrg2_qgl_viewport srrg2_solver srrg2_solver_extras

RUN git clone https://github.com/rvp-group/kd_slam2.git /tmp/kd_slam && \
    mv /tmp/kd_slam/kd_slam2 /ws/src/kd_slam2 && \
    mv /tmp/kd_slam/kd_slam2_msgs /ws/src/kd_slam2_msgs && \
    mv /tmp/kd_slam/kd_eval_tools /ws/src/kd_eval_tools && \
    rm -rf /tmp/kd_slam

RUN bash /ws/src/kd_slam2/kd_slam2/configs/make_cpu_confs.sh

RUN . /opt/ros/jazzy/setup.sh && \
    . /ws/install/setup.sh && \
    colcon build \
        --cmake-args -DHAVE_CUDA=OFF -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 \
        --packages-select kd_slam2_msgs kd_slam2 kd_eval_tools

RUN echo "source /opt/ros/jazzy/setup.bash" >> /etc/bash.bashrc && \
    echo "source /ws/install/setup.bash" >> /etc/bash.bashrc

WORKDIR /

# -------------------------------------------------------------------
FROM base AS cuda

ENV CPATH=/usr/local/cuda/include
ENV LIBRARY_PATH=/usr/local/cuda/lib64

WORKDIR /ws/src
RUN git clone -b ros2 https://gitlab.com/srrg-software/srrg_cmake_modules.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_core.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_solver.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_qgl_viewport.git && \
    git clone -b ros2 https://gitlab.com/srrg-software/srrg2_config_visualizer.git

WORKDIR /ws
RUN . /opt/ros/jazzy/setup.sh && \
    colcon build \
        --cmake-args -DHAVE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=all-major \
            -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 \
        --packages-select \
            srrg_cmake_modules srrg2_core_msgs srrg2_core srrg2_core_ros \
            srrg2_qgl_viewport srrg2_solver srrg2_solver_extras

RUN git clone https://github.com/rvp-group/kd_slam2.git /tmp/kd_slam && \
    mv /tmp/kd_slam/kd_slam2 /ws/src/kd_slam2 && \
    mv /tmp/kd_slam/kd_slam2_msgs /ws/src/kd_slam2_msgs && \
    mv /tmp/kd_slam/kd_eval_tools /ws/src/kd_eval_tools && \
    rm -rf /tmp/kd_slam

RUN . /opt/ros/jazzy/setup.sh && \
    . /ws/install/setup.sh && \
    colcon build \
        --cmake-args -DHAVE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=all-major \
            -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 \
        --packages-select kd_slam2_msgs kd_slam2 kd_eval_tools

RUN echo "source /opt/ros/jazzy/setup.bash" >> /etc/bash.bashrc && \
    echo "source /ws/install/setup.bash" >> /etc/bash.bashrc

WORKDIR /
