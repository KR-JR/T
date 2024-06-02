import torch
import numpy as np
import cv2

from functions import *
from ultralytics import YOLO

import nanocamera as nano
import threading

device = 'cuda' if torch.cuda.is_available() else 'cpu'

pose_model = YOLO('../model/yolov8n-pose.pt')
knife_model = YOLO('../model/yolov9c.pt')

window_name = 'origin'
window_name_2 = 'annotated'

frames = {'origin': None, 'annotated': None}
count = 0

fps = 30
detecting_count= 30
knife_conf = 0.5
pose_conf = 0.5
knife_min_score = 40
danger_threshold = 50
gap_time = 10

cap = nano.Camera(camera_type=1, device_id = 1, flip=2, fps=fps)
print('camera ready? :', cap.isReady())

while cap.isReady():
    frames['origin'] = cap.read()

    if frames['origin'] is not None:
        cv2.imshow(window_name, frames['origin'])
        if (count >= detecting_count):
            detect_danger(pose_model, knife_model, frames, knife_conf, pose_conf, knife_min_score, danger_threshold, gap_time)
            count = 0

    if frames['annotated'] is not None:
        cv2.imshow(window_name_2, frames['annotated'])

    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

    count += 1
    
cv2.destroyAllWindows()
cap.release()
