
import random
from time import sleep

class Player:
    points = 0

    def move(self, board, button):
        pass

    def add_points(self, points):
        self.points += points

    def get_points(self):
        return self.points


class Human(Player):
    # Basically returns the last button pressed by the user
    def move(self, game):
        if game.last_button_pressed is None:
            return None

        button_id = game.last_button_pressed.id.split('_')
        return (int(button_id[0]), int(button_id[1]))

class AI(Player):
    def __init__(self, train=False):
        self.train = train

    # Random for now
    def move(self, game):

        available_boxers = [boxer for boxer in game.board.boxers if not boxer.disabled]
        available_boxes = []
        for boxer in available_boxers:
            available_boxes += [box for box in boxer.boxes if box.text == '']
        box = random.choice(available_boxes)

        # Fake thinking time
        if not self.train:
            sleep(random.uniform(0.1,3.0))

        button_id = box.id.split('_')
        return (int(button_id[0]), int(button_id[1]))
