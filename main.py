#!/usr/bin/python

from game import Game
from players import Human, AI

__version__ = '1.0'

if __name__ == '__main__':

    Game([AI(train=True),AI(train=True)], silent=True).run()
