#!/bin/python3
import os


def rm(text):
	a = os.scandir(text)
	while True:
		b = next(a, None)
		if b == None:
			break
		if b.is_file():
			os.remove(b)
		if b.is_dir():
			rm(b.path)
	os.rmdir(text)


rm("cmake-build-debug")
rm("cmake-build-release")
os.mkdir("cmake-build-debug")
os.mkdir("cmake-build-release")
