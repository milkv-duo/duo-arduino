######
开始
######


关于Arduino
##################

简介
=======

    Arduino是一款便捷灵活、方便上手的开源电子原型平台。\
    包含硬件（各种型号的Arduino板）和软件（ArduinoIDE）。由一个欧洲开发团队于2005年冬季开发。\
    其成员包括Massimo Banzi、David Cuartielles、Tom Igoe、Gianluca Martino、David Mellis和Nicholas Zambetti等。

    它构建于开放原始码simple I/O介面版，并且具有使用类似Java、C语言的Processing/Wiring开发环境。\
    主要包含两个部分：硬件部分是可以用来做电路连接的Arduino电路板；另外一个则是Arduino IDE，你的计算机中的程序开发环境。\
    你只要在IDE中编写程序代码，将程序上传到Arduino电路板后，程序便会告诉Arduino电路板要做些什么了。

    Arduino能通过各种各样的传感器来感知环境，通过控制灯光、马达和其他的装置来反馈、影响环境。\
    板子上的微控制器可以通过Arduino的编程语言来编写程序，编译成二进制文件，烧录进微控制器。\
    对Arduino的编程是通过 Arduino编程语言 (基于 Wiring)和Arduino开发环境(基于 Processing)来实现的。\
    基于Arduino的项目，可以只包含Arduino，也可以包含Arduino和其他一些在PC上运行的软件，它们之间进行通信 (比如 Flash, Processing, MaxMSP)来实现。


Arduino特性：
==================

  - 跨平台

    Arduino IDE可以在Windows、Macintosh OS(Mac OS)、Linux三大主流操作系统上运行，而其他的大多数控制器只能在Windows上开发。

  - 简单清晰

    Arduino IDE基于processing IDE开发。对于初学者来说，极易掌握，同时有着足够的灵活性。\
    Arduino语言基于wiring语言开发，是对 avr-gcc库的二次封装，不需要太多的单片机基础、编程基础，简单学习后，你也可以快速的进行开发。

  - 开放性

    Arduino的硬件原理图、电路图、IDE软件及核心库文件都是开源的，在开源协议范围内里可以任意修改原始设计及相应代码。

  - 发展迅速

    Arduino不仅仅是全球最流行的开源硬件，也是一个优秀的硬件开发平台，更是硬件开发的趋势。\
    Arduino简单的开发方式使得开发者更关注创意与实现，更快的完成自己的项目开发，大大节约了学习的成本，缩短了开发的周期。\
    因为Arduino的种种优势，越来越多的专业硬件开发者已经或开始使用Arduino来开发他们的项目、产品；\
    越来越多的软件开发者使用Arduino进入硬件、物联网等开发领域；大学课题，自动化、软件，甚至艺术专业，也纷纷开展了Arduino相关课程。


Arduino Sg200x
##################

    该项目是为Sg200x支持Arduino而建立，可以适配绝大多数Arduino的API, 要查看 Arduino 参考文档， 请考虑阅读官方文档。

    Arduino官方文档：Arduino参考_。

    Arduino Sg200x模块的API文档位于 API文档_ 。


    Arduino Sg200x项目源码地址：https://github.com/xxx/arduino-sophgo。


.. _Arduino参考: https://www.arduino.cc/reference/en/

.. _API文档: ../libraries.html

