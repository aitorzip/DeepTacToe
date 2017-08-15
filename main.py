#!/usr/bin/python

from game import Game
from players import Human, AI

__version__ = '1.0'

if __name__ == '__main__':
    Game([Human(),Human(),Human()], grid_size=4, silent=False).run()
