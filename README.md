# Noisy-Channel
From the course: intro to computers networking - Tel-Aviv university.
A project which implement an error correcetion Hamming code via sender to noisy channel and reciever. 
Sender: reads a file, divide the file into packets, sends the packets after coding as (31,27,3) hamming code.
Channel: recieve the message from sender, flips massage bits (able to flip random bits or every n'th bit).
Reciever: gets the corrupted  massage from the channel, fix errors and decode the hamming coded packets into original message.
