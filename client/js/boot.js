var boot = {
  preload: function() {
    this.load.image('loading', 'assets/loading.png')
  },

  create: function() {
    this.state.start('load')
  }
};
