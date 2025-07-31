import tkinter as tk
import pyperclip

class SegmentEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("7-segment symbol editor")

        self.segments = [False]*7

        self.canvas = tk.Canvas(root, width=220, height=280, bg='white')
        self.canvas.pack()

        self.segment_coords = {
            0: (50, 20, 150, 40),   # A - top
            1: (160, 45, 180, 125), # B - top right
            2: (160, 145, 180, 225),# C - bottom right
            3: (50, 230, 150, 250), # D - bottom
            4: (30, 145, 50, 225),  # E - bottom left
            5: (30, 45, 50, 125),   # F - top left
            6: (55, 125, 155, 145), # G - center
        }

        self.segment_ids = {}

        for i in range(7):
            x1, y1, x2, y2 = self.segment_coords[i]
            rect = self.canvas.create_rectangle(x1, y1, x2, y2, fill="grey", outline="black", width=2)
            self.segment_ids[i] = rect
            self.canvas.tag_bind(rect, "<Button-1>", lambda e, idx=i: self.toggle_segment(idx))

        self.label = tk.Label(root, text="Byte code: 0b0000000 (0x00)")
        self.label.pack(pady=10)

        self.update_code()

    def toggle_segment(self, idx):
        self.segments[idx] = not self.segments[idx]
        color = "red" if self.segments[idx] else "grey"
        self.canvas.itemconfig(self.segment_ids[idx], fill=color)
        self.update_code()
        self.copy_to_clipboard()

    def update_code(self):
        val = 0
        for i, on in enumerate(self.segments):
            if on:
                val |= (1 << i)
        bin_str = bin(val)[2:].rjust(7, '0')
        hex_str = hex(val)
        self.label.config(text=f"Byte code: 0b{bin_str} ({hex_str})")

    def copy_to_clipboard(self):
        val = 0
        for i, on in enumerate(self.segments):
            if on:
                val |= (1 << i)
        pyperclip.copy(f"0b{bin(val)[2:].rjust(7, '0')}  // hex {hex(val)}")
        print(f"Copied to clipboard: 0b{bin(val)[2:].rjust(7, '0')}  // hex {hex(val)}")

if __name__ == "__main__":
    root = tk.Tk()
    app = SegmentEditor(root)
    root.mainloop()
