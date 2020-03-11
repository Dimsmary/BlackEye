![image](https://github.com/Dimsmary/BlackEye/blob/master/Valkyrie_Dimsmary.jpg)

# 0 BlackEye

A ProJect aim to creat a **BlackEye**(Rainbow six)  
做一个彩虹六号里的 **黑眼**  

**I am a English noob*  
**这个Readme也就图一乐*

# 1 项目开发工具(Project Development Tools)

### 1.1 单片机开发(MCU Development)

单片机使用的是 Atmega 32U4 并使用 Arduino 进行开发。  
Atmega 32u4 对应 Arduino 的 Leonardo  

The MCU I used is atmega32u4, with development tool Arduino.  
Atmega32u4 in Arduino version is Leonardo.  

### 1.2 PCB绘制(Pcb Layout)

PCB绘制使用的是立创EDA  

PCB layout tool is LCEDA.

### 1.3 3D模型构建(3D Software)

SolidWorks.

# 2 文件内容介绍(File description)

### 2.1 3D_Model

包括了使用SolidWorks绘制的外壳源文件和用于3D打印的STL文件。  
Include solidworks source file and STL file for 3d printer.

### 2.2 Arduino_Code

包括了黑眼和控制器的Arduino源代码，不包括使用到的NRF/OLED库。  
Include BlackEye source code in Arduino.But not include NRF/OLED library.

### 2.3 PCB

包括了黑眼和控制器的PCB原理图和PCB图。  
Include BlackEye PCB file.

# 3 基本工作原理  

Arduino代码分为两个部分：Master & Slave, Master为控制板的程序，Slave为黑眼的程序。  

### 3.1 Master

#### 3.1.1 运行机制：

在控制板启动后，setup()部分将进行初始化。

1）初始化NRF模块，启动ACK模式。

2）初始化Analog IO: A0 A1。对应摇杆电位器。

3）初始化按钮中断。（按钮直接使用中断进行输入，而不是扫描。反正中断够用XD）

4）初始化OLED 并显示两秒的 Black Eye 图标。



初始化完成后，在loop()中控制板将不断通过NRF发送数据(command 7)，并企图获取ACK返回包。

如果返回包的数据返回正确则连接成功，画面进入工作模式。

ACK返回包中包括了以下信息：舵机速度，电池电压，图传开关状态，舵机开关状态。



不论是否连接成功，控制板仍会发送控制命令：

command 1/2/3/4 :用于控制舵机

command 5/6/8: 用于控制舵机电源 / 图传电源 / 更改舵机速度



如果超过3秒发送Command7无应答则判定为连接断开。



### 3.2 Master

#### 3.2.1 运行机制：

与Master类似，setup()进行NRF和IO初始化。A0获取电池电压，control1/2用于开关控制图传和舵机电源的MOS管。

在loop()中将不断接收Mster传来的命令，处理后通过ack返回。

# 更新记录(Updates)

### 2019/12/11

黑眼可以用了！

### 2020/03/11  

更新了Readme：详细的工作机制
