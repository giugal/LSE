#!/usr/bin/env python

import select
import socket
import sys

host = ''
port = 8082
backlog = 5
size = 1024

iphost = '127.0.0.1'

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
webserver = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host,port))
webserver.bind((iphost,8080))
server.listen(5)
webserver.listen(5)

input = [server,webserver]

running = 1
while running:
    inputready,outputready,exceptready = select.select(input,[],[])

    for s in inputready:

        if s == server:
            client, address = server.accept()
            print 'Connected by', address
            input.append(client)
            sensordata = client.recv(size)
            hab,sensor,data,um = sensordata.split('/')

            numhab = 2
            checkhab = 0

            for i in range(1,numhab):
                if hab == i:
                    print 'Room selected: {}'.format(hab)
                    break
                if i==numhab and hab != i:
                    print 'Error: room selected does not exist'
                    checkhab = 1
                    break

            if checkhab == 0:

                f = open('hab'+hab+'.html','r+')

                print "File opened:", f.name

                position = f.seek(0,0)
                fline = 'string'
                search = sensor + ':'
                
                if sensor in f.read():
                    while fline != '</html>':
                        
                        position = f.tell()
                        fline=f.readline()
                        if fline.find(search) == 0:
                            f.seek(position+len(search),0)
                            f.write(' ' + data + ' ')
                            if um == 'C' :
                                f.write('&degC')
                                f.write('<BR>')
                            else:
                                f.write(um)
                                f.write('<BR>')
                            position = f.tell()
                            break
                else :
                    while fline != '</html>':
                        position = f.tell()
                        fline=f.readline()
                        if fline.find('</body>') == 0:
                            
                            position = f.seek(-8,1)
                            f.write('<p></p>\n')
                            f.write(sensor)
                            f.write(': ')
                            f.write(data)
                            f.write(' ')
                            f.write(um)
                            f.write('<BR>\n')
                            f.write('</body>\n')
                            f.write('<A href="home.html"><center>Return to home</center></A>')
                            f.write('</html>')
                            position = f.tell()
                            fline = '</html>'
                            break

                f.close()
            else :
                print 'Please select an existing room'

        if s == webserver:

            c, (client_host, client_port) = s.accept()
            print 'Got connection from', client_host, client_port
            x = 0
            request = c.recv(size)
            line = request.split('\n', 1)[0]
            fline = 'string'
            print request
            if 'GET /home.html' in line:
                f = open("home.html", 'r')
            elif 'GET / ' in line:
                f = open("home.html", 'r')
            elif 'GET /hab1.html' in line:
                f = open("hab1.html", 'r')
            elif 'GET /hab2.html' in line:
                f = open("hab2.html", 'r')
            elif 'GET /1act1on' in line:
                f = open("hab1.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act1:') == 0:
                        f.seek(position+len('Act1: '),0)
                        f.write(' ON')
                        position = f.tell()
                        break
            elif 'GET /1act1off' in line:
                f = open("hab1.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act1:') == 0:
                        f.seek(position+len('Act1: '),0)
                        f.write('OFF')
                        position = f.tell()
                        break
            elif 'GET /1act2on' in line:
                f = open("hab1.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act2:') == 0:
                        f.seek(position+len('Act2: '),0)
                        f.write(' ON')
                        position = f.tell()
                        break
            elif 'GET /1act2off' in line:
                f = open("hab1.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act2:') == 0:
                        f.seek(position+len('Act2: '),0)
                        f.write('OFF')
                        position = f.tell()
                        break
            elif 'GET /2act1on' in line:
                f = open("hab2.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act1:') == 0:
                        f.seek(position+len('Act1: '),0)
                        f.write(' ON')
                        position = f.tell()
                        break
            elif 'GET /2act1off' in line:
                f = open("hab2.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act1:') == 0:
                        f.seek(position+len('Act1: '),0)
                        f.write('OFF')
                        position = f.tell()
                        break
            elif 'GET /2act2on' in line:
                f = open("hab2.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act2:') == 0:
                        f.seek(position+len('Act2: '),0)
                        f.write(' ON')
                        position = f.tell()
                        break
            elif 'GET /2act2off' in line:
                f = open("hab2.html", 'r+')
                while fline != '</html>':
                    position = f.tell()
                    fline=f.readline()
                    if fline.find('Act2:') == 0:
                        f.seek(position+len('Act2: '),0)
                        f.write('OFF')
                        position = f.tell()
                        break
            else :
                print ('Error 404 Not Found')
                c.send('HTTP/1.0 404 Not Found\n')
                c.close
                x = 1

            if x == 0:
                f.seek(0,0)
                page = f.read()

                c.send('HTTP/1.0 200 OK\n')
                c.send('Content-Type: text/html\n')
                c.send('\n')
                c.send(page)
                
                c.close()

                
                f.close()
            


server.close()

