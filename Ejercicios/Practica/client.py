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

    _user_connected:str = ""
    _user_thread:threading.Thread
    _user_socket:socket.socket

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
    def sock_listen():
        client._user_socket.listen(5)
        while True:
            try:
                conn, _ = client._user_socket.accept()
                try:
                    msg = client.readLine(client._user_socket)
                    print(msg)
                finally:
                    conn.close()
                    client._user_socket.close()
            except:
                client._user_socket.close()
                break
    
    @staticmethod
    def  connect(user) :
        if(client._user_connected):
            # Cada terminal gestiona un único cliente, por tanto si ya hay un cliente conectado a esta terminal,
            # no podemos permitir que uno nuevo se conecte
            # Lo tratamos como un error genérico ya que puede que el usuario esté o no conectado desde otra terminal, o puede que no exista
            print("c> CONNECT FAIL")
            return client.RC.ERROR

        # Creamos el socket por el que el cliente va a escuchar peticiones de otros clientes
        client._user_socket = socket.socket()
        # Dejamos que la librería escoja un puerto libre y luego con getsockname() extraemos el número de este
        client._user_socket.bind(('',0))
        _, port = client._user_socket.getsockname()
        
        # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.connect((client._server, client._port))

        # Enviar comando "CONNECT"
        message = 'CONNECT\0'
        sock.sendall(message.encode())
        
        # Enviar el nombre del usuario
        message = f'{user}\0'
        sock.sendall(message.encode())
        
        # Enviar el puerto de escucha
        message = f'{port}\0'
        sock.sendall(message.encode())

        client._user_connected = user
        client._user_thread = threading.Thread(target=client.sock_listen)
        client._user_thread.start()

        res = client.readResponse(sock)
        sock.close()
        if res == 0:
            print("c> CONNECT OK")
            return client.RC.OK
        elif res == 1:
            # Si el servidor no ha procesado la operación correctamente, se revierte la creación del socket e hilo
            print("c> CONNECT FAIL, USER DOES NOT EXIST")
            # No llamamos explícitamente a disconnect ya que no queremos que se desconecte al usuario del sistema si su sesión está iniciada desde otro terminal
            client._user_connected = ""
            client._user_socket.close()
            return client.RC.USER_ERROR
        elif res == 2:
            # Si el servidor no ha procesado la operación correctamente, se revierte la creación del socket e hilo
            print("c> USER ALREADY CONNECTED")
            # No llamamos explícitamente a disconnect ya que no queremos que se desconecte al usuario del sistema si su sesión está iniciada desde otro terminal
            client._user_connected = ""
            client._user_socket.close()
            return client.RC.USER_ERROR
        else:
            # Si el servidor no ha procesado la operación correctamente, se revierte la creación del socket e hilo
            print("c> CONNECT FAIL")
            # No llamamos explícitamente a disconnect ya que no queremos que se desconecte al usuario del sistema si su sesión está iniciada desde otro terminal
            client._user_connected = ""
            client._user_socket.close()
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
        sock.sendall(f'{client._user_connected}\0'.encode())

        # Enviar el nombre del archivo (asegurarse que no exceda 256 bytes)
        if len(fileName) > 256 or len(description) > 256:
            return client.RC.ERROR
        
        sock.sendall(f'{fileName}\0'.encode())

        # Enviar la descripción (asegurarse que no exceda 256 bytes)
        if len(description) > 256:
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
    def  disconnect(user) :
        # Dado que el programa guarda internamente los datos del cliente conectado, comprobaremos si el cliente que se quiere desconectar 
        # es el mismo que está conectado
        if(user != client._user_connected):
            # Lo tratamos como un error genérico, ya que puede que el usuario exista y esté conectado, sólo que desde otra terminal
            print("c> DISCONNECT FAIL")
            return client.RC.ERROR
        
        # El hilo de escucha se detendrá independientemente de la respuesta del servidor
        if (client._user_socket):
            # Si el cliente se detuvo 
            client._user_connected = ""
            client._user_socket.close()
        # No es necesario hacer un join al hilo, ya que al cerrar el socket por el que escucha, este capturará una excepción y morirá 
        # Por defecto, python crea los hilos independientes, por tanto, el mismo programa se encarga de limpiarlos al morir

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
    def  delete(fileName) :
         # Crear socket y conectar al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((client._server, client._port))

        # Enviar comando "DELETE"
        message = 'DELETE\0'
        sock.sendall(message.encode())

        # Enviar el nombre del usuario
        sock.sendall(f'{client._user_connected}\0'.encode())

        # Enviar el nombre del archivo (asegurarse que no exceda 256 bytes y no tenga espacios)
        if len(fileName) > 256:
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
        sock.sendall(f'{client._user_connected}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta inicial
        if res == 0:
            # Leer el número de usuarios que se enviarán
            num_users = int(client.readLine(sock))
            print(num_users)
            print("c> LIST_USERS OK")
            end_str = ""
            for _ in range(num_users):
                username = client.readLine(sock)
                ip = client.readLine(sock)
                port = client.readLine(sock)
                end_str += f"\t{username}\t{ip}\t{port}\n"
            print(end_str)
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
        sock.sendall(f'{client._user_connected}\0'.encode())

        # Enviar el nombre del usuario cuyo contenido se desea conocer
        sock.sendall(f'{user}\0'.encode())

        # Leer la respuesta del servidor
        res = client.readResponse(sock)

        # Manejar la respuesta inicial
        if res == 0:
            # Leer el número de ficheros que se enviarán
            print("c> LIST_CONTENT OK")
            num_files = int(client.readLine(sock))
            end_str = ""
            for _ in range(num_files):
                file_name = client.readLine(sock)
                description = client.readLine(sock)
                end_str += f'\t{file_name}\t{description}\n'
            print(end_str)
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
                            if (client._user_connected):
                                client.disconnect(client._user_connected)
                            break
                        else :
                            print("Syntax error. Use: QUIT")
                    else :
                        print("Error: command " + line[0] + " not valid.")
            except KeyboardInterrupt:
                print("Keyboard Interrupting the program")
                if(client._user_connected):
                    client.disconnect(client._user_connected)
                break
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
        if(client._user_socket):
            client._user_socket.close()
        print("+++ FINISHED +++")
    

if __name__=="__main__":
    client.main([])