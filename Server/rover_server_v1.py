# 192.168.1.137

import socket, cv2, pickle, struct

# Socket create
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#host_name = socket.getfqdn()
#host_ip = socket.gethostbyname(host_name)
host_ip = '192.168.1.108'
print('HOST IP: ', host_ip)
port = 9999
socket_address = (host_ip,port)

# Socket Bind
server_socket.bind(socket_address)

# Socket listen
server_socket.listen(5)
print("LISTENING AT: ", socket_address)

# socket accept
while True:
  client_socket, addr = server_socket.accept()
  print("GOT CONNECTION FROM: ", addr)
  if client_socket:
    vid = cv2.VideoCapture(0)
    while(vid.isOpened()):
      ret, frame = vid.read()
      if(ret):
        a = pickle.dumps(frame)
        message = struct.pack("Q", len(a)) + a
        client_socket.sendall(message)

      ## vvv Play video, enable for debugging vvv
      ## vvv (will use processing power, disable when implimenting on rover) vvv
      cv2.imshow('TRANSMITTING VIDEO', frame) 
      
      key = cv2.waitKey(1) & 0xFF
      if key == ord('q'):
        vid.release()
        client_socket.close()
        exit()
