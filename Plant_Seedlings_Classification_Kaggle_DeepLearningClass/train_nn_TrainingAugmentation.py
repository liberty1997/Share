#!/bin/python3
import time
import tensorflow as tf
from Read_Data import *

def Conv_layer(in_, name, kh, kw, n_out, dh, dw, padding):
	n_in = in_.get_shape()[-1].value
	w = tf.get_variable('w_' + name + str(n_out), shape = [kh, kw, n_in, n_out], dtype = tf.float32, initializer = tf.contrib.layers.xavier_initializer_conv2d())
	b = tf.get_variable('b_' + name + str(n_out), shape = [n_out], dtype = tf.float32, initializer = tf.constant_initializer(0.0))
	conv = tf.nn.conv2d(input = in_, filter = w, strides = [1, dh, dw, 1], padding = padding)
	out_ = tf.nn.relu(tf.nn.bias_add(conv, b))
	return out_

def Maxpooling_layer(in_, kh, kw, dh, dw, padding):
	return tf.nn.max_pool(in_, ksize = [1, kh, kw, 1], strides = [1, dh, dw, 1], padding = padding)

def Fullc_layer(in_, name, n_out):
	n_in = in_.get_shape()[-1].value
	w = tf.get_variable('w_' + name + str(n_out), shape=[n_in, n_out], dtype = tf.float32, initializer = tf.contrib.layers.xavier_initializer())
	b = tf.get_variable('b_' + name + str(n_out), shape = [n_out], dtype = tf.float32, initializer = tf.constant_initializer(0.1))
	return tf.nn.relu_layer(in_, w, b)

def Model(x_, keep_prob):
	
	conv1_1 = Conv_layer(x_, 'conv1_1', 5, 5, 32, 3, 3, 'SAME')
	pool1 = Maxpooling_layer(conv1_1, 3, 3, 2, 2, 'SAME')

	conv2_1 = Conv_layer(pool1, 'conv2_1', 5, 5, 64, 3, 3, 'SAME')
	pool2 = Maxpooling_layer(conv2_1, 3, 3, 2, 2, 'SAME')

	pool_shape = pool2.get_shape().as_list()
	nodes = pool_shape[1] * pool_shape[2] * pool_shape[3]
	reshaped = tf.reshape(pool2, [-1, nodes])

	fc1 = Fullc_layer(reshaped, 'fc1', 256)
	fc2 = Fullc_layer(fc1, 'fc2', 256)
	fc2_drop = tf.nn.dropout(fc2, keep_prob)
	fc3 = Fullc_layer(fc2_drop, 'fc3', 12)
	
	logits = fc3
	return logits

if __name__ == '__main__':
	IMAGE_SIZE = 256
	IMAGE_CHANNELS = 3
	BATCH_SIZE = 32
	MIN_AFTER_DEQUEUE = 500
	CAPACITY = MIN_AFTER_DEQUEUE + 3 * BATCH_SIZE
	NUM_THREADS = 4

	keep_prob = tf.placeholder(tf.float32, name = "keep_prob")

	#image, label = Read_TFRecords('Weed_InputData_Training.tfrecords*')
	image, label = Read_TFRecords('Weed_InputData_Training_Augmentation*')
	#image, label = Read_TFRecords('Weed_InputData_Augmentation*')
	image_p, label_p = Preprocess(image, label, IMAGE_SIZE)
	image_batch, label_batch = tf.train.shuffle_batch([image_p, label_p], batch_size = BATCH_SIZE, capacity = CAPACITY, min_after_dequeue = MIN_AFTER_DEQUEUE, num_threads = NUM_THREADS)

	logits = Model(image_batch, keep_prob)
	cross_entropy = tf.nn.softmax_cross_entropy_with_logits_v2(labels = label_batch, logits = logits)
	loss = tf.reduce_mean(cross_entropy)
	train_step = tf.train.AdamOptimizer(1e-3).minimize(loss)

	correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(label_batch, 1))
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
	saver = tf.train.Saver()
	with tf.Session() as sess:
		step = 0
		c_acc = 0
		start = time.time()
		sess.run([tf.global_variables_initializer(), tf.local_variables_initializer()])
		coord = tf.train.Coordinator()
		threads = tf.train.start_queue_runners(sess = sess, coord = coord)
		while True:
			sess.run(train_step, feed_dict = {keep_prob: 0.5})
			if step % 10 == 0:
				valid_loss, valid_accuracy = sess.run([loss, accuracy], feed_dict = {keep_prob: 1.0})
				print('Step {}, loss = {:.6f}, training accuracy = {:.6f}, total time = {:.3f}'.format(step, valid_loss, valid_accuracy, time.time() - start))
				if valid_accuracy == 1.0:
					c_acc += 1
					if c_acc == 10:
						saver.save(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt', global_step = c_acc)
					elif c_acc == 12:
						saver.save(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt', global_step = c_acc)
					elif c_acc == 13:
						saver.save(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt', global_step = c_acc)
					elif c_acc == 15:
						saver.save(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt', global_step = c_acc)
					elif c_acc == 18:
						saver.save(sess, './Model_Saver04_TrainingDataAugmentation/model_save.ckpt', global_step = c_acc)
					elif c_acc > 18:
						break
			step += 1
		coord.request_stop()
		coord.join(threads)
