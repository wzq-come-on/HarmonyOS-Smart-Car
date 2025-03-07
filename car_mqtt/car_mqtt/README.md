**摘要**：本文介绍如何远程控制OpenHarmony小车

**适合群体**：适用于润和Hi3861开发板

文中所有代码仓库：[https://github.com/wzq-come-on/HarmonyOS-Smart-Car.git]

## 1 小车介绍

基于鸿蒙系统 + Hi3861 的WiFi小车

首先，我们得有一套WiFi小车套件，其实也是Hi3861 加上电机、循迹模块、超声波等模块。

小车安装完大概是这样：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3e2bf6202ffa4d2a882cece10c5ed814.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBA6L-e5b-X5a6J55qE5Y2a5a6i,size_20,color_FFFFFF,t_70,g_se,x_16)


## 2 如何使用

### （1）下载MQTT软件包
MQTT 是当前最主流的物联网通信协议，需要物联网云平台，例如华为云、阿里云、移动OneNET都支持mqtt。而Hi3861则是一款专为IoT应用场景打造的芯片。本节主要讲如何在鸿蒙系统中通过移植第3方软件包 paho mqtt去实现MQTT协议功能，最后会给出测试验证。为后续的物联网项目打好基础。

已经移植好的MQTT源码： https://gitee.com/qidiyun/harmony_mqtt

将下载后的文件放到 third_party 下并重命名为 pahomqtt

### （2）移植car_mqtt包
将car_mqtt文件移植到app目录下，调整外层的BUILD.gn的调用即可。

### （3）小车热点设置
热点的名字和密码在sta_entry.c的宏定义部分中
需要自己修改成自己的热点。

### （3）PC上位机下载
PC上位机采用C#开发，源码随后上传。 
