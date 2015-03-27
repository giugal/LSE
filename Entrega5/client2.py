# Echo client program
import socket
import sys

HOST = '127.0.0.1'    # The remote host
PORT = 8082          	 # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(sys.argv[1]+'/'+sys.argv[2]+'/'+sys.argv[3]+'/'+sys.argv[4])  #Habitacion
#s.sendall(sys.argv[2]+'/')  #Sensor
#s.sendall(sys.argv[3]+'/')  #Valor
#s.sendall(sys.argv[4])  #Unidad de medida
s.close()
