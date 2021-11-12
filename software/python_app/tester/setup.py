try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

config = {
    'description': 'Moj projekt',
    'author': 'Surmiak Łukasz',
    'url': ' github',
    'author_email': 'Surmiak.lukasz86@gmail.com',
    'version': '0.1',
    'install_requires': ['nose', 'PyQt6', 'PySerial', 'keyboard'],
    'packages': ['Packages'],
    'scripts': ['bin/motor_move.py'],
    'name': 'nazwa projektu',
}

setup(**config)
