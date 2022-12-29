# PICO(rp2040) motor control via micro-ROS

## Prepare

### Hardware

- [Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- [L298N motor Driver](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/)

### Connect Solution

- Encoder

    - GPIO 10
    - GPIO 11

- PWM for L298N motor control
    
    - GPIO 6

## Getting Started

[Detailed Tutorial](https://me.w0x7ce.eu/rp2040/micro-ROS-on-RP2040)

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

Second, make sure microros freeRTOS and pico-example have configured

```bash
git clone https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk ~/micro_ROS_SDK_PATH
export micro_ROS_SDK_PATH=~/micro_ROS_SDK_PATH

git clone https://github.com/raspberrypi/pico-examples ~/pico-examples
export pico_examples_PATH=~/pico-examples
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
cp pico_micro_ros_motor_control.uf2 /media/$USER/RPI-RP2
```

## Start Micro-ROS Agent

Micro-ROS follows the client-server architecture, so you need to start the Micro-ROS Agent.
You can do so using the [micro-ros-agent Docker](https://hub.docker.com/r/microros/micro-ros-agent):
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:humble serial --dev /dev/ttyACM0 -b 115200
```

## Remote Control

### RCLPY

#### Configure

```bash
cd ~/1_ros/PC_Control/src
ros2 pkg create motor_control_rclcpp  --build-type ament_python --dependencies rclpy
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


### PySide6 GUI Control

```bash
git clone https://github.com/tianrking/1_ros -b GUI Pico_Control
cd Pico_Control
pip install PySide6
source /opt/ros/humble/setup.bash
python3 main.py
```