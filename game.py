from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.config import Config
from kivy.clock import Clock
from kivy.core.audio import SoundLoader

import random

# Constants
SYMBOLS = ('X','O')
UNNOCUPIED = -1

class Game(App):
    title = 'Deep Tac Toe!'
    sounds = (SoundLoader.load('assets/player1_move.ogg'),
            SoundLoader.load('assets/player2_move.ogg'))

    def __init__(self, players, update_rate=300, silent=False):
        super(Game, self).__init__()

        if len(players) > len(SYMBOLS):
            raise ValueError('Number of players should be equal or less than ' + str(len(SYMBOLS)))
            return

        self.players = players
        self.silent = silent
        self.last_button_pressed = None

        Clock.schedule_interval(self.update, 1.0/update_rate)

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
        self.turn = random.randint(0, len(self.players) - 1)
        if not self.silent:
            self.popup_message('Welcome!', 'Player ' + str(self.turn + 1) + ' starts first')

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

    def update(self, *args):
        button_id = self.players[self.turn].move(self)

        if button_id is None:
            return

        # Gets the button from the id
        button = self.board.boxers[button_id[0]].boxes[button_id[1]]
        self.last_button_pressed = None

        # Already occupied
        if button.text != '':
            return

        # Play sound
        if not self.silent:
            self.sounds[self.turn].play()

        # Change button state
        button.text = SYMBOLS[self.turn]

        # Check if someone has already won
        winner = self.board.check_state()
        if winner != UNNOCUPIED:
            self.players[winner].add_points(1)
            if not self.silent:
                self.popup_message('Game over!', 'Player ' + str(winner + 1) + ' wins!', on_dismiss=self.new_match)
            else:
                self.new_match(None)
            return

        # Change available places
        self.board.toggle_boxers(button_id[1])

        # Check if it is a draw (no space left to move)
        available_boxers = [boxer for boxer in self.board.boxers if not boxer.disabled]
        available_boxes = []
        for boxer in available_boxers:
            available_boxes += [box for box in boxer.boxes if box.text == '']
        if len(available_boxes) == 0:
            if not self.silent:
                self.popup_message('Game over!', 'Draw!', on_dismiss=self.new_match)
            else:
                self.new_match(None)
            return

        # Change to next turn
        self.turn = (self.turn + 1) % len(self.players)

    def btn_pressed(self, button):
        self.last_button_pressed = button

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
            boxer = Boxer(box_size, box_grid_size, game, id=str(i))
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
            for i in range(len(SYMBOLS)):
                if all(self.boxers[boxer_id].state == i for boxer_id in combo):
                    return i

        return UNNOCUPIED

    def toggle_boxers(self, id):
        if self.boxers[id].state == UNNOCUPIED:
            for boxer in self.boxers:
                boxer.disabled = True
            self.boxers[id].disabled = False
        else:
            for boxer in self.boxers:
                if boxer.state == UNNOCUPIED:
                    boxer.disabled = False
                else:
                    boxer.disabled = True

    def is_boxer_occupied(self, id):
        return self.boxers[id].state != UNNOCUPIED

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

        self.state = UNNOCUPIED
        self.boxes = []

        for i in range(box_grid_size*box_grid_size):
            box = Button(text='', font_size=box_size-30, width=box_size,
                        height=box_size, size_hint=(None,None), id=self.id + '_' + str(i))
            box.bind(on_release=game.btn_pressed)
            self.boxes.append(box)
            self.add_widget(box)

    def reset(self):
        self.state = UNNOCUPIED
        self.disabled = False
        for box in self.boxes:
            box.text = ''
            box.disabled = False

    def check_state(self):
        self.state = UNNOCUPIED

        for combo in self.valid_combos:
            for i,symbol in enumerate(SYMBOLS):
                if all(self.boxes[box_id].text == symbol for box_id in combo):
                    self.state = i

        return self.state
