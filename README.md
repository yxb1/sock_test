# sock_test
A TCP / UDP program supporting multiple test scenarios
The current communication protocol only supports UDP. TCP will be supported later.

## How to compile
Firstly, you need download zip source code or git clone source code.After comple, you can find binary file in output/bin
```
cd ~
mkdir myTest
cd myTest
git clone https://github.com/yxb1/sock_test.git
cd test
mkdir build
cd build
cmake ..
make
```
## Test Result
You can view results of test and a lots of xmls on https://mubu.com/doc/4B4YZ6cP-rg
 
## How to run
You have to pass the path of an XML file to the program. There are many configure parameters in the xml file. Through this file, we can flexibly configure some parameters of the program, such as src_ip, src_port, dst_port, dst_ip, interval of send in s/ms/us.
> ./sock_test server.xml

A xml file "server.xml" as bellow.
```
<?xml version="1.0" encoding="UTF-8"?>
<task_info_list>
	<version>1.0.0</version>
	<author>makou_yuan@163.com</author>
	<build_time>2021-11-13 13:00:00</build_time>
	<task_info>
		<protocol identify="server">udp</protocol>
		<src_ip>172.16.5.89</src_ip>
		<src_port>55100</src_port>
		<dst_ip>172.16.5.84</dst_ip>
		<dst_port>61017</dst_port>
		<interval unit="us">100</interval>
		<!-- 1.data_info=data 就是直接由测试程序负责造数据 -->
		<!-- 2.data_info=file 测试程序直接读bin文件，意思就是文件中是数据流。len是一次性读多少数据出来 -->
		<data_info len="1100">data</data_info>
		<!-- <data_info len="1100">data</data_info> -->
		<!-- <data_info file_path="../file/test.bin" len="1100">file</data_info> -->
		<fun_property dt_src="self">just_send</fun_property>
	</task_info>
</task_info_list>
```

## Supported scenarios
##### One device sends and one device receives
server.xml
```
...
	<task_info>
		<protocol identify="server">udp</protocol>
		<!-- 1.data_info=data The test program is directly responsible for creating data -->
		<!-- 2.data_info=file The test program directly reads the bin file, which means that the file is a data stream. How much data does len read at one time -->
		<data_info len="1100">data</data_info>
		<!-- <data_info len="1100">data</data_info> -->
		<!-- <data_info file_path="../file/test.bin" len="1100">file</data_info> -->
		<fun_property dt_src="self">just_send</fun_property>
	</task_info>
...
```
client.xml
```
...
	<task_info>
	    #If you need to count the packet loss rate, you can configure this option. If necessary, you can configure it as file or not config
		<data_info len="1100">data</data_info>
		<fun_property dt_src="self">just_recv</fun_property>
	</task_info>
...
```
##### Two devices transmit and receive at the same time, that is, full duplex communication
server.xml
```
...
	<task_info>
		<protocol identify="server">udp</protocol>
		<src_ip>172.16.5.89</src_ip>
		<src_port>55100</src_port>
		<dst_ip>172.16.5.84</dst_ip>
		<dst_port>61017</dst_port>
		<interval unit="ms">50</interval>
		<!-- 1.data_info=data 就是直接由测试程序负责造数据 -->
		<!-- 2.data_info=file 测试程序直接读bin文件，意思就是文件中是数据流。len是一次性读多少数据出来 -->
		<data_info file_path="../file/test.bin" len="1100">file</data_info>
		<!-- <data_info len="1100">data</data_info> -->
		<fun_property dt_src="self">send_recv</fun_property>
		<!-- <fun_property>just_recv</fun_property> -->
		<!-- <fun_property dt_src="self">send_recv</fun_property> -->
		<!-- <fun_property dt_src="opposite">send_recv</fun_property> -->
	</task_info>
...
```
client.xml
```
...
	<task_info>
	    #If you select self, the client will produce new data to the server. If you select opposite, a complete communication loop will be generated. data(server)->client->server
		<fun_property dt_src="self">send_recv</fun_property>
		<!-- <fun_property>just_recv</fun_property> -->
		<!-- <fun_property dt_src="self">send_recv</fun_property> -->
		<!-- <fun_property dt_src="opposite">send_recv</fun_property> -->
	</task_info>
...
```

---
The period of data communication is controlled by the server.
