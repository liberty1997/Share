#!/bin/python3
import tensorflow as tf
import train_nn
import train_nn_TrainingAugmentation
import Read_Data

IMAGE_SIZE = 256
BATCH_SIZE = 6
with open('classes.txt') as f:
	con = f.read()
class_ = con.splitlines()
label = {}
for i in range(len(class_)):
	a, b = class_[i].split('#')
	label[int(a)] = b
print(label)
img_test, img_name = Read_Data.Read_Test_TFRecords('Weed_InputData_Final_Test*', IMAGE_SIZE)
img_test_batch, img_name_batch = tf.train.batch([img_test, img_name], batch_size = BATCH_SIZE)
keep_prob = tf.placeholder(tf.float32)
#logits = train_nn_TrainingAugmentation.Model(img_test_batch, keep_prob)
logits = train_nn.Model(img_test_batch, keep_prob)
pred = tf.argmax(tf.nn.softmax(logits), 1)
saver = tf.train.Saver()
with tf.Session() as sess:
	sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
	coord = tf.train.Coordinator()
	threads = tf.train.start_queue_runners(sess = sess, coord = coord)
	# results04
	saver = tf.train.import_meta_graph('./Model_Saver03_DataAugmentation/model_save.ckpt.meta')
	saver.restore(sess, './Model_Saver03_DataAugmentation/model_save.ckpt')
	# results03
	#saver = tf.train.import_meta_graph('./Model_Saver01/model_save.ckpt.meta')
	#saver.restore(sess, './Model_Saver01/model_save.ckpt')
	# results02
	#saver = tf.train.import_meta_graph('./Model_Saver05_Final_Augmentation/model_save.ckpt-15.meta')
	#saver.restore(sess, './Model_Saver05_Final_Augmentation/model_save.ckpt-15')
	# results01
	#saver = tf.train.import_meta_graph('./Model_Saver06_Final_Augmentation/model_save.ckpt-13.meta')
	#saver.restore(sess, './Model_Saver06_Final_Augmentation/model_save.ckpt-13')
	with open('TestData_results04.csv', 'w') as fw:
		fw.write(',species\n')
		for i in range(79):
			print('----------------------------------------')
			#a = sess.run(img_name_batch)
			ans = sess.run(pred, feed_dict = {keep_prob: 1.0})
			#print(img_name_batch.eval())
			for _ in range(len(ans)):
				fw.write(',' + label[ans[_]] + '\n')
			#print(a)
			# 以下两个eval()每运行一下 就会取出来一个batch
			#print(img_name_batch.eval())
			#print(img_name_batch.eval())
	coord.request_stop()
	coord.join(threads)
