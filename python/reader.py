import serial
import sys
import time

# Configure the serial connection
ser = serial.Serial("COM10", baudrate=115200, timeout=1)


def foo_one():
    start_time = time.time()
    #with open("output_two.gba", "wb") as file:
  
    with open("output_seven.gba", "wb+") as file:
        #32\
        #for i in range(1000):#this should get to the point of bad stuff getting written in
        #for i in range(16400): #output gba 2
        for i in range(8200): #output gba 2
        #for i in range(3):
        #for i in range(200):
            # Send the character "u"
            ser.write(b'u')  # Use b'u' to send as bytes

            # Read 100 bytes from the serial port
            #data = ser.read(512)
            data = ser.read(1024)

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
            #file.write(modified_data_four)
            file.write(data)
            #print(modified_data_four)

            #this should read out the data thqt was just written and print it out
            #file.seek(-512, 2)  # Move 512 bytes before the EOF (end of file)
            #last_data = file.read(512)  # Read last 512 bytes
            #if(last_data == modified_data_four):
                #print("Last 512 Bytes (Hex):", last_data)
            #    print(i)
            #else:
            #    print("they not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not samehey not same")
            #    sys.exit()
            
            print(i)
            elapsed_time = time.time() - start_time
            print(f"Time since start: {elapsed_time:.2f} seconds")

def foo_two():
    #output_10_one.gba output_10_two.gba
    #with open("output_10_one.gba", "rb") as file_one, open("output_10_two.gba", "rb") as file_two:
    with open("kirby.gba", "rb") as file_one, open("output_four.gba", "rb") as file_two, open("output_shortened.gba", "wb+") as file_three:

        i = 0
        match_count = 0
        not_match_count = 0
        single_byte_not_match = 0
        #while chunk_one := file_one.read(512):
        for i in range(16400):
            chunk_one = file_one.read(512)            
            chunk_two = file_two.read(512)
            if(chunk_one == chunk_two):
                match_count = match_count + 1
                print(i)
                #print(chunk_two)
                file_three.write(chunk_two)

                file_three.seek(-512, 2)  # Move 512 bytes before the EOF (end of file)
                last_data = file_three.read(512)  # Read last 512 bytes
                print(last_data)
            else:

                not_match_count = not_match_count + 1
                for j in range(len(chunk_one)):
                    if (chunk_one[j] != chunk_two[j]):
                        single_byte_not_match = single_byte_not_match + 1
                        print(f"{i}, {j}, chunk one: {chunk_one[j]}, chunk two: {chunk_two[j]}")  
                        sys.exit() 
                    else:
                        file_three.write(bytes(chunk_two[j]))
                        print(chunk_two[j])

            i = i + 1
            #terminate if we not_match too many times
            if (not_match_count > 8):
                sys.exit()
            

                

foo_one()
#foo_two()
ser.close()
