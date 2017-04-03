# Hadoop-2.7.3 Pseudo Distributed on Archlinux
***
> ## 环境

* #### [Archlinux 4.x](https://www.archlinux.org)
* #### [Hadoop-2.7.3](http://hadoop.apache.org/releases.html)
* #### JDK7-openjdk

> ## Start

### 1. 创建用户名为hadoop的用户
`# useradd -m hadoop -s /bin/bash` *以root用户登陆创建*</br>
`# passwd hadoop` *设置hadoop用户的密码*</br>
 **为hadoop用户增加sudo权限:**</br>
 *sudoers文件如果为440，需要修改权限为640，否则不能对文件进行写入操作*</br>
`# vim /etc/sudoers` *找到root ALL=(ALL) ALL这行，在它下一行添加hadoop ALL=(ALL) ALL*</br>
### 2. 安装环境
0. 登录hadoop用户
1. 启用网络并安装SSH和JDK</br>
2. 配置SSH免密码登录</br>

  `$ ssh yourhostname` *'yourhostname'请换成自己的主机名*</br>
此时会有ssh首次登录提示，输入yes，然后按提示输入hadoop用户密码，然后退出配置免密码登录</br>
```shell
$ exitcd  # 退出刚才的ssh localhost
$ cd ~/.ssh/ # 没有该目录请先执行ssh yourhostname
$ ssh-keygen -t rsa # 出现的提示都按 Enter
$ cat id_rsa.pub >> authorized_keys # 加入授权
$ chmod 600 ./authorized_keys # 更改文件权限
```
</br>再次执行`$ ssh yourhostname`&nbsp;检查是否成功免密码登录ssh</br>
3. 配置Java环境变量

  `$ vim ~/.bashrc` </br>在文件最后添加一行*(指向JDK的安装位置,不同Linux发行版的安装目录可能不一样)*</br>
  `export JAVA_HOME=/usr/lib/jvm/java-1.7.0-openjdk` &nbsp;*保存并退出*

  `$ source ~/.bashrc` *使JAVA_HOME变量生效*</br>

  执行以下两条命令,若两次的输出一样,说明环境变量配置成功</br>
  `$ java -version`</br>
  `$ $JAVA_HOME/bin/java -version`
4. 安装Hadoop-2.7.3

  将Hadoop安装在/usr/local/中:

  ```shell
  $ sudo tar -zxvf /hadoop的.tar.gz包的位置 -C /usr/local # 解压到/usr/local/中
  $ cd /usr/local/
  $ sudo mv ./hadoop-2.7.3/ ./hadoop # 将文件夹名改为hadoop
  $ sudo chown -R hadoop:hadoop ./hadoop # 修改文件权限
  $ cd /usr/local/hadoop
  $ ./bin/hadoop version # 显示Hadoop版本信息则说明Hadoop安装成功
  ```

5. 配置hadoop环境变量

  在~/.bashrc中增加以下内容:
  ```shell
  # Hadoop Environment Variables
  export HADOOP_HOME=/usr/local/hadoop
  export HADOOP_INSTALL=$HADOOP_HOME
  export HADOOP_MAPRED_HOME=$HADOOP_HOME
  export HADOOP_MAPRED_HOME=$HADOOP_HOME
  export HADOOP_COMMON_HOME=$HADOOP_HOME
  export HADOOP_HDFS_HOME=$HADOOP_HOME
  export YARN_HOME=$HADOOP_HOME
  export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
  export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
  ```
  </br>保存并执行`$ source ~/.bashrc`使其生效

6. 修改hadoop配置文件

  `$ cd /usr/local/hadoop/etc/hadoop/`

   将文件core-site.xml修改为以下内容:

   ```XML
   <configuration>
     <property>
       <name>hadoop.tmp.dir</name>
       <value>file:/usr/local/hadoop/tmp</value>
       <description>Abase for other temporary directories.</description>
     </property>
     <property>
       <name>fs.defaultFS</name>
       <value>hdfs://yourhostname:9000</value>
     </property>
   </configuration>
   ```

   将文件hdfs-site.xml修改为以下内容:

   ```XML
   <configuration>
     <property>
       <name>dfs.replication</name>
       <value>1</value>
     </property>
     <property>
       <name>dfs.namenode.name.dir</name>
       <value>file:/usr/local/hadoop/tmp/dfs/name</value>
     </property>
     <property>
       <name>dfs.datanode.data.dir</name>
       <value>file:/usr/local/hadoop/tmp/dfs/data</value>
     </property>
   </configuration>
   ```

7. 启动Hadoop

  完成上述配置后，执行Namenode的格式化:

  `$ ./bin/hdfs namenode -format`

  在输出信息中找&nbsp;*"successfully formatted"* 和 *"Exitting with status 0"* ，若有则成功；若为 *"Exitting with status 1"* 则是出错。

  启动守护进程: </br>
  `$ ./sbin/start-dfs.sh` *出现 "(yes/no)?" 输yes*

  执行：</br>
  `$ jps` *若成功，则会有Jps NameNode DataNode SecondaryNameNode 这四个进程*

  **成功启动后可在浏览器输入&nbsp;http://yourhostname:50070 查看相关信息**</br>
  **请注意查看此页面中的live nodes 和 dead nodes 的数量,前者为0说明不成功**

> ## 运行伪分布式实例

1. 首先，在HDFS中创建用户目录:  

  `$ cd /usr/local/hadoop/` </br>

  `$ ./bin/hdfs dfs -mkdir -p /user/hadoop`

2. 将./etc/hadoop 中的xml文件作为输入文件复制到分布式文件系统中：

  `$ ./bin/hdfs dfs -mkdir input`

  `$ ./bin/hdfs dfs -put ./etc/hadoop/*.xml input`

  查看HDFS中的文件列表：

  `$ ./bin/hdfs dfs -ls input`

  `$ ./bin/hadoop jar ./share/hadoop/mapreduce/hadoop-mapreduce-examples-*.jar grep input output 'dfs[a-z.]+'`

  查看运行结果：

  `$ ./bin/hdfs dfs -cat output/*`

  将运行结果取回本地：
  ```shell
  $ rm -r ./output # 先删除本地的output文件夹
  $ ./bin/hdfs dfs -get output ./output # 将HDFS上的output文件夹拷贝到本机
  # Hadoop运行程序时输出目录不能存在
  # 执行 ./bin/hdfs dfs -rm -r output 可删除output文件夹
  $ cat ./output/*
  ```

3. 关闭Hadoop

  `$ ./sbin/stop-dfs.sh`

  **下次启动Hadoop时不需要格式化namenode，只需要启动start-dfs.sh即可**

> ## 错误原因分析及警示

* #### SSH
 * /etc/hosts 和 /etc/hostname 中的主机名没有正确配置
 * SSH服务未启动
* #### JAVA_HOME变量
 * jdk安装路径没输对 *不同发行版的jdk安装路径可能不同,可用whereis java 查找安装路径*
 * 没有source使环境变量生效
 * PATH里的路径分隔符是英文冒号不是分号
* #### Hadoop
  * 检查/etc/hostname 中的主机名和下行提到的主机名是否一致
  * 检查是否修改/usr/local/hadoop/etc/hadoop/core-site.xml文件中的 **"yourhostname"** 为自己的主机名
  * 重新格式化Namenode前请先删除/usr/local/hadoop/下的tmp目录及其子目录


> ## 参考

* [厦大数据库实验室博客](http://dblab.xmu.edu.cn/blog/install-hadoop-in-centos/)
* [Hadoop Wiki](https://wiki.apache.org/hadoop)
* [Archlinux Wiki](https://wiki.archlinux.org/)
* [Markdown指南](http://www.jianshu.com/p/1e402922ee32/)

***
<center>&copy;[Liberty1997](https://github.com/liberty1997) ,04/03/2017 </center>
