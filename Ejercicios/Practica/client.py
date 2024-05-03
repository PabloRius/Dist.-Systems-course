from enum import Enum
import argparse
import socket
import threading

class client :

    # ******************** TYPES *********************
    # *
    # * @brief Return codes for the protocol methods
    class RC(Enum) :
        OK = 0
        ERROR = 1
        USER_ERROR = 2

    # ****************** ATTRIBUTES ******************
    _server = None
    _port = -1
    _threads = {}

    # ******************** METHODS *******************
    @staticmethod
    def readResponse(sock):
        a = sock.recv(1)
        return (int(a.decode(),10))
    
    @staticmethod
    def readLine(sock):
        a = ''
        while True:
            msg = sock.recv(1)
            if(msg == b'\0'):
                break
            a+= msg.decode()
        return a

    @staticmethod
    def  register(user) :
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))
        message = 'REGISTER\0'
        sock.sendall(message.encode())
        message = f'{user}\0'
        sock.sendall(message.encode())
        res = client.readResponse(sock)
        sock.close()
        if res == 0:
            print("c> REGISTER OK")
        elif res == 1:
            print("c> USERNAME IN USE")
        else:
            print("c> REGISTER FAIL")
                
        return client.RC.OK

   
    @staticmethod
    def  unregister(user) :
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))
        message = 'UNREGISTER\0'
        sock.sendall(message.encode())
        message = f'{user}\0'
        sock.sendall(message.encode())
        res = client.readResponse(sock)
        sock.close()
        if res == 0:
            print("c> UNREGISTER OK")
        elif res == 1:
            print("c> USER DOES NOT EXIST")
        else:
            print("c> UNREGISTER FAIL")
                
        return client.RC.OK

    @staticmethod
    def sock_listen(s_socket:socket.socket):
        s_socket.listen(5)
        while True:
            try:
                conn, _ = s_socket.accept()
                try:
                    msg = client.readLine(s_socket)
                    print(msg)
                finally:
                    conn.close()
            except KeyboardInterrupt:
                break
    
    @staticmethod
    def  connect(user) :
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.connect((client._server, client._port))
        message = 'CONNECT\0'
        sock.sendall(message.encode())
        message = f'{user}\0'
        sock.sendall(message.encode())
        # Creamos el socket por el que el cliente va a escuchar peticiones de otros clientes
        s_socket = socket.socket()
        s_socket.bind(('',0))
        _, port = s_socket.getsockname()
        message = f'{port}\0'
        sock.sendall(message.encode())

        # TODO: Hay que crear el hilo independiente para no tener que hacer un join
        client._threads[user] = threading.Thread(target=client.sock_listen, args=(s_socket,))
        client._threads[user].start()

        res = client.readResponse(sock)
        sock.close()
        if res == 0:
            print("c> CONNECT OK")
        elif res == 1:
            print("c> CONNECT FAIL, USER DOES NOT EXIST")
        elif res == 2:
            print("c> USER ALREADY CONNECTED")
        else:
            print("c> CONNECT FAIL")
        return client.RC.ERROR


    
    @staticmethod
    def  disconnect(user) :
        # Primero, detener el hilo de escucha si está activo
        if user in client._threads:
            client._threads[user].do_run = False
            client._threads[user].join()

        # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "DISCONNECT"
        sock.sendall('DISCONNECT\0'.encode())

        # Enviar el nombre del usuario que se desconecta
        sock.sendall(f'{user}\0'.encode())

        # Recibir el resultado de la operación
        res = client.readResponse(sock)

        # Interpretar la respuesta del servidor
        if res == 0:
            print("c> DISCONNECT OK")
        elif res == 1:
            print("c> DISCONNECT FAIL / USER DOES NOT EXIST")
        elif res == 2:
            print("c> DISCONNECT FAIL / USER NOT CONNECTED")
        else:
            print("c> DISCONNECT FAIL")
        return client.RC.ERROR

    @staticmethod
    def  publish(fileName,  description) :
        # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "PUBLISH"
        message = 'PUBLISH\0'
        sock.sendall(message.encode())

        # Enviar el nombre del usuario
        sock.sendall(f'{user}\0'.encode())

        # Enviar el nombre del archivo (asegurarse que no exceda 256 bytes y no tenga espacios)
        if len(fileName) > 256 or ' ' in fileName:
            print("c> PUBLISH FAIL, FILE NAME INVALID")
            return client.RC.ERROR
        sock.sendall(f'{fileName}\0'.encode())

        # Enviar la descripción (asegurarse que no exceda 256 bytes)
        if len(description) > 256:
            print("c> PUBLISH FAIL, DESCRIPTION TOO LONG")
            return client.RC.ERROR
        sock.sendall(f'{description}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta
        if res == 0:
            print("c> PUBLISH OK")
        elif res == 1:
            print("c> PUBLISH FAIL, USER DOES NOT EXIST")
        elif res == 2:
            print("c> PUBLISH FAIL, USER NOT CONNECTED")
        elif res == 3:
            print("c> PUBLISH FAIL, CONTENT ALREADY PUBLISHED")
        else:
            print("c> PUBLISH FAIL")

        return client.RC.ERROR

    @staticmethod
    def  delete(fileName) :
         # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "DELETE"
        message = 'DELETE\0'
        sock.sendall(message.encode())

        # Enviar el nombre del usuario
        sock.sendall(f'{user}\0'.encode())

        # Enviar el nombre del archivo (asegurarse que no exceda 256 bytes y no tenga espacios)
        if len(fileName) > 256 or ' ' in fileName:
            print("c> DELETE FAIL, FILE NAME INVALID")
            return client.RC.ERROR
        sock.sendall(f'{fileName}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta
        if res == 0:
            print("c> DELETE OK")
        elif res == 1:
            print("c> DELETE FAIL, USER DOES NOT EXIST")
        elif res == 2:
            print("c> DELETE FAIL, USER NOT CONNECTED")
        elif res == 3:
            print("c> DELETE FAIL, CONTENT NOT PUBLISHED")
        else:
            print("c> DELETE FAIL")
        return client.RC.ERROR

    @staticmethod
    def  listusers() :
         # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "LIST USERS"
        message = 'LIST_USERS\0'
        sock.sendall(message.encode())

        # Enviar el nombre del usuario que realiza la operación
        sock.sendall(f'{user}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta inicial
        if res == 0:
            # Leer el número de usuarios que se enviarán
            num_users = int(client.readLine(sock))
            print("c> LIST_USERS OK")
            for _ in range(num_users):
                username = client.readLine(sock)
                ip = client.readLine(sock)
                port = client.readLine(sock)
                print(f"{username} {ip} {port}")
        elif res == 1:
            print("c> LIST_USERS FAIL, USER DOES NOT EXIST")
        elif res == 2:
            print("c> LIST_USERS FAIL, USER NOT CONNECTED")
        else:
            print("c> LIST_USERS FAIL")
        return client.RC.ERROR

    @staticmethod
    def  listcontent(user) :
        # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "LIST CONTENT"
        message = 'LIST_CONTENT\0'
        sock.sendall(message.encode())

        # Enviar el nombre del usuario que realiza la operación
        sock.sendall(f'{requesting_user}\0'.encode())

        # Enviar el nombre del usuario cuyo contenido se desea conocer
        sock.sendall(f'{user}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta inicial
        if res == 0:
            # Leer el número de ficheros que se enviarán
            num_files = int(client.readLine(sock))
            print("c> LIST_CONTENT OK")
            for _ in range(num_files):
                file_name = client.readLine(sock)
                description = client.readLine(sock)
                print(f'{file_name} "{description}"')
        elif res == 1:
            print("c> LIST_CONTENT FAIL, USER DOES NOT EXIST")
        elif res == 2:
            print("c> LIST_CONTENT FAIL, USER NOT CONNECTED")
        elif res == 3:
            print("c> LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST")
        else:
            print("c> LIST_CONTENT FAIL")
        return client.RC.ERROR

    @staticmethod
    def  getfile(user,  remote_FileName,  local_FileName) :

         # Crear un socket TCP/IP
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Conectar al cliente remoto
        sock.connect((remote_ip, remote_port))

        # Enviar el comando "GET FILE"
        message = 'GET_FILE\0'
        sock.sendall(message.encode())

        # Enviar el nombre del fichero que se desea descargar
        sock.sendall(f'{remote_FileName}\0'.encode())

        # Recibir el resultado de la operación
        result = client.readResponse(sock)

        if result == 0:
            # Recibir el contenido del fichero y escribirlo al fichero local
            with open(local_FileName, 'wb') as f:
                while True:
                    data = sock.recv(1024)
                    if not data:
                        break
                    f.write(data)
            print("c> GET_FILE OK")
        elif result == 1:
            print("c> GET_FILE FAIL / FILE NOT EXIST")
        else:
            print("c> GET_FILE FAIL")
        return client.RC.ERROR

    # *
    # **
    # * @brief Command interpreter for the client. It calls the protocol functions.
    @staticmethod
    def shell():

        while (True) :
            try :
                command = input("c> ")
                line = command.split(" ")
                if (len(line) > 0):

                    line[0] = line[0].upper()

                    if (line[0]=="REGISTER") :
                        if (len(line) == 2) :
                            client.register(line[1])
                        else :
                            print("Syntax error. Usage: REGISTER <userName>")

                    elif(line[0]=="UNREGISTER") :
                        if (len(line) == 2) :
                            client.unregister(line[1])
                        else :
                            print("Syntax error. Usage: UNREGISTER <userName>")

                    elif(line[0]=="CONNECT") :
                        if (len(line) == 2) :
                            client.connect(line[1])
                        else :
                            print("Syntax error. Usage: CONNECT <userName>")
                    
                    elif(line[0]=="PUBLISH") :
                        if (len(line) >= 3) :
                            #  Remove first two words
                            description = ' '.join(line[2:])
                            client.publish(line[1], description)
                        else :
                            print("Syntax error. Usage: PUBLISH <fileName> <description>")

                    elif(line[0]=="DELETE") :
                        if (len(line) == 2) :
                            client.delete(line[1])
                        else :
                            print("Syntax error. Usage: DELETE <fileName>")

                    elif(line[0]=="LIST_USERS") :
                        if (len(line) == 1) :
                            client.listusers()
                        else :
                            print("Syntax error. Use: LIST_USERS")

                    elif(line[0]=="LIST_CONTENT") :
                        if (len(line) == 2) :
                            client.listcontent(line[1])
                        else :
                            print("Syntax error. Usage: LIST_CONTENT <userName>")

                    elif(line[0]=="DISCONNECT") :
                        if (len(line) == 2) :
                            client.disconnect(line[1])
                        else :
                            print("Syntax error. Usage: DISCONNECT <userName>")

                    elif(line[0]=="GET_FILE") :
                        if (len(line) == 4) :
                            client.getfile(line[1], line[2], line[3])
                        else :
                            print("Syntax error. Usage: GET_FILE <userName> <remote_fileName> <local_fileName>")

                    elif(line[0]=="QUIT") :
                        if (len(line) == 1) :
                            break
                        else :
                            print("Syntax error. Use: QUIT")
                    else :
                        print("Error: command " + line[0] + " not valid.")
            except Exception as e:
                print("Exception: " + str(e))

    # *
    # * @brief Prints program usage
    @staticmethod
    def usage() :
        print("Usage: python3 client.py -s <server> -p <port>")


    # *
    # * @brief Parses program execution arguments
    @staticmethod
    def  parseArguments(argv) :
        parser = argparse.ArgumentParser()
        parser.add_argument('-s', type=str, required=True, help='Server IP')
        parser.add_argument('-p', type=int, required=True, help='Server Port')
        args = parser.parse_args()

        if (args.s is None):
            parser.error("Usage: python3 client.py -s <server> -p <port>")
            return False

        if ((args.p < 1024) or (args.p > 65535)):
            parser.error("Error: Port must be in the range 1024 <= port <= 65535")
            return False
        
        client._server = args.s
        client._port = args.p

        return True


    # ******************** MAIN *********************
    @staticmethod
    def main(argv) :
        if (not client.parseArguments(argv)) :
            client.usage()
            return

        #  Write code here
        client.shell()
        for i in client._threads:
            i.termiante()
        print("+++ FINISHED +++")
    

if __name__=="__main__":
    client.main([])