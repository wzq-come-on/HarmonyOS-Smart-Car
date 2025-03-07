**摘要**：本文介绍如何远程控制OpenHarmony小车

**适合群体**：适用于润和Hi3861开发板

文中所有代码仓库：[https://github.com/wzq-come-on/HarmonyOS-Smart-Car.git]

## 1 小车介绍

基于鸿蒙系统 + Hi3861 的WiFi小车

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
