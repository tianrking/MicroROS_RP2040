# PICO(rp2040) motor control via micro-ROS

## Getting Started

```bash
git clone https://github.com/tianrking/1_ros ~/1_ros
```

## Dependencies

First, make sure the Pico SDK is properly installed and configured:

```bash
# Install dependencies
sudo apt install cmake g++ gcc-arm-none-eabi doxygen libnewlib-arm-none-eabi git python3
git clone --recurse-submodules https://github.com/raspberrypi/pico-sdk.git $HOME/pico-sdk

# Configure environment
echo "export PICO_SDK_PATH=$HOME/pico-sdk" >> ~/.bashrc
source ~/.bashrc

```

## Build

```bash
cd ~/1_ros
mkdir build
cd build
cmake ..
make
```

## Flash 

To flash, hold the boot button, plug the USB and run:

```bash
cp pico_micro_ros_example.uf2 /media/$USER/RPI-RP2
```

## Start Micro-ROS Agent

Micro-ROS follows the client-server architecture, so you need to start the Micro-ROS Agent.
You can do so using the [micro-ros-agent Docker](https://hub.docker.com/r/microros/micro-ros-agent):
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:humble serial --dev /dev/ttyACM0 -b 115200
```

## What files are relevant?
- `pico_uart_transport.c`: Contains the board specific implementation of the serial transport (no change needed).
- `CMakeLists.txt`: CMake file.
- `pico_micro_ros_motor_control.c`: Our code .

## Remote Control

### RCLPY

#### Configure

```bash
cd ~/1_ros/PC_Control/src
ros2 pkg create example_topic_rclpy  --build-type ament_python --dependencies rclpy
```

```bash
cd example_topic_rclpy/example_topic_rclpy
touch topic_subscribe_02.py
touch topic_publisher_02.py
```

```bash
cd ~/1_ros/PC_Control/
colcon build
source install/setup.bash
```

#### RUN

```bash
ros2 run motor_control_rclpy change_speed
ros2 run motor_control_rclpy get_speed
```

### RCLCPP

Todo