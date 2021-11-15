from nose.tools import *
import tester

def setup():
    print("KONFIGURACJA!")

def teardown():
    print("ZAMYKANIE!")

def test_basic():
    print("URUCHOMIONO TEST!")
