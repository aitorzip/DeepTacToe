var play = {
  create: function() {
    // Add sprites
    for (var i = 0; i < 3; i++) {
      for (var j = 0; j < 3; j++) {
       for (var k = 0; k < 3; k++) {
         for (var l = 0; l < 3; l++) {
           var x = j*290 + l*95;
           var y = i*290 + k*95;
           var button = this.add.button(x, y, 'box', this.actionOnClick, this, 1, 0, 2, 0)
           button.grid_id = i*3 + j;
           button.id = k*3 + l;
         }
       }
      }
    }
  },

  actionOnClick: function() {
    console.log('click!');
  }
};
