# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'untitled.ui'
##
## Created by: Qt User Interface Compiler version 6.4.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QTimer, QUrl, Qt) ## QTime vs QTimer ?
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QDateTimeEdit, QDial,
    QDoubleSpinBox, QLCDNumber, QLabel, QLineEdit,
    QMainWindow, QMenu, QMenuBar, QPushButton,
    QSizePolicy, QSlider, QStatusBar, QTextEdit,
    QWidget)

from PySide6.QtCore import Slot

import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32

import random

from common import global_var as gl
gl._init()

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

# @Slot()
# def get_topic():
#     print("单击按钮, Hello!")

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(706, 600)
        self.actionAuthor = QAction(MainWindow)
        self.actionAuthor.setObjectName(u"actionAuthor")
        self.actionMode = QAction(MainWindow)
        self.actionMode.setObjectName(u"actionMode")
        self.actionAuthor_2 = QAction(MainWindow)
        self.actionAuthor_2.setObjectName(u"actionAuthor_2")
        self.actionAbout = QAction(MainWindow)
        self.actionAbout.setObjectName(u"actionAbout")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(40, 100, 67, 17))
        self.label_2 = QLabel(self.centralwidget)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(40, 140, 67, 17))
        self.label_3 = QLabel(self.centralwidget)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setGeometry(QRect(40, 180, 67, 17))
        self.label_4 = QLabel(self.centralwidget)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setGeometry(QRect(40, 220, 67, 17))
        self.lineEdit_4 = QLineEdit(self.centralwidget)
        self.lineEdit_4.setObjectName(u"lineEdit_4")
        self.lineEdit_4.setGeometry(QRect(130, 220, 113, 25))
        self.pushButton = QPushButton(self.centralwidget)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setGeometry(QRect(40, 260, 231, 25))
        self.pushButton_2 = QPushButton(self.centralwidget)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setGeometry(QRect(30, 40, 89, 25))
        self.pushButton_3 = QPushButton(self.centralwidget)
        self.pushButton_3.setObjectName(u"pushButton_3")        
        self.pushButton_3.setGeometry(QRect(150, 40, 89, 25))
        self.horizontalSlider = QSlider(self.centralwidget)
        self.horizontalSlider.setObjectName(u"horizontalSlider")
        self.horizontalSlider.setGeometry(QRect(150, 100, 141, 21))
        self.horizontalSlider.setOrientation(Qt.Horizontal)
        self.horizontalSlider_2 = QSlider(self.centralwidget)
        self.horizontalSlider_2.setObjectName(u"horizontalSlider_2")
        self.horizontalSlider_2.setGeometry(QRect(150, 140, 141, 21))
        self.horizontalSlider_2.setOrientation(Qt.Horizontal)
        self.horizontalSlider_3 = QSlider(self.centralwidget)
        self.horizontalSlider_3.setObjectName(u"horizontalSlider_3")
        self.horizontalSlider_3.setGeometry(QRect(150, 180, 141, 21))
        self.horizontalSlider_3.setOrientation(Qt.Horizontal)
        self.doubleSpinBox = QDoubleSpinBox(self.centralwidget)
        self.doubleSpinBox.setObjectName(u"doubleSpinBox")
        self.doubleSpinBox.setGeometry(QRect(70, 100, 65, 26))
        self.doubleSpinBox_2 = QDoubleSpinBox(self.centralwidget)
        self.doubleSpinBox_2.setObjectName(u"doubleSpinBox_2")
        self.doubleSpinBox_2.setGeometry(QRect(70, 140, 65, 26))
        self.doubleSpinBox_3 = QDoubleSpinBox(self.centralwidget)
        self.doubleSpinBox_3.setObjectName(u"doubleSpinBox_3")
        self.doubleSpinBox_3.setGeometry(QRect(70, 180, 65, 26))
        self.comboBox = QComboBox(self.centralwidget)
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.setObjectName(u"comboBox")
        self.comboBox.setGeometry(QRect(280, 220, 101, 25))
        self.comboBox_2 = QComboBox(self.centralwidget)
        self.comboBox_2.setObjectName(u"comboBox_2")
        self.comboBox_2.setGeometry(QRect(280, 40, 86, 25))
        self.comboBox_3 = QComboBox(self.centralwidget)
        self.comboBox_3.setObjectName(u"comboBox_3")
        self.comboBox_3.setGeometry(QRect(420, 40, 86, 25))
        self.dial = QDial(self.centralwidget)
        self.dial.setObjectName(u"dial")
        self.dial.setGeometry(QRect(540, 110, 50, 64))
        self.dial_2 = QDial(self.centralwidget)
        self.dial_2.setObjectName(u"dial_2")
        self.dial_2.setGeometry(QRect(610, 110, 50, 64))
        self.lcdNumber = QLCDNumber(self.centralwidget)
        self.lcdNumber.setObjectName(u"lcdNumber")
        self.lcdNumber.setGeometry(QRect(620, 180, 64, 23))
        self.lcdNumber_2 = QLCDNumber(self.centralwidget)
        self.lcdNumber_2.setObjectName(u"lcdNumber_2")
        self.lcdNumber_2.setGeometry(QRect(540, 180, 64, 23))
        self.dateTimeEdit = QDateTimeEdit(self.centralwidget)
        self.dateTimeEdit.setObjectName(u"dateTimeEdit")
        self.dateTimeEdit.setGeometry(QRect(540, 40, 194, 26))
        self.dateTimeEdit_2 = QDateTimeEdit(self.centralwidget)
        self.dateTimeEdit_2.setObjectName(u"dateTimeEdit_2")
        self.dateTimeEdit_2.setGeometry(QRect(540, 80, 194, 26))
        self.textEdit = QTextEdit(self.centralwidget)
        self.textEdit.setObjectName(u"textEdit")
        self.textEdit.setGeometry(QRect(440, 230, 321, 70))
        self.label_5 = QLabel(self.centralwidget)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setGeometry(QRect(440, 210, 71, 17))
        self.pushButton_4 = QPushButton(self.centralwidget)
        self.pushButton_4.setObjectName(u"pushButton_4")
        self.pushButton_4.setGeometry(QRect(290, 260, 89, 25))
        self.label_6 = QLabel(self.centralwidget)
        self.label_6.setObjectName(u"label_6")
        self.label_6.setGeometry(QRect(360, 90, 111, 17))
        self.horizontalSlider_4 = QSlider(self.centralwidget)
        self.horizontalSlider_4.setObjectName(u"horizontalSlider_4")
        self.horizontalSlider_4.setGeometry(QRect(330, 130, 160, 16))
        self.horizontalSlider_4.setOrientation(Qt.Horizontal)
        self.lcdNumber_3 = QLCDNumber(self.centralwidget)
        self.lcdNumber_3.setObjectName(u"lcdNumber_3")
        self.lcdNumber_3.setGeometry(QRect(330, 180, 64, 23))
        self.lcdNumber_4 = QLCDNumber(self.centralwidget)
        self.lcdNumber_4.setObjectName(u"lcdNumber_4")
        self.lcdNumber_4.setGeometry(QRect(420, 180, 64, 23))
        self.label_7 = QLabel(self.centralwidget)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setGeometry(QRect(350, 160, 31, 17))
        self.label_8 = QLabel(self.centralwidget)
        self.label_8.setObjectName(u"label_8")
        self.label_8.setGeometry(QRect(430, 160, 31, 17))
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 706, 22))
        self.menuAbout = QMenu(self.menubar)
        self.menuAbout.setObjectName(u"menuAbout")
        self.menuAbout_2 = QMenu(self.menubar)
        self.menuAbout_2.setObjectName(u"menuAbout_2")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuAbout.menuAction())
        self.menubar.addAction(self.menuAbout_2.menuAction())
        self.menuAbout.addAction(self.actionMode)
        self.menuAbout_2.addAction(self.actionAuthor)
        self.menuAbout_2.addAction(self.actionAuthor_2)
        self.menuAbout_2.addAction(self.actionAbout)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.actionAuthor.setText(QCoreApplication.translate("MainWindow", u"Help", None))
        self.actionMode.setText(QCoreApplication.translate("MainWindow", u"Mode", None))
        self.actionAuthor_2.setText(QCoreApplication.translate("MainWindow", u"Author", None))
        self.actionAbout.setText(QCoreApplication.translate("MainWindow", u"About", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"KP", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"kI", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"kD", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Protocol", None))
        self.pushButton.setText(QCoreApplication.translate("MainWindow", u"Micro-ROS Agent Start/reStart", None))
        self.pushButton_2.setText(QCoreApplication.translate("MainWindow", u"Send", None))
        self.pushButton_3.setText(QCoreApplication.translate("MainWindow", u"Topic", None))
        self.comboBox.setItemText(0, QCoreApplication.translate("MainWindow", u"micro-ROS", None))
        self.comboBox.setItemText(1, QCoreApplication.translate("MainWindow", u"UART", None))
        self.comboBox.setItemText(2, QCoreApplication.translate("MainWindow", u"IIC", None))
        self.comboBox.setItemText(3, QCoreApplication.translate("MainWindow", u"SPI", None))
        self.comboBox.setItemText(4, QCoreApplication.translate("MainWindow", u"CAN", None))

        self.label_5.setText(QCoreApplication.translate("MainWindow", u"Command", None))
        self.pushButton_4.setText(QCoreApplication.translate("MainWindow", u"Reset", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Target_Speed", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"Set", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Now", None))
        self.menuAbout.setTitle(QCoreApplication.translate("MainWindow", u"Setting", None))
        self.menuAbout_2.setTitle(QCoreApplication.translate("MainWindow", u"More", None))
    # retranslateUi
    
    def add_slot(self):
    
        # clicked.connect(GET_ENCODE_VALUE)
        self.pushButton_3.clicked.connect(self.get_topic_list)
        self.pushButton_2.clicked.connect(self.publish_data)
        
        pass
    
    def add_timer(self):
        self._timer = QTimer()
        self._timer.timeout.connect(self.handleTimeout)
        self._timer.setInterval(100)
        self._timer.start()
        
    @Slot()
    def handleTimeout(self):
        
        msg = Int32()  
        msg.data = int(self.horizontalSlider.value())
        self.ros_node_publish_data_publisher_PID_change.publish(msg) 

    
    def ros_init(self):
        
        rclpy.init() # 初始化rclpy
        self.ros_node_publish_data = Node("tt_2")
        self.ros_node_publish_data_publisher_  = self.ros_node_publish_data.create_publisher(Int32,"/speed_change", 10) 
        self.ros_node_publish_data_publisher_PID_change = self.ros_node_publish_data.create_publisher(Int32,"/PID_change", 10)
        
    
    @Slot()
    def get_topic_list(self):

        # rclpy.init()
        node_dummy = Node("_ros2cli_show_topic_list")
        topic_list = node_dummy.get_topic_names_and_types()
        node_dummy.destroy_node()
        for info in topic_list:
            print(info[0])

    
    @Slot()
    def get_node_list(self):

        # rclpy.init()
        node_dummy = Node("_ros2cli_dummy_to_show_node_list")
        topic_list = node_dummy.get_node_names_and_types()
        node_dummy.destroy_node()
        for info in topic_list:
            print(info[0])
            
        
    @Slot()
    def publish_data(self):
        
        # rclpy.init() # 初始化rclpy
        # node_publish_data = Node("tt_2")
        try:
            self.ros_node_publish_data_publisher_ = self.ros_node_publish_data.create_publisher(Int32,"/speed_change", 10) 
        except:
            pass
        
        msg = Int32()  
        msg.data = int(gl.get_value('speed'))
        self.ros_node_publish_data_publisher_.publish(msg) 
        
        # msg = Int32()  
        # msg.data = int(self.horizontalSlider.value())
        # self.ros_node_publish_data_publisher_PID_change.publish(msg) 
        
        # node = NodePublisher02("topic_publisher_02")  # 新建一个节点
        # rclpy.spin(node) # 保持节点运行，检测是否收到退出指令（Ctrl+C）
        # node_publish_data.destroy_node()
        # rclpy.shutdown() # 关闭rclpy