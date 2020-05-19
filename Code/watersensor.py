import RPi.GPIO as GPIO
import time
import smtplib
import ssl
import csv
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart


#port configs on raspberry pi
SPICLK = 11
SPIMISO = 9
SPIMOSI = 10
SPICS = 8

# water sensor connected to adc #0
watersense = 0

def sendalert(level,receiver_email):
    sender_email = "autowaterbowlsensor.alert@gmail.com"
    
    #Depending on flag received from ADC, generate email to user
    if level == 1:
        message=MIMEMultipart()
        message['Subject']="ALARM: Waterbowl is at CRITICAL LEVEL"
        body="Water bowl level is at a critical level and needs attention."
        message.attach(MIMEText(body,'plain'))
        text=message.as_string()
    elif level == 2:
        message=MIMEMultipart()
        message['Subject']="Water levels are getting low!"
        body="Your furry friend is quite thirsty. It's time to check the waterbowl!"
        message.attach(MIMEText(body,'plain'))
        text=message.as_string()
    elif level == 3:
        message=MIMEMultipart()
        message['Subject']="Waterbowl filled - thanks"
        body="This is a confirmation that the waterbowl is now full!"
        message.attach(MIMEText(body,'plain'))
        text=message.as_string()
    elif level > 3 or level < 0:
        return -1;


    port = 465 #for ssl
    password = "Testing123!"
         
    context = ssl.create_default_context()
         
    with smtplib.SMTP_SSL("smtp.gmail.com",port,context=context) as server:
        server.login(sender_email,password)
        server.sendmail(sender_email,receiver_email,text)
    
    time.sleep(10) #pause to send email before resume testing
    

#port init
def init():
         GPIO.setwarnings(False)
         GPIO.cleanup()         #clean up at the end of script
         GPIO.setmode(GPIO.BCM)     #to specify whilch pin numbering system
         # pin setup
         GPIO.setup(SPIMOSI, GPIO.OUT)
         GPIO.setup(SPIMISO, GPIO.IN)
         GPIO.setup(SPICLK, GPIO.OUT)
         GPIO.setup(SPICS, GPIO.OUT)
         
#read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
        if ((adcnum > 7) or (adcnum < 0)):
                return -1
        GPIO.output(cspin, True)    

        GPIO.output(clockpin, False)  # start clock low
        GPIO.output(cspin, False)     # bring CS low

        commandout = adcnum
        commandout |= 0x18  # start bit + single-ended bit
        commandout <<= 3    
        for i in range(5):
                if (commandout & 0x80):
                        GPIO.output(mosipin, True)
                else:
                        GPIO.output(mosipin, False)
                commandout <<= 1
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)

        adcout = 0
        # read in one empty bit, one null bit and 10 ADC bits
        for i in range(12):
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)
                adcout <<= 1
                if (GPIO.input(misopin)):
                        adcout |= 0x1

        GPIO.output(cspin, True)
        
        adcout >>= 1       # first bit is 'null' so drop it
        return adcout

def main():
         init()
         time.sleep(2)
         message_sender_empty=0
         message_sender_critical=0
         message_sender_midway=0
         message_sender_full=0
         maxlvl=999.0
         i=0
         flag="Start"
         receiver_email = input("Input the target email for notifications and press ENTER:  ")
         #receiver_email = "autowaterbowlsensor.alert@gmail.com"
         print("Sensor power-on in progress...")
         time.sleep(10)
         print("Hang on...")
         time.sleep(10)
         print("Almost there...")
         time.sleep(10)
         print("Starting water level sensor!\n")
         print("Press CTRL+C to end data collection.")
         try:
             with open('SensorOutputData.csv','w') as file:
                 writer = csv.writer(file, delimiter='\t')
                 while True:
                          adc_value=readadc(watersense, SPICLK, SPIMOSI, SPIMISO, SPICS)
                          if adc_value<(maxlvl-150) and maxlvl != 999.0 and message_sender_critical==0:
                                   flag="CRITICAL" #flag in output file
                                   sendalert(1,receiver_email) #generate alert email
                                   message_sender_critical=1 #flag that email was generated
                                   message_sender_full=0
                          elif adc_value>=(maxlvl-150) and adc_value<(maxlvl-80) and maxlvl != 999.0 and message_sender_midway==0 and message_sender_critical==0:
                                   flag="LOW" #flag in output file
                                   sendalert(2,receiver_email) #generate alert email
                                   message_sender_midway = 1 #flag that email was generated
                                   message_sender_full=0
                          elif adc_value>=350 and message_sender_full==0:
                                   flag="FULL" #flag in output file
                                   maxlvl=adc_value
                                   sendalert(3,receiver_email) #generate alert email
                                   message_sender_full=1 #flag that email was generated
                                   
                                   #reset midway and critical flags after refill
                                   message_sender_midway=0 
                                   message_sender_critical=0
                          print(adc_value)
                          i=i+1 #note row number in CSV
                          writer.writerows(zip([i],[adc_value],[flag]))
                          flag="None"
                          time.sleep(1)
         except KeyboardInterrupt:
             pass
        

if __name__ == '__main__':
         try:
                  main()
                 
         except KeyboardInterrupt:
                  pass
GPIO.cleanup()



