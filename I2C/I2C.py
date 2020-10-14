#main.py
# Blob Detection Example
#
# This example shows off how to use the find_blobs function to find color
# blobs in the image. This example in particular looks for dark green objects.

import sensor, image, time
from pid import PID
import pyb, ustruct
import ujson
from pyb import Pin, Timer
# You may need to tweak the above settings for tracking green things...
# Select an area in the Framebuffer to copy the color settings.

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
clock = time.clock() # Tracks FPS.
# OpenMV上的硬件I2C总线都是2
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit() # 完全关闭设备
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")
# For color tracking to work really well you should ideally be in a very, very,
# very, controlled enviroment where the lighting is constant...
green_threshold   = (12, 100, -47, 87, 40, 101)   # 颜色阈值，不同物体需要修改
size_threshold = 2000               #小球距离
x_pid = PID(p=2.5, i=0.01, d=0.01, imax=30)     # 方向参数p
h_pid = PID(p=0.01, i=0.1, imax=100)    # 速度参数p
def run(left_speed, right_speed):
    data = str(left_speed)+" "+str(right_speed)+" "
    try:
        #print(data)
        bus.send(ustruct.pack("<h", len(data)), timeout=10000) # 首先发送长度 (16-bits).
        try:
            bus.send(data, timeout=10000) # 然后发送数据
            print("Sent Data!") # 没有遇到错误时，会显示
        except OSError as err:
            pass # 不用担心遇到错误，会跳过
    except OSError as err:
        pass # 不用担心遇到错误，会跳过

def find_max(blobs):                #找到视野中最大的色块，即最大的小球
    max_size = 0
    for blob in blobs:
        if blob[2] * blob[3] > max_size:
            max_blob = blob
            max_size = blob[2] * blob[3]
    return max_blob


while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.

    blobs = img.find_blobs([green_threshold])
    if blobs:
        max_blob = find_max(blobs)
        x_error = max_blob.cx()-img.width()/2    #色块的外框的中心x坐标blob[5]
        #h_error = max_blob[2]*max_blob[3]-size_threshold
        #色块的外框的宽度blob[2],色块的外框的高度blob[3]
        print("x error: ", x_error)   #打印 x 轴误差  用于转弯
        #print("h error: ", h_error)   #打印 距离误差  用于速度
        '''
        for b in blobs:
            # Draw a rect around the blob.
            img.draw_rectangle(b[0:4]) # rect
            img.draw_cross(b[5], b[6]) # cx, cy
        '''
        img.draw_rectangle(max_blob.rect()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy
        x_output=x_pid.get_pid(x_error,1)
        #h_output=h_pid.get_pid(h_error,1)    #h_error调整后的值
        print("x_output",x_output)
        #print("h_output",h_output)

        #run(-h_output-x_output,-h_output+x_output)
        run(-x_output,x_output)
        #print(-h_output-x_output,-h_output+x_output)
    else:
        run(0,0)
