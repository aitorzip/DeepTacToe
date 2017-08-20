var load = {
  preload: function() {
    var loadingBar = this.add.sprite(160,240,'loading');
    loadingBar.anchor.setTo(0.5,0.5);
    this.load.setPreloadSprite(loadingBar);

    this.add.plugin(PhaserInput.Plugin);
    this.load.spritesheet('box', 'assets/number-buttons-90x90.png', 90, 90);
    this.load.spritesheet('button', 'assets/button_sprite_sheet.png', 193, 71);
  },

  create: function() {
    this.state.start('login');
  }
};
