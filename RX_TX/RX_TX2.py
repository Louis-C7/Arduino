import sensor, image, time
import json
from pid import PID
from pyb import Servo
from pyb import UART


red_threshold  = (26, 100, -40, 27, 29, 61)

size_threshold = 2000               #小球距离
x_pid = PID(p=0.1, i=0, d=0, imax=10)     # 方向参数p
h_pid = PID(p=0.01, i=0.1, imax=100)    # 速度参数p

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
#sensor.set_vflip(True)
clock = time.clock() # Tracks FPS.
uart = UART(3, 115200)

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.

    blobs = img.find_blobs([red_threshold],pixels_threshold=200, area_threshold=200, merge=True)
    if blobs:
        max_blob = find_max(blobs)
        x_error = max_blob[5]-img.width()/2    #色块的外框的中心x坐标blob[5]
        h_error = max_blob[2]*max_blob[3]-size_threshold    #色块的外框的宽度blob[2],色块的外框的高度blob[3]
        print("x error: ", x_error)   #打印 x 轴误差  用于转弯

        img.draw_rectangle(max_blob.rect()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy
        x_output=x_pid.get_pid(x_error,1)
        print("x_output: ",x_output)
    else:
        data = {
        "left":0,
        "right":0
        }

        data_out = json.dumps(data)
        uart.write(data_out +'\n')
        print("not found!")


