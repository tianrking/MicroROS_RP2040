import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32

kk = 1

class NodePublisher02(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s!" % name)
        self.command_publisher_ = self.create_publisher(Int32,"/speed_change", 10) 
        self.timer = self.create_timer(0.5, self.timer_callback)
    
    def timer_callback(self):
        """
        定时器回调函数
        """
        # msg = String()
        msg = Int32()
            
        msg.data = 50
        self.command_publisher_.publish(msg) 
        self.get_logger().info(f'发布了指令：{msg.data}')    #打印一下发布的数据

def main(args=None):
    rclpy.init(args=args) # 初始化rclpy
    node = NodePublisher02("topic_publisher_02")  # 新建一个节点
    rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    rclpy.shutdown() # 关闭rclpy