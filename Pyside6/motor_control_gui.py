import PySide6.QtCore

import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui

from PySide6.QtCharts import QChart, QSplineSeries, QValueAxis ,QChartView
from PySide6.QtCore import Qt, QTimer, Slot
from PySide6.QtGui import QPen

from PySide6.QtGui import QPainter
from PySide6.QtWidgets import QApplication, QMainWindow

from chart import Chart
import chart

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import Int32

kk = 1

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.hello = ["1", "2", "3", "4"]

        self.button = QtWidgets.QPushButton("Click me!")
        self.text = QtWidgets.QLabel("Hello World",
                                     alignment=QtCore.Qt.AlignCenter)

        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.addWidget(self.text)
        self.layout.addWidget(self.button)

        self.button.clicked.connect(self.magic)

    @QtCore.Slot()
    def magic(self):
        self.text.setText(random.choice(self.hello))
        chart.speed_temp = random.randint(0,9)
        
        

class NodePublisher02(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("%s!" % name)
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
        self.get_logger().info(f'Value：{msg.data}')    #打印一下发布的数据

def main(args=None):
    # rclpy.init(args=args) # 初始化rclpy
    # node = NodePublisher02("topic_publisher_02")  # 新建一个节点
    # rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
    # rclpy.shutdown() # 关闭rclpy
    
    app = QtWidgets.QApplication([])

    widget = MyWidget()
    widget.resize(800, 600)
    widget.show()
    
    window = QMainWindow()
    chart = Chart()

    chart.setTitle("Speed chart")
    chart.legend().hide()
    chart.setAnimationOptions(QChart.AllAnimations)
    chart_view = QChartView(chart)
    chart_view.setRenderHint(QPainter.Antialiasing)
    window.setCentralWidget(chart_view)
    window.resize(400, 300)
    window.show()

    sys.exit(app.exec())
    
    # Prints PySide6 version
    print(PySide6.__version__)

    # Prints the Qt version used to compile PySide6
    print(PySide6.QtCore.__version__)

main()