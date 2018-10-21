import sys
import os
import grpc

import numpy as np
from PIL import Image, ImageDraw, ImageFont

import proto.vision.service_pb2_grpc as vision_service_grpc
import proto.vision.service_pb2 as vision_service

HOST = '0.0.0.0'
PORT = '9999'

FONT = '/usr/share/fonts/TTF/DejaVuSans.ttf'
FONT_SIZE = 30

COLOUR = (255, 0, 255)

def process_i(channel, name):
    stub = vision_service_grpc.OcrStub(channel)
    image, width, height = convert_to_rgba(name)

    frame = vision_service.Frame(image=image, width=width, height=height)
    response = stub.Recognize(frame)
    draw_ocr_result(name, response)

def convert_to_rgba(name):
    img = Image.open(name)
    img = img.convert('RGBA')
    size = img.size

    return np.array(img).flatten().tobytes(), size[0], size[1]

def draw_ocr_result(name, response):
    im = Image.open(name)

    for line in response.lines:
        box = line.a

        draw = ImageDraw.Draw(im)

        draw.line((box.x, box.y, box.x + box.width, box.y), fill=128)
        draw.line((box.x, box.y, box.x, box.y + box.height), fill=128)
        draw.line((box.x + box.width, box.y, box.x + box.width, box.y + box.height), fill=128)
        draw.line((box.x, box.y + box.height, box.x + box.width, box.y + box.height), fill=128)

        draw.text((box.x, box.y), line.value, COLOUR, ImageFont.truetype(FONT, FONT_SIZE))

        del draw

    im.save(open('{}_output.jpg'.format(os.path.splitext(name)[0]), 'w+'), 'JPEG')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print ('python {}.py <image>'.format(__name__))
        sys.exit(-1)

    channel = grpc.insecure_channel('{}:{}'.format(HOST, PORT))
    process_i(channel, sys.argv[1])
