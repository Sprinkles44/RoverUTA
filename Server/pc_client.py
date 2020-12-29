# host_ip = '192.168.1.108' # IP of Rover using WiFi


import socket, cv2, pickle, struct
import numpy as np

## Create Socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host_ip = '192.168.1.108' # IP of Rover
port = 9999
client_socket.connect((host_ip, port)) # tuple
data = b""
payload_size = struct.calcsize("Q")

## Get Video Frame from Rover
def get_frame():
  global client_socket, host_ip, port, data, payload_size
  while len(data) < payload_size:
    packet = client_socket.recv(4*1024) # 4 kB
    if not packet: break
    data += packet

  packed_msg_size = data[:payload_size]
  data = data[payload_size:]
  msg_size = struct.unpack("Q", packed_msg_size)[0]

  while len(data) < msg_size:
    data += client_socket.recv(4*1024)

  frame_data = data[:msg_size]
  data = data[msg_size:]
  return(pickle.loads(frame_data)) # Image Received from Rover


## Declaring Global Variables
color_thresh = 15
b = r = g = xpos = ypos = 0
clicked = False



## function for cv2 to get x, y coordinates of mouse double click
def get_color_func(event, x, y, flags, param):
  if event == cv2.EVENT_LBUTTONDBLCLK:
    global b, g, r, xpos, ypos, clicked
    clicked = True
    xpos = x
    ypos = y
    b, g, r = frame[y, x]
    b = int(b)
    g = int(g)
    r = int(r)


frame = get_frame()
color_frame = frame

while True:
  try:
    frame = get_frame()
    # Finding dimensions for Window Size
    f_height = frame.shape[0]
    f_width = frame.shape[1]  

    cv2.namedWindow('Recieved', cv2.WINDOW_NORMAL)
    cv2.imshow('Received', frame)

    ## Get color
    cv2.setMouseCallback('Received', get_color_func)

    if clicked:
      low_color = np.array([b-color_thresh, g-color_thresh, r-color_thresh])
      high_color = np.array([b+color_thresh, g+color_thresh, r+color_thresh])
      color_mask = cv2.inRange(frame, low_color, high_color)
      # Show frame(s)
      color_frame = cv2.bitwise_and(frame, frame, mask=color_mask)
      cv2.imshow('Red Mask', color_frame)
  except:
    pass

  key = cv2.waitKey(1) & 0xFF
  if key == ord('q'):
    break

cv2.destroyAllWindows()
client_socket.close()
close()
