from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.config import Config
from kivy.clock import Clock

import random

UNOCCUPIED = 0
P1 = 1
P2 = 2

class Game(App):
    title = 'Deep Tac Toe!'
    last_button_pressed = None

    def __init__(self, player_list, update_rate=300):
        super(Game, self).__init__()
        self.player_list = player_list
        self.last_button_pressed = None
        self.turn = 0
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
        if random.random() < 0.5:
            self.turn = 0
            msg = 'Player One starts first'
        else:
            self.turn = 1
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

    def update(self, *args):
        button_id = self.player_list[self.turn].move(self)

        if button_id is None:
            return

        button = self.board.boxers[button_id[0]].boxes[button_id[1]]
        self.last_button_pressed = None

        # Already occupied
        if button.text != '':
            return

        # Change button state
        if self.turn == 0:
            button.text = 'X'
        else:
            button.text = 'O'

        # Change available places
        self.board.toggle_boxers(button_id[1])

        # Change to next turn
        self.turn = (self.turn + 1) % len(self.player_list)

        # Check if someone has already won
        winner = self.board.check_state()
        if winner != UNOCCUPIED:
            self.player_list[self.turn].add_points(1)
            msg = 'Player ' + str(self.turn) + 'wins!'
            self.popup_message('Game over!', msg, on_dismiss=self.new_match)
            return

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
                        height=box_size, size_hint=(None,None), id=self.id + '_' + str(i))
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
