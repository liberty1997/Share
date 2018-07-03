# CNN植物幼苗图像分类

---

classes.txt  label及其对应的杂草种类

classification_results.csv  结果文件的格式

DataAugmentation  文件夹, 存放数据增强后的图片(720张, 做了随机翻转和亮度调整)

Data_Augmentation.py  进行数据增强

HALF2.txt  临时文件, 做tfrecords时用到

HALF.txt  临时文件, 做tfrecords时用到(记录哪些图片已经被写入tfrecords)

Model_Saver01  参数文件"Model_B.model"对应的模型(全部训练数据)

Model_Saver02_Training  未做数据增强的训练模型(划分后的训练集)

Model_Saver03_DataAugmentation  参数文件"Model_B.model"对应的模型(全部训练集, 数据增强)

Model_Saver04_TrainingDataAugmentation 保存训练模型(划分后的数据集, 数据增强)的文件夹

Model_Saver05_Final_Augmentation  参数文件"Model_二.model"对应的最终模型1(全部数据， 数据增强)

Model_Saver06_Final_Augmentation  最终模型2(不改参数, 将上一行的模型重新训练一遍, 也即最终用来预测的模型)

Model_Saver07_TrainingDataAugmentation  保存训练模型("Model_B.model"中的参数, 划分后的训练集)

Png_To_TFRecord.py  将图片写入tfrecords

Read_Data_Error.py  最初的读tfrecords的文件, 有错

Read_Data.py  改正的读tfrecords数据的文件

README.md  readme

Read_Test_Data.py  读取最终测试集tfrecords数据的文件

results.csv  保存最终预测结果的文件

test_nn_120.py  进行预测的文件(自行划分出的120张测试集)

test_nn.py  进行最终的预测的文件, 结果保存到"results.csv"

Test_To_TFRecord.py  将最终测试集的图片写入tfrecords

tmp.py  随机挑选出120张作验证集

train_nn.py  训练网络的初版文件

train_nn.py_v1  第一次收敛? 将卷积核数目减小了

train_nn_TrainingAugmentation.py  对数据增强后的划分出的训练集进行训练

train_nn_Training.py  对未做数据增强的划分出的训练集进行训练

Valid_120.txt  随机划分出的120张验证集的文件名

Valid_Augmentation_120.txt  随机划分出的120张有数据增强的验证集的文件名

Weed_InputData_Augmentation.tfrecords01  做了数据增强的全部训练集

Weed_InputData_Augmentation.tfrecords02

Weed_InputData_Test.tfrecords  测试图片

Weed_InputData.tfrecords01  未做数据增强的全部训练集

Weed_InputData.tfrecords02

Weed_InputData_Training_Augmentation.tfrecords01  划分出的训练集, 数据增强

Weed_InputData_Training_Augmentation.tfrecords02

Weed_InputData_Training.tfrecords01  划分出的训练集, 未做数据增强

Weed_InputData_Training.tfrecords02

Weed_InputData_Valid_120.tfrecords  120张未做数据增强的验证集

Weed_InputData_Valid_Augmentation_120.tfrecords  120张做了数据增强的验证集

---

<p align="center"><a href="https://github.com/liberty1997">&copy;Liberty</a></p>
