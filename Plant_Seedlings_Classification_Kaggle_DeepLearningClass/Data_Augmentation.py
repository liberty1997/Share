#!/bin/python3
import tensorflow as tf
import matplotlib.pyplot as plt
import os, random

def Left_Right(img):
	return tf.image.random_flip_left_right(img)

def Up_Down(img):
	return tf.image.random_flip_up_down(img)

def Transpose(img):
	return tf.image.transpose_image(img)

def Brightness(img, max_delta):
	return tf.image.random_brightness(img, max_delta)

def Contrast(img, lb, ub):
	return tf.image.random_contrast(img, lb, ub)

if __name__ == '__main__':
	path = '/mnt/windows_E/DeepLearningClass/Data'
	class_ = os.listdir(path)
	tf.set_random_seed(217)
	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())
		for i in range(len(class_)):
			print(class_[i])
			imgs = os.listdir(path + '/' + class_[i])
			random.shuffle(imgs)
			for j in range(0, 15):
				print(j)
				img	= tf.gfile.FastGFile(path + '/' + class_[i] + '/' + imgs[j], 'rb').read()
				img = tf.image.decode_png(img, 3)
				img = Left_Right(img)
				img = tf.image.encode_png(img)
				with tf.gfile.FastGFile('./DataAugmentation/' + class_[i] + '/Aug_LR_' + imgs[j], 'wb') as writer:
					writer.write(img.eval())
			for j in range(15, 30):
				print(j)
				img	= tf.gfile.FastGFile(path + '/' + class_[i] + '/' + imgs[j], 'rb').read()
				img = tf.image.decode_png(img, 3)
				img = Up_Down(img)
				img = tf.image.encode_png(img)
				with tf.gfile.FastGFile('./DataAugmentation/' + class_[i] + '/Aug_UD_' + imgs[j], 'wb') as writer:
					writer.write(img.eval())
			for j in range(30, 45):
				print(j)
				img	= tf.gfile.FastGFile(path + '/' + class_[i] + '/' + imgs[j], 'rb').read()
				img = tf.image.decode_png(img, 3)
				img = Transpose(img)
				img = tf.image.encode_png(img)
				with tf.gfile.FastGFile('./DataAugmentation/' + class_[i] + '/Aug_TR_' + imgs[j], 'wb') as writer:
					writer.write(img.eval())
			for j in range(45, 60):
				print(j)
				img	= tf.gfile.FastGFile(path + '/' + class_[i] + '/' + imgs[j], 'rb').read()
				img = tf.image.decode_png(img, 3)
				img = Brightness(img, 0.2)
				img = tf.image.encode_png(img)
				with tf.gfile.FastGFile('./DataAugmentation/' + class_[i] + '/Aug_BR_' + imgs[j], 'wb') as writer:
					writer.write(img.eval())
			'''
			for j in range(48, 60):
				print(j)
				img	= tf.gfile.FastGFile(path + '/' + class_[i] + '/' + imgs[j], 'rb').read()
				img = tf.image.decode_png(img, 3)
				img = Contrast(img, 0.1, 0.6)
				img = tf.image.encode_png(img)
				with tf.gfile.FastGFile('./DataAugmentation/' + class_[i] + '/Aug_CO_' + imgs[j], 'wb') as writer:
					writer.write(img.eval())
			'''
