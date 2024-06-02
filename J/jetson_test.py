import torch
import numpy as np
import cv2

from graduation_project2.code.functions import *
from ultralytics import YOLO

import nanocamera as nano
import threading

device = 'cuda' if torch.cuda.is_available() else 'cpu'

pose_model = YOLO('../model/yolov8n-pose.pt')
knife_model = YOLO('../model/best.pt')

# window_name = 'graduate'
# cap = cv2.VideoCapture('/dev/video0')
# while cap.isOpened():
#     ret, frame = cap.read()
#     if not ret:
#         continue
#     knife_result, pose_result = get_bbox_pose(pose_model, knife_model, frame)
#     if pose_result.shape[1] == 0:
#         pose_result = torch.zeros((1, 17, 2)).to(device)

#     hand_position, total_pose_result = get_hand_position(pose_result)
#     danger_score = get_danger_score(knife_result, total_pose_result)
#     for i, pose in enumerate(total_pose_result):
#         color_b_and_g = int(255 - danger_score[i].cpu().item() * 2.55)
#         frame = draw_landmark_custom_color(frame, pose.cpu().numpy().astype(int), (color_b_and_g, color_b_and_g, 255))
#         frame = draw_bounding_box(frame, knife_result.cpu().numpy().astype(int))
    
#     alert_if_dangerous(is_dangerous(danger_score))
    
#     cv2.imshow(window_name, frame)
#     if cv2.waitKey(10) & 0xFF == ord('q'):
#         break

window_name = 'graduate'
cap = cv2.VideoCapture(0)

danger_flag = False
send_time = 0
gap_time = 30

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        continue
    knife_result, pose_result = get_bbox_pose(pose_model, knife_model, frame)
    if pose_result.shape[1] == 0:
        pose_result = torch.zeros((1, 17, 2)).to(device)

    hand_position, total_pose_result = get_hand_position(pose_result)
    danger_score = get_danger_score(knife_result, total_pose_result)
    for i, pose in enumerate(total_pose_result):
        try:
            color_b_and_g = int(255 - danger_score[i].cpu().item() * 2.55)
        except Exception as e:
            # print(danger_score[i].cpu().item())
            # print(e)
            pass
        annotated_frame = draw_landmark_custom_color(frame, pose.cpu().numpy().astype(int), (color_b_and_g, color_b_and_g, 255))
        annotated_frame = draw_bounding_box(annotated_frame, knife_result.cpu().numpy().astype(int))
    
    danger_flag = is_dangerous(danger_score)
    current_time = time.time() 
    if (danger_flag) and (current_time - send_time > gap_time):
        thread = threading.Thread(target=works, args=(frame, annotated_frame))
        thread.start()
        send_time = current_time
    
    cv2.imshow(window_name, annotated_frame)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break
    
cv2.destroyAllWindows()
cap.release()
    
cv2.destroyAllWindows()
cap.release()