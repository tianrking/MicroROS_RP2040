from setuptools import setup

package_name = 'motor_control_rclpy'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='w0x7ce',
    maintainer_email='tian.r.king@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "get_speed = motor_control_rclpy.get_speed:main",
            "change_speed = motor_control_rclpy.change_speed:main"      
        ],
    },
)
