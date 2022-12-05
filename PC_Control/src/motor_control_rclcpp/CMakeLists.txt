cmake_minimum_required(VERSION 3.8)
project(motor_control_rclcpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(get_speed src/get_speed.cpp)
add_executable(change_speed src/change_speed.cpp)

install(TARGETS
  get_speed
  change_speed
  DESTINATION lib/${PROJECT_NAME}
)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

ament_target_dependencies(get_speed rclcpp std_msgs)
ament_target_dependencies(get_speed rclcpp)

ament_target_dependencies(change_speed rclcpp std_msgs)
ament_target_dependencies(change_speed rclcpp)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
