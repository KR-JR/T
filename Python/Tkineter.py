import tkinter as tk
import tkinter.font as font


class App:
    def __init__(self):
        root = ()

        self.customFont = font.Font(family = "Helvetica", size = 12)

        buttonframe = tk.Frame()
        label = tk.Label(root, text = "Hello World", font = self.customFont)
        buttonframe.pack()
        label.pack()

        bigger = tk.Button(root, text = "폰트를 크게", command = self.BigFont)
        smaller = tk.Button(root, text = "폰트를 작게", command = self.SmallFont)
        bigger.pack()
        smaller.pack()
        root.mainloop

    def BigFont(self):
        size = self.customFont['size']
        self.customFont.configure(size = size +2)

    def SmallFont(self):
        size = self.customFont['size']
        self.customFont.configure(size = size -2)

app = App()