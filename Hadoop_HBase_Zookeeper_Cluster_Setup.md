# Hadoop HBase Zookeeper的集群搭建
***
> #### 总体流程
* 先配置一个节点（Master），再配置其他节点（Slaves）

</br>

> #### 环境说明
* Arch Linux 4.9.11 --- 三台虚拟机
* Java version 1.7.0_131
* Hadoop 2.7.3
* HBase 1.2.6
* Zookeeper 3.4.10

***

## Getting Started

#### 1. 配置hosts文件
打开/etc/hosts文件，将各节点的IP及主机名写入，形成映射。
格式如下：
```/etc/hosts
192.168.56.101 hadoopmaster
192.168.56.102 hadoopslave1
192.168.56.102 hadoopslave2
```

然后将hosts文件复制给其他节点，如：
```shell
scp /etc/hosts root@hadoopslave1:/etc/hosts
scp /etc/hosts root@hadoopslave2:/etc/hosts
```

配置好后可以用命令 `ping -c 4 hadoopslave1` 来检测是否成功。（在任意节点上ping任意节点的主机名都成功，表明配置成功。）
#### 2. 配置SSH免密登录
*如果SSH服务没有启动，请先启动SSH服务，并设置为开机自启。（在Arch下为 `systemctl start sshd;systemctl enable sshd`）*

先打开/etc/ssh/sshd_config，将下面行的注释去掉：
```/etc/ssh/sshd_config
RSAAuthentication yes
PubkeyAuthentication yes
AuthorizedKeysFile        .ssh/authorized_keys
```
如果是用root用户来配置hadoop，即是root用户来使用ssh远程登录，还需要将`PermitRootLogin`这行的注释去掉并将其值改为`yes`。

之后用scp命令将此文件传给其他各节点。传到slave1节点如： `scp /etc/ssh/sshd_config root@hadoopslave1:/etc/ssh/sshd_config`

然后在每台节点上执行 `ssh-keygen -t rsa` 来生成公钥。

将各节点上生成的公钥合并到主节点下的authorized_keys文件，并将此文件复制给所有从节点，执行：
```shell
cd ~/.ssh  # 如果没有.ssh目录，就先执行一下 `ssh 当前主机的主机名`
cat id_rsa.pub >> authorized_keys
ssh root@hadoopslave1 cat ~/.ssh/id_rsa.pub >> authorized_keys # 将slave1节点上的公钥加入主节点的authorized_keys文件
ssh root@hadoopslave2 cat ~/.ssh/id_rsa.pub >> authorized_keys
scp authorized_keys root@hadoopslave1:~/.ssh/authorized_keys # 将包含所有节点的公钥的authorized_keys文件复制给slave1节点
scp authorized_keys root@hadoopslave2:~/.ssh/authorized_keys
```

此时，SSH应该已经配置成功，可以执行 `ssh hadoopslave1` 来检查是否是免密登录。（按上述步骤配置，最后效果为 主节点与从节点间/从节点与从节点间 均可进行免密登录）

#### 3. 配置需要的变量
在环境变量文件（如/etc/profile, /etc/bash.bashrc, /home/配置hadoop的用户/.bashrc, 有的文件在其他Linux发行版本中可能没有，请根据实际情况选择）中，添加如下配置：（各变量的值请按自己的路径写）
```/etc/bash.bashrc
JAVA_HOME=/usr/lib/jvm/java-7-openjdk
HADOOP_HOME=/home/hadoop/hadoop-2.7.3
HBASE_HOME=/home/hbase-1.2.6
export HADOOP_CLASSPATH=$JAVA_HOME/lib/tools.jar
PATH=$HBASE_HOME/bin:$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$HADOOP_HOME/bin:$HADOOP_HOME/sbin:$PATH:$HOME/bin
export PATH
```

然后将文件传给其余节点。集群中所有节点的上述配置应保持一致。
可以在任意目录下执行 `java -version` 和 `$JAVA_HOME/bin/java -version` 看输出信息是否一致，执行 `hadoop` , `hbase` 看能否得到对应输出（若输出`command not found`说明路径有错）。

#### 4. 配置Hadoop
*在本例子中，hadoop的安装路径为/home/hadoop/hadoop-2.7.3/*

接下来，需要配置四个文件，先进入hadoop的配置文件目录，即执行 `cd $HADOOP_HOME/etc/hadoop/` ，此目录下有很多xml文件。

添加如下配置到core-site.xml文件：
```$HADOOP_HOME/etc/hadoop/core-site.xml
<configuration>
	<property>
		<name>fs.defaultFS</name>
		<value>hdfs://hadoopmaster:9000</value>
	</property>
	<property>
		<name>hadoop.tmp.dir</name>
		<value>/home/hadoop/tmp</value>
	</property>
	<property>
		<name>io.file.buffer.size</name>
		<value>131072</value>
	</property>
</configuration>
```
注意此例中主节点的主机名为hadoopmaster，配置时请填写自己的主节点主机名， hadoop.tmp.dir的路径也请填写自己配置的实际路径。

添加如下配置到hdfs-site.xml文件：
```$HADOOP_HOME/etc/hadoop/hdfs-site.xml
<configuration>
	<property>
		<name>dfs.namenode.name.dir</name>
		<value>/home/hadoop/hdfs/name</value>
	</property>
	<property>
		<name>dfs.datanode.data.dir</name>
		<value>/home/hadoop/hdfs/data</value>
	</property>
	<property>
		<name>dfs.replication</name>
		<value>2</value>
	</property>
	<property>
		<name>dfs.blocksize</name>
		<value>256m</value>
	</property>
</configuration>
```
配置中涉及到路径的地方请换成自己的实际路径。replication为数据的份数，默认为3。blocksize的默认值为64m。

添加如下配置到yarn-site.xml文件：
```$HADOOP_HOME/etc/hadoop/yarn-site.xml
<configuration>
	<property>
		<name>yarn.resourcemanager.hostname</name>
		<value>hadoopmaster</value>
	</property>
	<property>
		<name>yarn.nodemanager.aux-services</name>
		<value>mapreduce_shuffle</value>
	</property>
</configuration>
```
注意将'hadoopmaster'换为实际的主节点主机名。

添加如下配置到mapred-site.xml文件：
（此文件在从节点上需要，主节点不需要。）
```$HADOOP_HOME/etc/hadoop/mapred-site.xml
<configuration>
<property>
	<name>mapreduce.framework.name</name>
	<value>yarn</value>
</property>
<property>
	<name>dfs.permissions</name>
	<value>false</value>
</property>
</configuration>
```
此文件的配置不需要进行更改。

添加如下配置到slaves文件：
```$HADOOP_HOME/etc/hadoop/slaves
hadoopslave1
hadoopslave2
```
此文件需要包含所有从节点的主机名，此例中只有两个从节点。

在hadoop-env.sh, mapred-env.sh, yarn-env.sh文件中配置所需变量，添加下面的变量到前述的三个文件中：
```
export JAVA_HOME=/usr/lib/jvm/java-7-openjdk
HADOOP_LOG_DIR=/home/hadoop/hadoop_logs
export HADOOP_LOG_DIR=${HADOOP_LOG_DIR}/$USER
HADOOP_PID_DIR=/home/hadoop/hadoop_pids
export HADOOP_PID_DIR=${HADOOP_PID_DIR}
```
**JAVA_HOME变量是必须配置的，log目录和pid目录可以不配置，只是官网上说最好将pid和log的目录设置为自定义的路径。**


至此，主节点上的hadoop已经配置完成。接下来只要将/home/hadoop目录scp -r到从节点hadoopslave1和hadoopslave2上就可以了。



官网上有上述配置文件中各属性的含义：
* [core-default.xml](http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/core-default.xml)
* [hdfs-default.xml](http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/hdfs-default.xml)
* [mapred-default.xml](http://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/mapred-default.xml)
* [yarn-default.xml](http://hadoop.apache.org/docs/stable/hadoop-yarn/hadoop-yarn-common/yarn-default.xml)

#### 5. 配置Zookeeper
*将zookeeper安装到主节点后，配置zoo.cfg文件即可完成配置。此例中zookeeper安装在/home/zookeeper-3.4.10/ 。*

进入zookeeper安装目录的conf目录下，新建文件zoo.cfg，添加如下配置：
```/home/zookeeper-3.4.10/conf/zoo.cfg
tickTime=2000
dataDir=/home/zookeeper/datadir
clientPort=2181
initLimit=5
syncLimit=2
server.1=hadoopmaster:2888:3888
server.2=hadoopslave1:2888:3888
server.3=hadoopslave2:2888:3888
```
注意替换dataDir和server的值。

然后将主节点的zookeeper目录scp -r到各个从节点就行了。

#### 6. 配置HBase ---基于HDFS
*此例中HBase安装在/home/hbase-1.2.6/ 。*

添加如下配置到hbase-site.xml：
```/home/hbase-1.2.6/conf/hbase-site.xml
<configuration>
  <property>
    <name>hbase.zookeeper.quorum</name>
    <value>hadoopmaster,hadoopslave1,hadoopslave2</value>
    <description>The directory shared by RegionServers.
    </description>
  </property>
  <property>
    <name>hbase.zookeeper.property.dataDir</name>
    <value>${hbase.tmp.dir}/zookeeper</value>
    <description>Property from ZooKeeper config zoo.cfg.
    The directory where the snapshot is stored.
    </description>
  </property>
  <property>
    <name>hbase.rootdir</name>
    <value>hdfs://hadoopmaster:9000/hbase</value>
    <description>The directory shared by RegionServers.
    </description>
  </property>
  <property>
    <name>hbase.cluster.distributed</name>
    <value>true</value>
    <description>The mode the cluster will be in. Possible values are
      false: standalone and pseudo-distributed setups with managed ZooKeeper
      true: fully-distributed with unmanaged ZooKeeper Quorum (see hbase-env.sh)
    </description>
  </property>
  <property>
    <name>hbase.tmp.dir</name>
    <value>/home/hbase/tmp</value>
  </property>
</configuration>
```
**注意替换相关配置为自己的实际值。**

/home/hbase-1.2.6/conf/hbase-env.sh文件中也需要配置JAVA_HOME。（pid和log变量选配）

/home/hbase-1.2.6/conf/regionservers文件中的内容和hadoop的slaves文件一样。（所有从节点的主机名）

将hdfs-site.xml文件复制到conf目录下，或者创建一个链接也可以。

然后将hbase目录scp -r到各个从节点上。

#### 7. 启动
```shell
start-dfs.sh
start-yarn.sh
start-hbase.sh
```

在浏览器地址栏访问 hadoopmaster:50070 可以进入hadoop页面， 访问 hadoopmaster:16010 可以进入hbase页面。hadoop页面中查看live nodes若与从节点个数相等，则成功。


运行上面的三个脚本后，各节点下的jps结果为：

主节点：

![hadoopmaster](https://github.com/liberty1997/Share/blob/master/hadoopmaster.png)

从节点1：

![hadoopslave1](https://github.com/liberty1997/Share/blob/master/hadoopslave1.png)

从节点2：

![hadoopslave2](https://github.com/liberty1997/Share/blob/master/hadoopslave2.png)


</br>

***
> ### Attentions

* hosts/ssh/jdk等环境变量/hadoop/hbase/zookeeper的路径在各节点上应该保持一致 ---所以最好是先配好一个，然后scp给其余节点
* [CDH](https://www.cloudera.com/products/open-source/apache-hadoop/key-cdh-components.html)是一个在集群上部署hadoop的方便工具 ---我还没用过，暂时不写
* 配置文件里的单词/语句一定要写对
* 多关注输出信息，遇到问题先看log日志（namenode/datanode/yarn等都有对应的.log文件）。如果看了日志还不会解决，就上网搜错误信息

> ### References

* [Hadoop Cluster Setup](http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/ClusterSetup.html)
* [ZooKeeper Getting Started Guide](http://zookeeper.apache.org/doc/trunk/zookeeperStarted.html)
* [Apache HBase ™ Reference Guide](http://hbase.apache.org/book.html)
* [Hadoop安装教程_伪分布式配置_CentOS6.4/Hadoop2.6.0](http://dblab.xmu.edu.cn/blog/install-hadoop-in-centos/)
* [Arch Wiki](https://wiki.archlinux.org/)

***
<p align="center"><a href="https://github.com/liberty1997">&copy;Liberty</a></p>
