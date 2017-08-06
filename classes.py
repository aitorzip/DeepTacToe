from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.config import Config

import random

UNOCCUPIED = 0
P1 = 1
P2 = 2

class Game(App):
    title = 'Deep Tac Toe!'

    board = None

    turn = True
    points = [0,0]

    # On application build handler
    def build(self):
        Config.set('graphics', 'width', str(100*3*3 + 20*2))
        Config.set('graphics', 'height', str(100*3*3 + 20*2))
        Config.set('graphics','resizable', False)

        self.board = Board(100, 3, 3, self)
        return self.board

    # On application start handler
    def on_start(self):
        self.new_match(None)

    # Initializes player turn randomly
    def init_players(self):
        if random.random() < 0.5:
            self.turn = True
            msg = 'Player One starts first'
        else:
            self.turn = False
            msg = 'Player Two starts first'

        self.popup_message('Welcome!', msg)

    # Starts a new match
    def new_match(self, popup):
        if self.board is not None:
            self.board.reset()
            self.init_players()

    # A generic popup message
    def popup_message(self, title, msg, on_dismiss=None):
        popup = Popup(title=title, content=Label(text=msg), size=(435, 100), size_hint=(None, None))
        if on_dismiss is not None:
            popup.bind(on_dismiss=on_dismiss)
        popup.open()

    # Handler when a button is pressed
    def btn_pressed(self, button):
        if button.text != '':
            return

        if self.turn == True:
            button.text = 'X'
        else:
            button.text = 'O'

        winner = self.board.check_state()
        if winner != UNOCCUPIED:
            msg = 'Player ' + str(winner) + 'wins!'
            self.points[winner-1] += 1
            self.popup_message('Game over!', msg, on_dismiss=self.new_match)
            return

        self.board.toggle_boxers(int(button.id))
        self.turn = not self.turn

class Board(GridLayout):
    valid_combos = [
        (0,1,2), (3,4,5), (6,7,8),  #Horizontal
        (0,3,6), (1,4,7), (2,5,8),  #Vertical
        (0,4,8), (2,4,6)            #Diagonal
    ]

    def __init__(self, box_size, box_grid_size, boxer_grid_size, game, **kwargs):
        super(Board, self).__init__(**kwargs)

        self.cols = boxer_grid_size
        self.rows = self.cols
        self.spacing = [20,20]

        self.boxers = []

        for i in range(boxer_grid_size*boxer_grid_size):
            boxer = Boxer(box_size, box_grid_size, game)
            self.boxers.append(boxer)
            self.add_widget(boxer)

    def reset(self):
        for boxer in self.boxers:
            boxer.reset()

    def check_state(self):
        # Update boxer states
        for boxer in self.boxers:
            boxer.check_state()

        for combo in self.valid_combos:
            if all(self.boxers[boxer_id].state == P1 for boxer_id in combo):
                return P1
            elif all(self.boxers[boxer_id].state == P2 for boxer_id in combo):
                return P2

        return UNOCCUPIED

    def toggle_boxers(self, id):
        if self.boxers[id].state == UNOCCUPIED:
            for boxer in self.boxers:
                boxer.disabled = True
            self.boxers[id].disabled = False
        else:
            for boxer in self.boxers:
                if boxer.state == UNOCCUPIED:
                    boxer.disabled = False
                else:
                    boxer.disabled = True

    def is_boxer_occupied(self, id):
        return self.boxers[id].state != UNOCCUPIED

    def set_boxer_active(self, id, active=False):
        for boxer in self.boxers:
            boxer.disabled = active
        self.boxers[id].disabled = not active

class Boxer(GridLayout):
    valid_combos = [
        (0,1,2), (3,4,5), (6,7,8),  #Horizontal
        (0,3,6), (1,4,7), (2,5,8),  #Vertical
        (0,4,8), (2,4,6)            #Diagonal
    ]

    def __init__(self, box_size, box_grid_size, game, **kwargs):
        super(Boxer, self).__init__(**kwargs)

        self.cols = box_grid_size
        self.rows = self.cols

        self.state = UNOCCUPIED
        self.boxes = []

        for i in range(box_grid_size*box_grid_size):
            box = Button(text='', font_size=box_size-30, width=box_size,
                        height=box_size, size_hint=(None,None), id=str(i))
            box.bind(on_release=game.btn_pressed)
            self.boxes.append(box)
            self.add_widget(box)

    def reset(self):
        self.state = UNOCCUPIED
        self.disabled = False
        for box in self.boxes:
            box.text = ''
            box.disabled = False

    def check_state(self):
        self.state = UNOCCUPIED

        for combo in self.valid_combos:
            if all(self.boxes[box_id].text == 'X' for box_id in combo):
                self.state = P1
            elif all(self.boxes[box_id].text == 'O' for box_id in combo):
                self.state = P2

        return self.state
