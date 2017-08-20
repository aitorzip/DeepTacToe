var login = {
  create: function() {
    var username = this.add.inputField(this.world.centerX - 87, this.world.centerY - 80, {
      font: '18px Arial',
      fill: '#212121',
      fontWeight: 'bold',
      width: 175,
      padding: 8,
      borderWidth: 1,
      borderColor: '#000',
      borderRadius: 6,
      placeHolder: 'User Name'
    });

    var password = this.add.inputField(this.world.centerX - 87, this.world.centerY - 40, {
      font: '18px Arial',
      fill: '#212121',
      fontWeight: 'bold',
      width: 175,
      padding: 8,
      borderWidth: 1,
      borderColor: '#000',
      borderRadius: 6,
      placeHolder: 'Password',
      type: PhaserInput.InputType.password
    });

    this.add.button(this.world.centerX - 87, this.world.centerY + 20, 'button', this.actionOnClick, this, 2, 1, 0);
  },

  actionOnClick: function() {
    console.log('click!');
    this.state.start('play');
  }
};
