############
MAILBOX
############

Duo 提供1组8通道Mailbox，用于大核C906B和小核C906L间的通信

在大核上也就是Linux系统中，参考程序详见: rtoscore_

.. _rtoscore: https://milkv.io/docs/duo/getting-started/rtoscore

使用的消息结构体如下:

::

    struct valid_t {
	    unsigned char linux_valid;
	    unsigned char rtos_valid;
    } __attribute__((packed));

    typedef union resv_t {
	    struct valid_t valid;
	    unsigned short mstime; // 0 : noblock, -1 : block infinite
    } resv_t;

    typedef struct cmdqu_t cmdqu_t;
    /* cmdqu size should be 8 bytes because of mailbox buffer size */
    struct cmdqu_t {
	    unsigned char ip_id;
	    unsigned char cmd_id : 7;
	    unsigned char block : 1;
	    union resv_t resv;
	    unsigned int  param_ptr;
    } __attribute__((packed)) __attribute__((aligned(0x8)));

注意，接收到的消息要和 ``cmdqu_t`` 进行映射，才具有意义

在arduino中，实现了以下接口

描述
======

mailbox 初始化函数

语法
======

.. code-block:: arduino

    void mailbox_init (bool master);

参数
======

    * ``master`` 是否进行硬件初始化，默认false

返回值
========

无

描述
======

mailbox 回调函数注册

语法
======

.. code-block:: arduino

    void mailbox_register (int channel, void(*callback)(MailboxMsg));

参数
======

    * ``channel`` 注册的通道号

    * ``void(*callback)(MailboxMsg)`` 回调函数，参数为MailboxMsg对象

返回值
========

无

描述
======

mailbox 回调函数取消注册

语法
======

.. code-block:: arduino

     void mailbox_unregister (int channel);

参数
======

    * ``channel`` 取消注册的通道号

返回值
========

无

描述
======

mailbox 使能接收通道

语法
======

.. code-block:: arduino

    void mailbox_enable_receive (int channel);

参数
======

    * ``channel`` 通道号

返回值
========

无
    
描述
======

mailbox 关闭使能接收通道

语法
======

.. code-block:: arduino

    void mailbox_disable_receive (int channel);

参数
======

    * ``channel`` 通道号

返回值
========

无

描述
======

mailbox 发送消息

语法
======

.. code-block:: arduino

    void mailbox_write(MailboxMsg& msg);

参数
======

    * ``msg`` 需要写入的消息和通道

返回值
========

无

    


使用示例：

::
    
    #include "mailbox.h"

    void showmsg(MailboxMsg msg)
    {
        Serial2.print("showmsg: ");
        Serial2.println(*(msg.data), HEX);
        *(msg.data) = 0;  //消息接收后需要手动清空
    }

    void setup() {
        // put your setup code here, to run once:
        Serial2.begin(115200);
        mailbox_init (false);
        mailbox_register (0, showmsg);
        mailbox_enable_receive (0);
    }

    void loop() {
        // put your main code here, to run repeatedly:

    }