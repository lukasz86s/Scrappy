# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()
from wifi import WiFi
from stepstick import A4988

wifi = WiFi('WIFI_NAME', 'PASSWORD')