[
    tk := __import__("tkinter"),

    API := type("API", (), {
        "attr": lambda self, name, value: [
            setattr(self, name, value),
        ]
    }),

    App := type("App", (tk.Tk, API), {
        "__init__": lambda self: [
            size := 1024,
            self.attr("size", size),

            (tk.Tk).__init__(self),
            self.title("Vec"),
            self.geometry(f"{size}x{size}"),
            self.resizable(False, False),

            field := tk.Canvas(self),
            field.grid(row=0, column=0, sticky="nsew"),

            field.create_line(size / 2, 0, size / 2, size, width=4),
            field.create_line(0, size / 2, size, size / 2, width=4),

            [
                [
                    [
                        s := size // 32,
                        field.create_rectangle(x * s, y * s, x * s + s, y * s + s),
                    ]
                    for y in range(32)
                ] for x in range(32)
            ],
            self.attr("field", field),

            self.columnconfigure(0, weight=1),
            self.rowconfigure(0, weight=1),
            
            self.attr("coords", []),
            self.attr("ids", []),
            self.attr("cur", False),

            self.bind("<Button-1>", self.click),
            self.bind("<Button-2>", self.pos),
            self.bind("<Button-3>", self.skip),
            self.bind("<c>", self.clear),
            self.bind("<u>", self.upload),
        None][-1],

        "click": lambda self, event: [
            coords := self.coords,

            self.ids != [] and [
                self.field.delete(id)
                for id in self.ids
            ],
            coords.append([event.x, event.y, self.cur]),
            self.attr("ids", []),

            [
                self.ids.append(
                    self.field.create_line(
                        coords[n - 1][0], coords[n - 1][1],
                        coords[n][0], coords[n][1],
                        fill="red", width=5
                    )
                )
                for n in range(1, len(coords))
                if coords[n][2]
            ],

            self.attr("cur", True),
        None][-1],

        "pos": lambda self, event: [
            print((event.x - self.size // 2) // (self.size // 32), end=' '),
            print((self.size // 2 - event.y) // (self.size // 32), end='\n'),
        ],

        "skip": lambda self, event: [
            self.attr("cur", False),
        None][-1],

        "upload": lambda self, event: [
            print("{"),
            [
                [
                    xs := (x - self.size // 2) // (self.size // 32),
                    ys := (y - self.size // 2) // (self.size // 32),
                    print(f"    {{ {xs}, {ys}, {int(s)} }}", end=",\n")
                ] for x, y, s in self.coords
            ],
            print("}"),
        None][-1],

        "clear": lambda self, event: [
            self.attr("coords", []),
            self.attr("cur", False),
        None][-1],

        "process": lambda self: [
            self.mainloop(),
        None][-1]
    }),

    main := lambda: [
        app := App(),
        app.process(),
    ],

    __name__ == "__main__" and main()
]
