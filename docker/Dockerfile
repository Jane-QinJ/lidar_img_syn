# 使用 Ubuntu 18.04 作为基础镜像
FROM ubuntu:18.04

# 设置时区，防止安装软件时报错
ENV TZ=Asia/Shanghai
RUN ln -sf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone

# 更新系统并安装必要的工具
RUN apt-get update && apt-get install -y \
    software-properties-common \
    curl \
    git \
    wget \
    lsb-release \
    sudo && \
    rm -rf /var/lib/apt/lists/*

# 添加 ROS 源并安装 ROS Melodic
RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
RUN apt-get update && apt-get install -y \
    ros-melodic-desktop-full \
    python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential && \
    rm -rf /var/lib/apt/lists/*

# 初始化 rosdep
RUN rosdep init && rosdep update

# 设置环境变量
SHELL ["/bin/bash", "-c"]
RUN echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
ENV ROS_PACKAGE_PATH=/home/janeqin/code/lidar_img_syn_ws:$ROS_PACKAGE_PATH
WORKDIR /home/janeqin/code/lidar_img_syn_ws

# 安装编译依赖
RUN apt-get update && apt-get install -y \
    cmake \
    gcc \
    g++ \
    libpcl-dev \
    libopencv-dev \
    ros-melodic-pcl-ros \
    ros-melodic-cv-bridge \
    ros-melodic-roscpp \
    ros-melodic-std-msgs \
    ros-melodic-sensor-msgs \
    ros-melodic-message-filters \
    && rm -rf /var/lib/apt/lists/*


# Clone the repository from GitHub
RUN git clone https://github.com/Jane-QinJ/lidar_img_syn.git /home/janeqin/code/lidar_img_syn_ws

# 编译项目
RUN source /opt/ros/melodic/setup.bash && \
    cd /home/janeqin/code/lidar_img_syn_ws && catkin_make

# 进入容器后自动加载 ROS 环境
CMD ["/bin/bash", "-c", "source /opt/ros/melodic/setup.bash && exec bash"]
