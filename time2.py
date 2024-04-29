import tkinter as tk
import time
import threading
import winsound

class TimerApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Timer Application")

        self.label = tk.Label(master, text="Enter time in seconds:")
        self.label.pack()

        self.time_entry = tk.Entry(master)
        self.time_entry.pack()

        self.timer_display = tk.Label(master, text="00:00")
        self.timer_display.pack()

        self.start_button = tk.Button(master, text="Start", command=self.start_timer)
        self.start_button.pack()

        self.stop_button = tk.Button(master, text="Stop", command=self.stop_timer)
        self.stop_button.pack()

        self.running = False
        self.time_in_seconds = 0

    def start_timer(self):
        if not self.running:
            try:
                self.time_in_seconds = int(self.time_entry.get())
            except ValueError:
                self.time_in_seconds = 0
            self.running = True
            self.thread = threading.Thread(target=self.run_timer)
            self.thread.start()

    def run_timer(self):
        while self.running and self.time_in_seconds > 0:
            self.timer_display.config(text=f"{self.time_in_seconds // 60:02}:{self.time_in_seconds % 60:02}")
            time.sleep(1)
            self.time_in_seconds -= 1

        if self.running:
            self.timer_display.config(text="Time's up!")
            winsound.Beep(440, 1000)  # Frequency 440 Hz, Duration 1000 ms
            self.running = False

    def stop_timer(self):
        self.running = False

def main():
    root = tk.Tk()
    app = TimerApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
