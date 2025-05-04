import serial
import sys
import time

# Configure the serial connection
ser = serial.Serial("COM10", baudrate=115200, timeout=1)


def foo_one():
    start_time = time.time()
    #with open("output_two.gba", "wb") as file:
    with open("output_three.gba", "wb+") as file:
        #32\
        #for i in range(1000):#this should get to the point of bad stuff getting written in
        for i in range(16100): #output gba 2
        #for i in range(3):
        #for i in range(200):
            # Send the character "u"
            ser.write(b'u')  # Use b'u' to send as bytes

            # Read 100 bytes from the serial port
            data = ser.read(512)

            # Print the received data
            modified_data = data.replace(b'\x00', b'\x20')
            modified_data_two = modified_data.replace(b'\x07', b'\x20')
            modified_data_three = modified_data_two.replace(b'\x0d', b'\x0a')
            #modified_data_three = modified_data_two.replace(b'\x0a', b'\x0a') #output gna 2 aI thingk
            modified_data_four = modified_data_three.replace(b'\x0b', b'\x0a')
            #print("Received:", modified_data)  # Decode as text (ignoring errors if non-text data)
            #print("Received:", data.decode(errors="ignore")) 
            #print(data)

            #file.write(data.decode(errors="ignore"))
            file.write(modified_data_four)

            #this should read out the data thqt was just written and print it out
            file.seek(-512, 2)  # Move 512 bytes before the EOF (end of file)
            last_data = file.read(512)  # Read last 512 bytes
            #print("Last 512 Bytes (Hex):", last_data)

            if(last_data == modified_data_four):
                #print("they same")
                #print("Last 512 Bytes (Hex):", last_data)
                print(i)
            else:
                print("they not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not same")
                sys.exit()
            
            elapsed_time = time.time() - start_time
            print(f"Time since start: {elapsed_time:.2f} seconds")


foo_one()
ser.close()
