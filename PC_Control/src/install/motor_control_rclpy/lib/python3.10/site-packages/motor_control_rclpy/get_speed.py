#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32

class NodeSubscribe02(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("%s!" % name)
        # 创建订阅者
        self.command_subscribe_ = self.create_subscription(Int32,"pico_publisher_encoder",self.command_callback,10)

    def command_callback(self,msg):
        speed = 0.0
        if msg.data=="backup":
            speed = -0.2
        self.get_logger().info(f'recieve[{msg.data}]，message_recieve{speed}')

def main(args=None):
    rclpy.init(args=args) # 初始化rclpy
    node = NodeSubscribe02("topic_subscribe_02")  # 新建一个节点
    rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() # 关闭rclpy
