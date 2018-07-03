#!/bin/python3
import tensorflow as tf
import train_nn_TrainingAugmentation
import train_nn_Training
import train_nn
import Read_Data

IMAGE_SIZE = 256
BATCH_SIZE = 20
'''
with open('classes.txt') as f:
	con = f.read()
class_ = con.splitlines()
label = {}
for i in range(len(class_)):
	a, b = class_[i].split('#')
	label[int(a)] = b
print(label)
'''
#img_valid, img_label = Read_Data.Read_TFRecords('Weed_InputData_Valid_120.tfre*')
img_valid, img_label = Read_Data.Read_TFRecords('Weed_InputData_Valid_Augmentation*')
img_p, label_p = Read_Data.Preprocess(img_valid, img_label, IMAGE_SIZE)
img_valid_batch, img_label_batch = tf.train.batch([img_p, label_p], batch_size = BATCH_SIZE)
keep_prob = tf.placeholder(tf.float32)
#logits = train_nn.Model(img_valid_batch, keep_prob)
#logits = train_nn_Training.Model(img_valid_batch, keep_prob)
logits = train_nn_TrainingAugmentation.Model(img_valid_batch, keep_prob)
correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(img_label_batch, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
saver = tf.train.Saver()
with tf.Session() as sess:
	sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
	coord = tf.train.Coordinator()
	threads = tf.train.start_queue_runners(sess = sess, coord = coord)
	# BEST Model_Saver06_Final_Augmentation
	saver = tf.train.import_meta_graph('./Model_Saver06_Final_Augmentation/model_save.ckpt-13.meta')
	saver.restore(sess, './Model_Saver06_Final_Augmentation/model_save.ckpt-13')
	#saver = tf.train.import_meta_graph('./Model_Saver05_Final_Augmentation/model_save.ckpt-15.meta')
	#saver.restore(sess, './Model_Saver05_Final_Augmentation/model_save.ckpt-15')
	#saver = tf.train.import_meta_graph('./Model_Saver04_TrainingDataAugmentation/model_save.ckpt-18.meta')
	#saver.restore(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt-18')
	#saver = tf.train.import_meta_graph('./Model_Saver01/model_save.ckpt.meta')
	#saver.restore(sess, './Model_Saver01/model_save.ckpt')
	#saver = tf.train.import_meta_graph('./Model_Saver03_DataAugmentation/model_save.ckpt.meta')
	#saver.restore(sess, './Model_Saver03_DataAugmentation/model_save.ckpt')
	#saver = tf.train.import_meta_graph('./Model_Saver02_Training/model_save.ckpt-15.meta')
	#saver.restore(sess, './Model_Saver02_Training/model_save.ckpt-15')
	A = 0.0
	for i in range(6):
		ans = sess.run(accuracy, feed_dict = {keep_prob: 1.0})
		print(ans)
		A += ans
	print('Mean: ', A / 6)
	coord.request_stop()
	coord.join(threads)
