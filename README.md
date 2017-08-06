# Deep Tac Toe
Multi-platform (Windows, GNU/Linux, Mac, Android, iOS) funnier version of the original Tic Tac Toe

## Requirements

* Kivy
* Python 2.7

Run with:

```python main.py```

## Build for android

To build an APK for android make sure you have [buildozer](https://github.com/kivy/buildozer) installed and execute the following command:

```buildozer android debug```

The APK can be found in the `bin` directory.

If you have an android device connected (or emulator) it will automatically be pushed by issuing the following command:

```buildozer android deploy```
